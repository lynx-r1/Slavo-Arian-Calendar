#include <QDebug>
#include <QGraphicsEffect>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QMessageBox>
#include <QParallelAnimationGroup>
#include <QPixmap>
#include <QPluginLoader>
#include <QPropertyAnimation>
#include <QSettings>
#include <QStateMachine>
#include <QToolBar>

#include "sadefines.h"
#include "interfaces.h"

#include "sagraphicstoolbar.h"
#include "sagraphicsbuttongroup.h"
#include "sagraphicsitemgroup.h"

#include "sakalendarsettingsdialog.h"
#include "sakalendar.h"

const QSize ToolButtonSize = QSize(48, 48);

SAKalendar::SAKalendar(QWidget *parent)
    : QMainWindow(parent)
    , mSAKalendarView(0)
    , mMainToolBar(new SAGraphicsToolBar)
    , mDertyWidgetPos(false)
{
    setupUi(this);

    mMainToolBar->setOrientation(Qt::Vertical);

    setWindowIcon(QIcon(":/SAKalendar/SAKalendarApp/images/icons/kalyadnik.png"));

    createKalyadnikLogo();
    createSAKalendarView();
    loadPlugins();
    createToolBar();
    setupConnections();

    readSettings();
}

SAKalendar::~SAKalendar()
{
}

// ---------------------------- protected ---------------------------------- //
void SAKalendar::closeEvent(QCloseEvent *e)
{
    writeSettings();
    e->accept();
}

void SAKalendar::resizeEvent(QResizeEvent *e)
{
    dertyWidgetPos();
    mSAKalendarView->resize(e->size());
    mSAKalendarScene->setSceneRect(mSAKalendarView->rect());
}

// ------------------------- private slots --------------------------------- //
void SAKalendar::showAbout()
{
    QString plugins;
    foreach (QString name, mPluginNames)
        plugins += QString("<span style='white-space:pre; font-style:italic'>    %1;</span><br>").arg(name);
    plugins[plugins.lastIndexOf(";")] = QChar('.');

    QString about = tr("<b>Славяно-Арийский Календарь (Версия %1)</b><br>"
                       "Программа наглядно представляет Славяно-Арийский Календарь.<br>"
                       "В программу включены модули:<br>"
                       "%2<br>"
                       "Программа написана на Qt %3.<br><br>"
                       "<b>Разработчик</b><br>"
                       "<span style='white-space:pre'>    Рысь (lynx_r)</span>")
            .arg(QCoreApplication::applicationVersion()).arg(plugins).arg(qVersion());

    QMessageBox::about(this, tr("О программе Даарийски Круголет Числобога"), about);
}

void SAKalendar::toggleVisiblePlugin(bool visible)
{
    foreach (QGraphicsWidget *widget, mPluginsList) {
        QObject *obj = sender();
        if (obj) {
            QString widgetName = widget->objectName();
            QString actionName = obj->objectName();
            actionName.chop(6);
            if (widgetName == actionName) {
                const int duration = 1000;
                widget->setTransformOriginPoint(widget->boundingRect().center());

                QParallelAnimationGroup *group = new QParallelAnimationGroup;

                QPropertyAnimation *scaleAnim = new QPropertyAnimation(widget, "scale", group);
                scaleAnim->setDuration(duration);
                scaleAnim->setEasingCurve(visible ? QEasingCurve::OutBack : QEasingCurve::InBack);
                scaleAnim->setEndValue(visible);
                group->addAnimation(scaleAnim);

                QPropertyAnimation *opacityAnim = new QPropertyAnimation(widget, "opacity", group);
                opacityAnim->setDuration(duration);
                opacityAnim->setEasingCurve(visible ? QEasingCurve::OutBack : QEasingCurve::InBack);
                opacityAnim->setEndValue(visible);
                group->addAnimation(opacityAnim);

                group->start(QAbstractAnimation::DeleteWhenStopped);
                break;
            }
        }
    }

}

void SAKalendar::updateStaticItemsPos(const QRectF &rect)
{
    int center = rect.width() / 2 - mKalyadnikLogo->boundingRect().width() / 2;
    mKalyadnikLogo->setPos(rect.topLeft() + QPointF(center, 2));
    mMainToolBar->setPos(rect.topLeft() + QPointF(4, 4));
}

void SAKalendar::resetItemPos()
{
    if (mDertyWidgetPos && !mPluginsList.isEmpty()) {
        int i = 0;
        int pluginWidth = mMainToolBar->preferredWidth();
        QParallelAnimationGroup *group = new QParallelAnimationGroup;
        foreach (QGraphicsWidget *widget, mPluginsList) {
            int floatY = height() / 2 - widget->preferredHeight() / 2;
            QPointF pos = QPointF(pluginWidth, floatY);

            QPropertyAnimation *anim = new QPropertyAnimation(widget, "pos", group);
            anim->setDuration(2000 + i * 1000);
            anim->setEndValue(pos);
            anim->setEasingCurve(QEasingCurve::InOutBack);

            pluginWidth += widget->preferredWidth() - widget->preferredWidth() / 4;
            ++i;
        }

        group->start(QAbstractAnimation::DeleteWhenStopped);
        connect(group, SIGNAL(finished()), this, SLOT(cleanWidgetPos()));
    }
}

void SAKalendar::dertyWidgetPos()
{
    mDertyWidgetPos = true;
}

void SAKalendar::cleanWidgetPos()
{
    mDertyWidgetPos = false;
}

// ----------------------------- private ----------------------------------- //
void SAKalendar::createToolBar()
{
    QColor buttonColor(200, 200, 255, 127);
    mRangeWidgetButton = new SAGraphicsPushButton;
    mRangeWidgetButton->setFixedSize(ToolButtonSize);
    mRangeWidgetButton->setBackgroundColor(buttonColor);
    mRangeWidgetButton->setToolTip(tr("Начальное расположение"));
    mRangeWidgetButton->setPixmap(QPixmap(":/SAKalendar/SAKalendarApp/images/icons/ranged.png"));
    connect(mRangeWidgetButton, SIGNAL(clicked()), this, SLOT(resetItemPos()));

    mSettingsButton = new SAGraphicsPushButton;
    mSettingsButton->setFixedSize(ToolButtonSize);
    mSettingsButton->setBackgroundColor(buttonColor);
    mSettingsButton->setToolTip(tr("Настройки"));
    mSettingsButton->setPixmap(QPixmap(":/SAKalendar/SAKalendarApp/images/icons/settings.png"));
    connect(mSettingsButton, SIGNAL(clicked()), mSettingsDialog, SLOT(show()));

    mAboutButton = new SAGraphicsPushButton;
    mAboutButton->setFixedSize(ToolButtonSize);
    mAboutButton->setBackgroundColor(buttonColor);
    mAboutButton->setToolTip(tr("О программе"));
    mAboutButton->setPixmap(QPixmap(":/SAKalendar/SAKalendarApp/images/icons/kalyadnik.png"));
    connect(mAboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));

    mQuitButton = new SAGraphicsPushButton;
    mQuitButton->setFixedSize(ToolButtonSize);
    mQuitButton->setBackgroundColor(buttonColor);
    mQuitButton->setToolTip(tr("Выход"));
    mQuitButton->setPixmap(QPixmap(":/SAKalendar/SAKalendarApp/images/icons/power.png"));
    connect(mQuitButton, SIGNAL(clicked()), this, SLOT(close()));

    QSettings s;
    mMainToolBar->setHideOnHoverLeave(s.value("/SAKalendar/SAKalendarApp/HideMainToolBar", true).toBool());

    mMainToolBar->addSeparator();
    mMainToolBar->addWidget(mRangeWidgetButton);
    mMainToolBar->addSeparator();
    mMainToolBar->addWidget(mSettingsButton);
    mMainToolBar->addWidget(mAboutButton);
    mMainToolBar->addSeparator();
    mMainToolBar->addWidget(mQuitButton);
}

void SAKalendar::createSAKalendarView()
{
    QSettings s;
    QColor sceneBackgroundColor = s.value("/SAKalendar/SAKalendarApp/BackgroundColor",
                                          QColor(230, 255, 230)).value<QColor>();

    mSAKalendarScene = new QGraphicsScene;
    mSAKalendarScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    mSAKalendarScene->setBackgroundBrush(sceneBackgroundColor);
    mSAKalendarScene->addItem(mKalyadnikLogo);
    mSAKalendarScene->addItem(mMainToolBar);

    mSAKalendarView = new QGraphicsView(mSAKalendarScene, this);
    mSAKalendarView->setAlignment(Qt::AlignCenter);
    mSAKalendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mSAKalendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mSAKalendarView->setCacheMode(QGraphicsView::CacheBackground);
    mSAKalendarView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    mSAKalendarView->setOptimizationFlags(mSAKalendarView->optimizationFlags()
                                          | QGraphicsView::DontAdjustForAntialiasing);

    setCentralWidget(mSAKalendarView);

    mSettingsDialog = new SAKSettingsDialog(this, mSAKalendarView->scene(), mMainToolBar, mKalyadnikLogo);
}

void SAKalendar::createKalyadnikLogo()
{
    mKalyadnikLogo = new SAGraphicsItemGroup;

    QGraphicsDropShadowEffect *dropShadowName = new QGraphicsDropShadowEffect;
    dropShadowName->setOffset(2);
    dropShadowName->setBlurRadius(4);
    dropShadowName->setColor(QColor(32, 32, 32, 200));

    QGraphicsDropShadowEffect *dropShadowSlogan = new QGraphicsDropShadowEffect;
    dropShadowSlogan->setOffset(2);
    dropShadowSlogan->setBlurRadius(4);
    dropShadowSlogan->setColor(QColor(32, 32, 32, 200));

    QGraphicsPixmapItem *pixmap = new QGraphicsPixmapItem(
            QPixmap(":/SAKalendar/SAKalendarApp/images/icons/kalyadnik.png"));

    QGraphicsTextItem *name = new QGraphicsTextItem(tr("Славяно-Арийский Календарь"));
    QFont f(SAFONTFAMILYIZHITSAC);
    f.setPointSize(28);
    name->setDefaultTextColor(Qt::red);
    name->setFont(f);
    name->setPos(pixmap->boundingRect().width(), pixmap->boundingRect().height() / 2 -
                 name->boundingRect().height() / 2);
    name->setGraphicsEffect(dropShadowName);

    QGraphicsTextItem *slogan = new QGraphicsTextItem(tr("Слава Числобогу!"));
    slogan->setPos(pixmap->boundingRect().width() + 20, name->boundingRect().height());
    slogan->setFont(SAFONTFAMILYIZHITSAC);
    slogan->setGraphicsEffect(dropShadowSlogan);

    QGraphicsRectItem *rect = new QGraphicsRectItem(-2, -2, pixmap->boundingRect().width() +
                                                    name->boundingRect().width() + 2, 80);
    rect->setBrush(QColor(90,160, 80, 127));
    rect->setPen(Qt::NoPen);

    mKalyadnikLogo->addToGroup(rect);
    mKalyadnikLogo->addToGroup(pixmap);
    mKalyadnikLogo->addToGroup(name);
    mKalyadnikLogo->addToGroup(slogan);

    QSettings s;
    mKalyadnikLogo->setOpacity(s.value("/SAKalendar/SAKalendarApp/ShowLogo", true).toBool());
}

void SAKalendar::setupConnections()
{
    connect(mSAKalendarScene, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(updateStaticItemsPos(QRectF)));
}

void SAKalendar::readSettings()
{
    QSettings s;
    QByteArray initGeometry = QByteArray::fromHex("01d9d0cb0001000000000000000000000000059f00000"
                                                  "36000000002000000150000059d0000035e000000000200");
    restoreGeometry(s.value("/SAKalendar/SAKalendarApp/Geometry", initGeometry).toByteArray());

    if (!mPluginsList.isEmpty()) {
        int i = 0;
        int pluginWidth = mMainToolBar->preferredWidth();
        QParallelAnimationGroup *group = new QParallelAnimationGroup;
        foreach (QGraphicsWidget *widget, mPluginsList) {
            int floatY = height() / 2 - widget->preferredHeight() / 2;
            QPointF initPos = QPointF(pluginWidth, floatY);
            QPointF pos = s.value(QString("/SAKalendar/SAKalendarApp/%1/scenePos")
                                  .arg(widget->objectName()),
                                  initPos).toPointF();
            mDertyWidgetPos = mDertyWidgetPos || pos != initPos;

            QPropertyAnimation *anim = new QPropertyAnimation(widget, "pos", group);
            anim->setDuration(2000 + i * 1000);
            anim->setStartValue(QPoint(0, 0));
            anim->setEndValue(pos);
            anim->setEasingCurve(QEasingCurve::InOutBack);
            group->addAnimation(anim);

            QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect;
            widget->setGraphicsEffect(opacity);

            anim = new QPropertyAnimation(opacity, "opacity", group);
            anim->setDuration(2000 + i * 1000);
            anim->setStartValue(0.2);
            anim->setEndValue(1);
            anim->setEasingCurve(QEasingCurve::InOutBack);
            group->addAnimation(anim);

            bool active = s.value(QString("/SAKalendar/SAKalendarApp/%1/active")
                                  .arg(widget->objectName())).toBool();
            if (active) {
                if (i * 2 < mGroupStates->children().count()) {
                    QState *initialState = qobject_cast<QState *>(mGroupStates->children().at(i * 2));
                    if (initialState)
                        mGroupStates->setInitialState(initialState);
                }
            }

            mSAKalendarScene->addItem(widget);
            pluginWidth += widget->preferredWidth() - widget->preferredWidth() / 4;
            ++i;
        }

        if (!mGroupStates->initialState()) {
            QState *initialState = qobject_cast<QState *>(mGroupStates->children().at(0));
            if (initialState)
                mGroupStates->setInitialState(initialState);
        }

        group->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void SAKalendar::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SAKalendarApp/Geometry", saveGeometry());

    foreach (QGraphicsWidget *widget, mPluginsList) {
        s.setValue(QString("/SAKalendar/SAKalendarApp/%1/scenePos")
                   .arg(widget->objectName()), widget->scenePos());
        if (widget->zValue() != 0)
            s.setValue(QString("/SAKalendar/SAKalendarApp/%1/active")
                       .arg(widget->objectName()), true);
        else
            s.setValue(QString("/SAKalendar/SAKalendarApp/%1/active")
                       .arg(widget->objectName()), false);
    }
}

void SAKalendar::loadPlugins()
{
    mStateMachine = new QStateMachine(this);
    mStateMachine->setGlobalRestorePolicy(QStateMachine::RestoreProperties);

    mGroupStates = new QState(mStateMachine);
    mGroupStates->setObjectName("mGroupStates");

    mPluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_UNIX)
    mPluginsDir.cd("../plugins");
#elif defined(Q_OS_WIN)
    mPluginsDir.cd("plugins");
#elif defined(Q_OS_MAC)
    if (mPluginsDir.dirName() == "MacOS") {
        mPluginsDir.cdUp();
        mPluginsDir.cdUp();
        mPluginsDir.cdUp();
    }
#endif

    foreach (QString fileName, mPluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(mPluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            KrugoletChislobogaInterface *krugolet = qobject_cast<KrugoletChislobogaInterface *>(plugin);
            if (krugolet) {
                QString dk4Name = tr("Даарийский Круголет Числобога");
                QGraphicsWidget *k = krugolet->sozdatKrugolet(dk4Name, mGroupStates, mStateMachine);
                addPlugin(k, QPixmap(":/SAKalendar/SAKalendarApp/images/icons/dk4.png"), dk4Name);
            }

            ChasyInterface *chasy = qobject_cast<ChasyInterface *>(plugin);
            if (chasy) {
                QString sachasyName = tr("Славяно-Арийские часы");
                QGraphicsWidget *ch = chasy->sozdatGraphicsChasy(sachasyName, mGroupStates, mStateMachine);
                addPlugin(ch, QPixmap(":/SAKalendar/SAKalendarApp/images/icons/sachasy.png"), sachasyName);
            }

            SorokovnikInterface *sorokovnik = qobject_cast<SorokovnikInterface *>(plugin);
            if (sorokovnik) {
                QString sorokovnikName = tr("Сороковник");
                QGraphicsWidget *s = sorokovnik->sozdatSorokovnik(sorokovnikName, mGroupStates, mStateMachine);
                addPlugin(s, QPixmap(":/SAKalendar/SAKalendarApp/images/icons/sorokovnik.png"), sorokovnikName);
            }
        } else {
            qWarning() << loader.errorString();
        }
    }

    mStateMachine->setInitialState(mGroupStates);
    mStateMachine->start();
}

void SAKalendar::addPlugin(QGraphicsWidget *plugin, const QPixmap &pixmap,
                           const QString &pluginName)
{
    if (!plugin) {
        qWarning() << tr("Не удалось загрузить плагин %1").arg(pluginName);
        return;
    }

    mPluginNames.append(pluginName);
    plugin->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    plugin->setFlags(QGraphicsItem::ItemIsMovable);
    mPluginsList.append(plugin);
    connect(plugin, SIGNAL(xChanged()), this, SLOT(dertyWidgetPos()));
    connect(plugin, SIGNAL(yChanged()), this, SLOT(dertyWidgetPos()));

    QColor buttonColor(255, 200, 200, 127);
    SAGraphicsPushButton *pbn = new SAGraphicsPushButton;
    pbn->setFixedSize(ToolButtonSize);
    pbn->setObjectName(plugin->objectName() + "Widget");
    pbn->setCheckable(true);
    pbn->setChecked(true);
    pbn->setBackgroundColor(buttonColor);
    pbn->setToolTip(pluginName);
    pbn->setPixmap(pixmap);
    connect(pbn, SIGNAL(clicked(bool)), this, SLOT(toggleVisiblePlugin(bool)));

    mMainToolBar->addWidget(pbn);
}
