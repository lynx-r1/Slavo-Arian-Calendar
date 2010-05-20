#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneResizeEvent>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QStateMachine>
#include <QStyleOptionGraphicsItem>
#include <QTextEdit>
#include <QTimer>

#include "sadefineslib.h"
#include "sagraphicslabelitem.h"
#include "sagraphicstitlebarbutton.h"

#include "sagraphicswindow.h"

SAGraphicsWindowBase::SAGraphicsWindowBase(const QString &title, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mStateMachine(new QStateMachine)
    , mCentralWidget(0)
    , mPrevParent(0)
    , mWidgetWithEffects(0)
    , mTitle(title)
    , mTitleColor(QColor(180, 255, 180, 200))
    , mFirstWidgetWithEffects(true)
{
    setObjectName("theSAGraphicsWindowBase");
    setWindowTitle(title);

    mMainLayout = new QGraphicsLinearLayout;
    mMainLayout->setOrientation(Qt::Vertical);
    mMainLayout->setSpacing(0);

    mMainLayout->setItemSpacing(0, 18);
    mMainLayout->addItem(createTitleBar());

    mPrevParent = this;

    mOpacityEffect.setOpacity(0.9);
    mBlurEffect.setBlurRadius(4.0);
    mDropShadowEffect.setBlurRadius(8);

    QTextEdit *te = new QTextEdit;
    te->setFont(QFont(SAFONTFAMILYIZHITSAC, 12));
    te->setReadOnly(true);

    mInfoWidget = new QGraphicsProxyWidget(this);
    mInfoWidget->setWidget(te);
    mInfoWidget->setOpacity(0);
}

QGraphicsWidget *SAGraphicsWindowBase::centralWidget() const
{
    return mCentralWidget;
}

void SAGraphicsWindowBase::setCentralWidget(QGraphicsWidget *widget)
{
    if (mCentralWidget) {
        mMainLayout->removeItem(mCentralWidget);
        delete mCentralWidget;
    }

    // добвляем центральный виджет
    mCentralWidget = widget;
    mMainLayout->addItem(mCentralWidget);

    if (mWidgetWithEffects)
        mWidgetWithEffects->setLayout(mMainLayout);
    else
        setLayout(mMainLayout);

    // добавляем эффекты для сворачивания
    if (mStateMachine) {
        QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect;
        blurEffect->setBlurRadius(0);
        mCentralWidget->setGraphicsEffect(blurEffect);

        QState *off = new QState();
        off->setObjectName("off");
        off->assignProperty(mCentralWidget, "opacity", 0.0);
        off->assignProperty(blurEffect, "blurRadius", 12);

        QState *on = new QState();
        on->setObjectName("on");
        on->assignProperty(mCentralWidget, "opacity", 1.0);
        on->assignProperty(blurEffect, "blurRadius", 0);

        off->addTransition(mMinimizeTitleButton, SIGNAL(clicked(bool)), on);
        connect(off, SIGNAL(exited()), this, SLOT(showMinimized()));

        on->addTransition(mMinimizeTitleButton, SIGNAL(clicked(bool)), off);
        connect(on, SIGNAL(exited()), this, SLOT(showNormal()));

        mStateMachine->addState(off);
        mStateMachine->addState(on);

        QPropertyAnimation *animation = new QPropertyAnimation(mCentralWidget, "opacity");
        animation->setDuration(1000);
        mStateMachine->addDefaultAnimation(animation);

        animation = new QPropertyAnimation(blurEffect, "blurRadius");
        animation->setDuration(1000);
        mStateMachine->addDefaultAnimation(animation);

        mStateMachine->setInitialState(on);
        mStateMachine->start();
    }
}

QRectF SAGraphicsWindowBase::boundingRect() const
{
    if (mPrevParent != this) {
        QRectF parentRect = mPrevParent->boundingRect();
        if (mInfoWidget->opacity() == 1) {
            QRectF infoRect = mInfoWidget->geometry();
            return QRectF(parentRect.topLeft(), infoRect.bottomRight() + QPointF(18, 18));
        } else {
            return parentRect;
        }
    } else {
        return QGraphicsWidget::boundingRect();
    }
}

void SAGraphicsWindowBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setClipRect(option->exposedRect);

    // градиент
    QRectF titleRect = mMainLayout->itemAt(0)->geometry();
    QLinearGradient lgr(titleRect.topLeft(), titleRect.bottomLeft());
    lgr.setColorAt(0, Qt::white);
    lgr.setColorAt(0.4, mTitleColor.darker(115));
    lgr.setColorAt(0.6, mTitleColor.darker(115));
    lgr.setColorAt(1, mTitleColor.lighter(120));

    // рисуем закруглённый прямоугольник для заглавия
    painter->setPen(Qt::NoPen);
    painter->setBrush(lgr);
    painter->drawRoundedRect(titleRect, 9, 9);

    // эффект стекла
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::white, 0.01));
    painter->setOpacity(0.3);
    painter->drawRoundedRect(titleRect.x(), titleRect.y(), titleRect.width(), titleRect.height() / 2 - 2, 9, 9);

    QGraphicsWidget::paint(painter, option, widget);
}

void SAGraphicsWindowBase::setInfoHtml(const QString &html)
{
    QTextEdit *te = qobject_cast<QTextEdit *>(mInfoWidget->widget());
    te->setHtml(html);
    te->setFocus();

    if (mInfoHtml != html)
        mInfoHtml = html;
}

void SAGraphicsWindowBase::showMinimized()
{
    mMainLayout->addItem(mCentralWidget);
    mMainLayout->activate();
    mMinimizeTitleButton->setToolTip(tr("Свернуть"));
}

void SAGraphicsWindowBase::showNormal()
{
    mMainLayout->removeAt(1);
    mMinimizeTitleButton->setToolTip(tr("Развернуть"));
}

void SAGraphicsWindowBase::showInfo(bool show, const QString &html)
{
    int duration = 1000;
    QPointF offsetXY(10, 10);
    QPointF offsetY(0, 10);
    QRectF r(mCentralWidget->geometry());

    if (mInfoHtml == html)
        return;

    mInfoHtml = html;

    QParallelAnimationGroup *parGroup = new QParallelAnimationGroup;
    QSequentialAnimationGroup *seqGroup = new QSequentialAnimationGroup;
    QPropertyAnimation *animPos = new QPropertyAnimation(mInfoWidget, "pos");
    QPropertyAnimation *animOpacity = new QPropertyAnimation(mInfoWidget, "opacity");
    if (mInfoWidget->opacity() == 1 && show) {
        // показываем окно с информацией о дне
        animPos->setDuration(duration);
        animPos->setStartValue(r.topRight() + offsetXY);
        animPos->setEndValue(r.topLeft() + offsetY);
        parGroup->addAnimation(animPos);

        animOpacity->setDuration(duration);
        animOpacity->setStartValue(1);
        animOpacity->setEndValue(0);
        parGroup->addAnimation(animOpacity);

        seqGroup->addAnimation(parGroup);

        // прячим окно с информацией о дне
        QParallelAnimationGroup *pargroup2 = new QParallelAnimationGroup;
        QPropertyAnimation *animPos2 = new QPropertyAnimation(mInfoWidget, "pos");
        animPos2->setDuration(duration);
        animPos2->setStartValue(r.topLeft() + offsetY);
        animPos2->setEndValue(r.topRight() + offsetXY);
        pargroup2->addAnimation(animPos2);

        QPropertyAnimation *animOpacity2 = new QPropertyAnimation(mInfoWidget, "opacity");
        animOpacity2->setDuration(duration);
        animOpacity2->setStartValue(0);
        animOpacity2->setEndValue(1);
        pargroup2->addAnimation(animOpacity2);

        seqGroup->addAnimation(pargroup2);

        seqGroup->start(QAbstractAnimation::DeleteWhenStopped);
        QTimer::singleShot(duration, this, SLOT(setInfoHtml()));
    } else if (show) {
        // показываем окно с информацией о дне
        animPos->setDuration(duration);
        animPos->setStartValue(r.topLeft() + offsetY);
        animPos->setEndValue(r.topRight() + offsetXY);
        parGroup->addAnimation(animPos);

        animOpacity->setDuration(duration);
        animOpacity->setStartValue(0);
        animOpacity->setEndValue(1);
        parGroup->addAnimation(animOpacity);

        parGroup->start(QAbstractAnimation::DeleteWhenStopped);
        setInfoHtml();
    } else {
        // прячим окно с информацией о дне
        animPos->setDuration(duration);
        animPos->setStartValue(r.topRight() + offsetXY);
        animPos->setEndValue(r.topLeft() + offsetY);
        parGroup->addAnimation(animPos);

        animOpacity->setDuration(duration);
        animOpacity->setStartValue(1);
        animOpacity->setEndValue(0);
        parGroup->addAnimation(animOpacity);

        parGroup->start(QAbstractAnimation::DeleteWhenStopped);

        mInfoHtml = "";
        QTimer::singleShot(duration, this, SLOT(setInfoHtml()));
    }
}

void SAGraphicsWindowBase::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget::resizeEvent(event);
    if (mMainLayout && mMainLayout->count() > 1 && mMainLayout->itemAt(1)) {
        QSizeF prefSize = mMainLayout->itemAt(1)->preferredSize();
        mInfoWidget->resize(prefSize.width() / 1.4, prefSize.height() - 20);
    }
}

QGraphicsLayout *SAGraphicsWindowBase::createTitleBar()
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout;
    l->setContentsMargins(5, 5, 0, 0);

    mMinimizeTitleButton = new SAGraphicsTitleBarButton(QColor(255, 255, 0, 220), this);
    mMinimizeTitleButton->setToolTip(tr("Свернуть"));
    mMinimizeTitleButton->setFixedSize(QSizeF(24, 24));

    l->addItem(mMinimizeTitleButton);

    QFont f(SAFONTFAMILYIZHITSAC, 16);
    mWindowTitle = new SAGraphicsLabelItem(mTitle, f, Qt::AlignCenter, "#cc0000");
    l->addItem(mWindowTitle);
    mWindowTitle->setFixedHeight(30);

    return l;
}

qreal SAGraphicsWindowBase::opacityEffect() const
{
    return mOpacityEffect.opacity();
}

void SAGraphicsWindowBase::setOpacityEffect(qreal opacity)
{
    mOpacityEffect.setOpacity(opacity);
}

qreal SAGraphicsWindowBase::blurRadiusEffect() const
{
    return mBlurEffect.blurRadius();
}

void SAGraphicsWindowBase::setBlurRadiusEffect(qreal radius)
{
    mBlurEffect.setBlurRadius(radius);
}

void SAGraphicsWindowBase::addGraphicsEffect(QGraphicsEffect *eff)
{
    QGraphicsWidget *newParent = new QGraphicsWidget;
    newParent->setGraphicsEffect(eff);

    if (mFirstWidgetWithEffects) {
        mWidgetWithEffects = newParent;
        mFirstWidgetWithEffects = false;
    }

    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(newParent);
    l->setContentsMargins(0, 0, 0, 0);
    l->addItem(mPrevParent);
    mPrevParent = newParent;
}

void SAGraphicsWindowBase::setInfoHtml()
{
    setInfoHtml(mInfoHtml);
}

bool SAGraphicsWindowBase::minimized() const
{
    return mMinimizeTitleButton->checked();
}


// ------------------------------------------------------------------------- //
SAGraphicsWindow::SAGraphicsWindow(const QString &title, bool effects,
                                   QGraphicsWidget *parent)
                                       : QGraphicsWidget(parent)
                                       , mEffects(effects)
{
    mWindow = new SAGraphicsWindowBase(title);

    if (mEffects) {
        mBlurEffect.setBlurRadius(2);
        mWindow->addGraphicsEffect(&mBlurEffect);

        mOpacityEffect.setOpacity(0.4);
        mWindow->addGraphicsEffect(&mOpacityEffect);

        mDropShadowEffect.setOffset(2);
        mDropShadowEffect.setBlurRadius(20);
        mDropShadowEffect.setColor(Qt::blue);
        mWindow->addGraphicsEffect(&mDropShadowEffect);
    }

    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    l->setContentsMargins(0, 0, 0, 0);
    l->addItem(mWindow->widget());
}

void SAGraphicsWindow::setCentralWidget(QGraphicsWidget *widget)
{
    mWindow->setCentralWidget(widget);
}

void SAGraphicsWindow::createAnimatedStateSelected(QState *parent, QStateMachine *machine)
{
    QState *s = new QState(parent);
    s->assignProperty(parentWidget(), "zValue", 100.0);
    if (mEffects) {
        s->assignProperty(&mOpacityEffect, "opacity", 0.8);
        s->assignProperty(&mBlurEffect, "blurRadius", 0);
    }
    parent->addTransition(this, SIGNAL(pressed()), s);

    machine->addDefaultAnimation(new QPropertyAnimation(parentWidget(), "zValue"));
    if (mEffects) {
        machine->addDefaultAnimation(new QPropertyAnimation(&mBlurEffect, "blurRadius"));
        machine->addDefaultAnimation(new QPropertyAnimation(&mOpacityEffect, "opacity"));
    }
}

void SAGraphicsWindow::update(const QRectF &rect)
{
    mWindow->update(rect);
    QGraphicsWidget::update(rect);
}

void SAGraphicsWindow::update(qreal x, qreal y, qreal width, qreal height)
{
    mWindow->update(x, y, width, height);
    QGraphicsWidget::update(x, y, width, height);
}

void SAGraphicsWindow::showInfo(bool show, const QString &html)
{
    mWindow->showInfo(show, html);
}

void SAGraphicsWindow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit pressed();
    QGraphicsWidget::mousePressEvent(event);
}
