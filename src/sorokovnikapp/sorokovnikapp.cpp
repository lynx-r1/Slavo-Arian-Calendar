#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QSettings>

#include <QSqlQuery>
#include <QSqlError>

#include "sadefines.h"
#include "sorokovnikden.h"
#include "sorokovnik.h"

#include "sorokovnikappsettingsdialog.h"

#include "sorokovnikapp.h"

SorokovnikApp::SorokovnikApp(QWidget *parent)
    : QMainWindow(parent)
    , mZametkiDBName("zametki")
    , mSorokovnik(0)
    , mDen(-1)
    , mSorok(-1)
    , mLeto(-1)
{
    setupUi(this);
    setWindowIcon(QIcon(":/SAKalendar/SorokovnikApp/images/icons/sorokovnik.png"));

    QFont saFont(SAFONTFAMILYIZHITSAC, 12);
    mPrazdnikiTextEdit->setFont(saFont);
    mZametkiTextEdit->setFont(saFont);

    readSettings();
    createSorokovnik();
    populateZametkamiLeto(mSorokovnik->leto());
    setupConnections();
}

// ---------------------------- protected ---------------------------------- //
void SorokovnikApp::closeEvent(QCloseEvent *e)
{
    writeSettings();
    e->accept();
}

// --------------------------- private slots ------------------------------- //
void SorokovnikApp::showAbout()
{
    QString about = tr("<b>Сороковник (Версия %1)</b><br>"
                       "Программа наглядно представляет Славяно-Арийский сороковник "
                       "(месяц), а так же содержит информацию о "
                       "Славяно-Арийских праздниках и предоставляет возможность "
                       "делать заметки по дням.<br><br>"
                       "Программа написана на Qt %2.<br><br>"
                       "<b>Разработчик</b><br>"
                       "<span style='white-space:pre'>    Рысь (lynx_r)</span>")
            .arg(QCoreApplication::applicationVersion()).arg(qVersion());

    QMessageBox::about(this, tr("О программе Сороковник"), about);
}

void SorokovnikApp::daySelected(int den, int sorokovnik, int leto)
{
    bool prazdnikiDbOpened = true;
    QSqlDatabase prazdnikiDb = QSqlDatabase::database(mSorokovnik->dbPrazdnikiConnectionName());
    if (!prazdnikiDb.isOpen()) {
        prazdnikiDbOpened = false;
    }

    QSqlQuery q(mZametkiDB);
    if (mZametkiTextEdit->document()->isModified()
        && mDen != -1 && mSorok != -1 && mLeto > 0) {
        q.prepare("SELECT zametka FROM zametki WHERE den=? AND sorokovnik=? AND leto=?");
        q.addBindValue(mDen);
        q.addBindValue(mSorok);
        q.addBindValue(mLeto);
        q.exec();
        q.next();

        if (q.isValid()) {
            q.clear();
            q.prepare("UPDATE zametki SET zametka=? WHERE den=? AND sorokovnik=? AND leto=?");
            if (mZametkiTextEdit->document()->isEmpty())
                q.addBindValue(QString());
            else
                q.addBindValue(mZametkiTextEdit->toHtml());
            q.addBindValue(mDen);
            q.addBindValue(mSorok);
            q.addBindValue(mLeto);
            if (!q.exec())
                qWarning() << q.lastError();

            mSorokovnik->den(mDen, mSorok)->setPomechen(!mZametkiTextEdit->document()->isEmpty());
        } else {
            q.clear();
            q.prepare("INSERT INTO zametki VALUES(?, ?, ?, ?)");
            q.addBindValue(mDen);
            q.addBindValue(mSorok);
            q.addBindValue(mLeto);
            q.addBindValue(mZametkiTextEdit->toHtml());
            if (!q.exec())
                qWarning() << q.lastError();

            mSorokovnik->den(mDen, mSorok)->setPomechen(true);
        }

        mZametkiTextEdit->document()->setModified(false);
    }

    q.clear();
    q.prepare("SELECT zametka FROM zametki WHERE den=? AND sorokovnik=? AND leto=?");
    q.addBindValue(den);
    q.addBindValue(sorokovnik);
    q.addBindValue(leto);
    q.exec();
    q.next();

    if (q.isValid()) {
        mZametkiTextEdit->setHtml(q.value(0).toString());
    } else {
        mZametkiTextEdit->clear();
    }

    if (mZametkiTextEdit->document()->isEmpty() && den != -1) {
        QString footer(tr("<br><p align='center'><s style='white-space:pre'>          </s>"
                          "%1:%2 %3 %4 %5 лето от С.М.З.Х.<s style='white-space:pre'>         </s></p>")
                       .arg(mSorokovnik->chasText()).arg(mSorokovnik->chastText()).arg(den)
                       .arg(SAMesyaci_Opisanie[sorokovnik - 1].first).arg(leto));

        mZametkiTextEdit->setHtml(footer);
    }

    mZametkiTextEdit->setFocus();
    mZametkiTextEdit->moveCursor(QTextCursor::End);
    mZametkiTextEdit->moveCursor(QTextCursor::Up);
    mZametkiTextEdit->moveCursor(QTextCursor::Up);
    mZametkiTextEdit->moveCursor(QTextCursor::EndOfLine);

    mDen = den;
    mSorok = sorokovnik;
    mLeto = leto;
}

void SorokovnikApp::populateZametkamiLeto(int leto)
{
    mZametkiDB = QSqlDatabase::addDatabase("QSQLITE", QString("%1_%2.db").arg(mZametkiDBName).arg(leto));
#if defined(Q_OS_UNIX)
    mZametkiDB.setDatabaseName(QString("../share/%1_%2.db").arg(mZametkiDBName).arg(leto));
#elif defined(Q_OS_WIN)
    mZametkiDB.setDatabaseName(QString("databases/%1_%2.db").arg(mZametkiDBName).arg(leto));
#endif

    if (!mZametkiDB.open()) {
        qWarning() << tr("Не удалось открыть базу данных") << mZametkiDB.lastError();
        return;
    }

    QStringList tables = mZametkiDB.tables();
    if (tables.isEmpty()) {
        QSqlQuery q(mZametkiDB);
        if (!q.exec("CREATE TABLE zametki (den integer, sorokovnik integer, leto integer, zametka text)"))
            qWarning() << q.lastError();
    }

    QString zametka;
    QSqlQuery q(mZametkiDB);
    for (int i = 1; i <= SACOLMESYACEV; ++i) {
        int s = i % 2 ? 41 : 40;
        for (int j = 1; j <= s; ++j) {
            q.clear();
            q.prepare("SELECT zametka FROM zametki WHERE den=? AND sorokovnik=? AND leto=?");
            q.addBindValue(j);
            q.addBindValue(i);
            q.addBindValue(leto);
            q.exec();
            q.next();

            if (q.isValid()) {
                zametka = q.value(0).toString();
                mSorokovnik->den(j, i)->setPomechen(!zametka.isEmpty());
            }
        }
    }
}

void SorokovnikApp::showPrazdnikInfo(bool show, const QString &info)
{
    if (show) {
        mPrazdnikiTextEdit->setText(info);
    } else {
        mPrazdnikiTextEdit->clear();
    }
}

// ------------------------------- private --------------------------------- //
void SorokovnikApp::readSettings()
{
    QSettings s;
    resize(s.value("/SAKalendar/SorokovnikApp/size").toSize());
    move(s.value("/SAKalendar/SorokovnikApp/pos").toPoint());
    mTabWidget->setCurrentIndex(s.value("/SAKalendar/SorokovnikApp/tab").toInt());
}

void SorokovnikApp::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SorokovnikApp/size", size());
    s.setValue("/SAKalendar/SorokovnikApp/pos", pos());
    s.setValue("/SAKalendar/SorokovnikApp/tab", mTabWidget->currentIndex());
}

void SorokovnikApp::createSorokovnik()
{
    QSettings s;
    QColor conturColor = s.value("/SAKalendar/SorokovnikApp/ConturColor",
                                 QColor(100, 200, 100, 127)).value<QColor>();
    QColor shadowColor = s.value("/SAKalendar/SorokovnikApp/ShadowColor",
                                 QColor(Qt::blue)).value<QColor>();

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setColor(shadowColor);
    shadow->setOffset(2);
    shadow->setBlurRadius(12);

    mSorokovnik = new Sorokovnik;
    mSorokovnik->setSwitchMesyacEasingCurve(QEasingCurve::OutExpo);
    mSorokovnik->setSwitchSorokovnikEasingCurve(QEasingCurve::OutExpo);
    mSorokovnik->setSwitchMesyacDuration(3000);
    mSorokovnik->setSwitchSorokovnikDuration(3000);
    mSorokovnik->setConturColor(conturColor);
    mSorokovnik->setGraphicsEffect(shadow);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->addItem(mSorokovnik);

    QPalette p = mSorokovnikGraphicsView->viewport()->palette();
    p.setColor(QPalette::Base, Qt::transparent);
    mSorokovnikGraphicsView->viewport()->setPalette(p);
    mSorokovnikGraphicsView->setScene(scene);
    mSorokovnikGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mSorokovnikGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mSorokovnikGraphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
#if defined(Q_OS_UNIX)
    mSorokovnikGraphicsView->setMinimumSize(432, 418);
#elif defined(Q_OS_WIN)
    mSorokovnikGraphicsView->setMinimumSize(442, 428);
#endif

    mSettingsDialog = new SorokovnikAppSettingsDialog(mSorokovnik, shadow, this);
}

void SorokovnikApp::setupConnections()
{
    connect(mSorokovnik, SIGNAL(selected(int,int,int)), this, SLOT(daySelected(int,int,int)));
    connect(mSorokovnik, SIGNAL(showInfoRequest(bool,QString)), this, SLOT(showPrazdnikInfo(bool,QString)));
    connect(mSorokovnik, SIGNAL(letoChanged(int)), this, SLOT(populateZametkamiLeto(int)));

    connect(mSettingsAction, SIGNAL(triggered()), mSettingsDialog, SLOT(show()));
    connect(mQuitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(mAboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
}
