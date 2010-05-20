#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QSettings>
#include <QSqlQuery>

#include "sadefines.h"

#include "dk4appsettingsdialog.h"
#include "sadate_cpp.h"
#include "dk4.h"

#include "dk4app.h"

DK4App::DK4App(QWidget *parent)
    : QMainWindow(parent)
    , mMessageTimeout(4000)
{
    setupUi(this);
    readSettings();
    setWindowIcon(QIcon(":/SAKalendar/DK4App/images/icons/dk4.png"));

    createDK4();
    setupConnections();

    mOpisanieLetTextEdit->setFont(QFont(SAFONTFAMILYIZHITSAC, 12));
}

void DK4App::closeEvent(QCloseEvent *e)
{
    writeSettings();
    e->accept();
}

void DK4App::showAbout()
{
    QString about = tr("<b>Даарийский Круголет Числобога (Версия %1)</b><br>"
                       "Программа наглядно представляет Даарийский Круголет Числобога, "
                       "а так содержит описание лет Круга Жизни.<br><br>"
                       "Программа написана на Qt %2.<br><br>"
                       "<b>Разработчик</b><br>"
                       "<span style='white-space:pre'>    Рысь (lynx_r)</span>")
            .arg(QCoreApplication::applicationVersion()).arg(qVersion());

    QMessageBox::about(this, tr("О программе Даарийски Круголет Числобога"), about);
}

void DK4App::kzhizniSelected(int lvkzhizni)
{
    QSqlDatabase opisanieLetDB = QSqlDatabase::database(mDK4->dbOpisanieLetConnectionName());
    if (!opisanieLetDB.isOpen())
        return;

    if (lvkzhizni == -1) {
        clearInfoGui();
        return;
    }

    QSqlQuery q(opisanieLetDB);
    q.prepare(QString("SELECT name, descr, symbol, power, color FROM opisanie_let WHERE id=?"));
    q.addBindValue(lvkzhizni);
    q.exec();
    q.next();
    if (q.isValid()) {
        QString name = q.value(0).toString();
        QString description = q.value(1).toString();
        QString symbol = q.value(2).toString();
        QString power = q.value(3).toString();
        QString color = q.value(4).toString();

        mColorLabel->setStyleSheet(QString("color: %1").arg(color));
        mColorLabel->setText(colorText(color));

        mPowerLabel->setText(power);
        mSymbolLabel->setText(symbol);

        mOpisanieLetTextEdit->setText(QString("<h3 style='color: red'>%1</h3>%2").arg(name).arg(description));
    } else {
        clearInfoGui();
    }

    int leto = SADate::letoFromSMZH(mDK4->kzhizniOtSMZH(), lvkzhizni);
    mLetoOtSMZHLabel->setText(QString::number(leto));
    int nr = SADate::nachRamkhata(leto);
    mNachRamhataLabel->setText(SADniNedeli_Opisanie[nr - 1].first);
}

void DK4App::readSettings()
{
    QSettings s;
    restoreGeometry(s.value("/SAKalendar/DK4App/Geometry").toByteArray());
    restoreState(s.value("/SAKalendar/DK4App/State").toByteArray());
}

void DK4App::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/DK4App/Geometry", saveGeometry());
    s.setValue("/SAKalendar/DK4App/State", saveState());
}

void DK4App::createDK4()
{
    QSettings s;
    s.beginGroup("/SAKalendar/DK4App");
    bool useAltColor = s.value("AltColor", false).toBool();
    QColor conturColor = s.value("ConturColor", QColor(100, 200, 100, 127)).value<QColor>();
    QColor diagonalColor = s.value("DiagonalColor", QColor(0, 200, 200, 64)).value<QColor>();
    QColor shadowColor = s.value("ShadowColor", QColor(Qt::blue)).value<QColor>();
    s.endGroup();

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setColor(shadowColor);
    shadow->setOffset(2);
    shadow->setBlurRadius(12);

    mDK4 = new DK4;
    mDK4->setAltColor(useAltColor);
    mDK4->setConturColor(conturColor);
    mDK4->setDiagonalColor(diagonalColor);
    mDK4->setGraphicsEffect(shadow);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->addItem(mDK4);
    mDK4->scale(0.9, 0.9);

    QPalette p = mDK4GraphicsView->viewport()->palette();
    p.setColor(QPalette::Base, Qt::transparent);
    mDK4GraphicsView->viewport()->setPalette(p);
    mDK4GraphicsView->setScene(scene);
    mDK4GraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mDK4GraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mDK4GraphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
#if defined(Q_OS_UNIX)
    mDK4GraphicsView->setMinimumSize(642, 608);
#elif defined(Q_OS_WIN)
    mDK4GraphicsView->setMinimumSize(652, 618);
#endif

    mSettingsDialog = new DK4AppSettingsDialog(mDK4, shadow, this);
}

void DK4App::setupConnections()
{
    connect(mDK4, SIGNAL(selected(int)), this, SLOT(kzhizniSelected(int)));
    connect(mSettingsAction, SIGNAL(triggered()), mSettingsDialog, SLOT(show()));

    connect(mQuitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(mAboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
}

void DK4App::clearInfoGui()
{
    mOpisanieLetTextEdit->clear();
    mPowerLabel->clear();
    mSymbolLabel->clear();
    mColorLabel->clear();
    mColorLabel->setStyleSheet(QString());
    mNachRamhataLabel->clear();
    mLetoOtSMZHLabel->clear();
}

QString DK4App::colorText(const QString &color)
{
    for (int i = 0; i < SACOLCVETOV; ++i) {
        if (SACvetaHTMLCveta[i].second == color) {
            QString c = SACvetaHTMLCveta[i].first;
            c.replace("h", "е");
            return c;
        }
    }
    return QString();
}
