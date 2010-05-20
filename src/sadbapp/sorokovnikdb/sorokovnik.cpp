#include <QDebug>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStyleOptionGraphicsItem>
#include <QTimer>

#include "sadb.h"
#include "dbgraphicspushbutton.h"
#include "dbgraphicsbuttongroup.h"
#include "dbgraphicslabelitem.h"
#include "dbgraphicsstackedlayout.h"

#include "sorokovnikden.h"

#include "sorokovnik.h"

const QSize DenItemSize = QSize(45, 30);

Sorokovnik::Sorokovnik(const QString &dbConnectionName, int leto, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mShemaLeto(new QGraphicsLinearLayout)
    , mStackMesyac(new DBGraphicsStackedLayout)
    , mDbConnectionName(dbConnectionName)
    , mLeto(leto)
{
    setAcceptHoverEvents(true);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../share/prazdniki.db");

    if (!db.open()) {
        qWarning() << tr("Не удалось открыть базу данных праздников в ../share:") << db.lastError();
        db.setDatabaseName("share/prazdniki.db");
        if (!db.open()) {
            qWarning() << tr("Не удалось открыть базу данных праздников в ./share:") << db.lastError();
            db.setDatabaseName("prazdniki.db");
            if (!db.open())
                qWarning() << tr("Не удалось открыть базу данных праздников в ./:") << db.lastError();
        }
    }

    recreateSorokovnikButtonGroup();

    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    l->addItem(shemaSorokovnika());
    l->setContentsMargins(0, 0, 0, 0);
    setLayout(l);

    mSorokovnik = 1;
    mStackMesyac->setSilentSwitch(true);
    mStackMesyac->setCurrentIndex(mSorokovnik - 1);
    mStackMesyac->setSilentSwitch(false);

    QGraphicsWidget *w = dynamic_cast<QGraphicsWidget *>(mShemaLeto->itemAt(0));
    mStackSorokovnik = dynamic_cast<DBGraphicsStackedLayout *>(w->layout()->itemAt(0));
    mStackSorokovnik->setSilentSwitch(true);
    mStackSorokovnik->setCurrentIndex(mSorokovnik - 1);
    mStackSorokovnik->setSilentSwitch(false);

    mStackMesyac->setSwitchEasingCurve(QEasingCurve::OutExpo);
    mStackMesyac->setSwitchDuration(1000);
}

Sorokovnik::~Sorokovnik()
{
    delete mStackMesyac;
    delete mStackSorokovnik;
}

// ------------------------------- public ---------------------------------- //
QRectF Sorokovnik::boundingRect() const
{
    return layout()->geometry().adjusted(-10, -10, 10, 10);
}

void Sorokovnik::setLeto(int leto)
{
    pereitiNaLeto(leto);
}

void Sorokovnik::updateCheckedDen(const QString &prazdnik, const QString &opisanie, int rang)
{
    setInfoForDay(mSorokovnik, mSorokovnikButtonGroupList.at(mSorokovnik - 1)->checkedId(),
                  prazdnik, opisanie, rang);
}

// ------------------------------ public slots ----------------------------- //
void Sorokovnik::sledSorokovnik()
{
    int index = mStackSorokovnik->currentIndex() + 1;
    index = index > mStackSorokovnik->count() - 1 ? 0 : index;

    mStackMesyac->setSwitchDirection(DBGraphicsStackedLayout::Right2Left);
    mStackSorokovnik->setSwitchDirection(DBGraphicsStackedLayout::Right2Left);

    if (mStackSorokovnik->setCurrentIndex(index)
        && mStackMesyac->setCurrentIndex(index)) {
        mSorokovnik++;
        if (mSorokovnik > 9)
            mSorokovnik = 1;
    }
}

void Sorokovnik::predSorokovnik()
{
    int index = mStackSorokovnik->currentIndex() - 1;
    index = index < 0 ? mStackSorokovnik->count() - 1 : index;

    mStackMesyac->setSwitchDirection(DBGraphicsStackedLayout::Left2Right);
    mStackSorokovnik->setSwitchDirection(DBGraphicsStackedLayout::Left2Right);

    if (mStackSorokovnik->setCurrentIndex(index)
        && mStackMesyac->setCurrentIndex(index)) {
        mSorokovnik--;
        if (mSorokovnik == 0)
            mSorokovnik = 9;
    }
}

void Sorokovnik::populatePrazdn()
{
    for (int i = 0; i < mSorokovnikButtonGroupList.count(); ++i) {
        QList<DBGraphicsPushButton *> buttons = mSorokovnikButtonGroupList.at(i)->buttons();
        for (int j = 0; j < buttons.count(); ++j) {
            QString prazdnik;
            QString opisanie;
            int rang;
            if (getInfoFromDB(i + 1, j + 1, prazdnik, opisanie, rang))
                setInfoForDay(i + 1, j + 1, prazdnik, opisanie, rang);
        }
    }
}

// ----------------------------- private slots ----------------------------- //
void Sorokovnik::buttonClicked(int den)
{
    emit selected(den, mSorokovnik);
}

// -------------------------------- private -------------------------------- //
QGraphicsWidget *Sorokovnik::sozdatSorokovnik(int nachRamhata)
{
    DBGraphicsStackedLayout *stack = new DBGraphicsStackedLayout;
    stack->setSwitchEasingCurve(QEasingCurve::OutExpo);
    stack->setSwitchDuration(1000);

    for (int s = 0; s < DBCOLMESYACEV; ++s) {
        QGraphicsWidget *storona = new QGraphicsWidget;
        storona->setZValue(101 + s);

        QGraphicsGridLayout *shema = new QGraphicsGridLayout;
        shema->setContentsMargins(0, 0, 0, 0);
        shema->setSpacing(3.0);

        QGraphicsWidget *den;
        int dni = 1;
        int chasti = 6;
        int m = (s + 1) % 2 ? nachRamhata : nachRamhata - 4;
        m = m <= 0 ? m + 9 : m;
        int dneiVSorok = (s + 1) % 2 ? DBCOLDNEYIVPOLNOMMESYACE + m : DBCOLDNEYIVNEPOLNOMMESYACE + m;
        for (int j = 0; j < chasti; ++j) {
            for (int i = 0; i < DBCOLDNEYINEDELI; ++i) {
                if (dni >= m && dni < (dneiVSorok)) {
                    den = dobavitDen(s, dni - m + 1, i);
                    shema->addItem(den, i, j);
                } else {
                    DBGraphicsLabelItem *rect = new DBGraphicsLabelItem(
                            "", font().family(), Qt::AlignCenter,
                            DBCvetaHTMLCveta[i].second, true);

                    shema->addItem(rect, i, j);
                    rect->setMaximumSize(DenItemSize);
                }

                dni++;
            }
        }

        storona->setLayout(shema);
        stack->addItem(storona);
    }

    QGraphicsWidget *sorokovnik = new QGraphicsWidget;
    sorokovnik->setZValue(200);
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(sorokovnik);
    l->setContentsMargins(0, 0, 0, 0);
    l->addItem(stack);

    return sorokovnik;
}

void Sorokovnik::pereitiNaLeto(int leto)
{
    const int duration = 1000;

    recreateSorokovnikButtonGroup();

    // удаляем предыдущий сороковник из схемы
    QGraphicsWidget *widgetStack = dynamic_cast<QGraphicsWidget *>(mShemaLeto->itemAt(0));
    mShemaLeto->removeAt(0);

    // анимируем исчезновение предыдущего сороковника
    QPropertyAnimation *anim2 = new QPropertyAnimation(widgetStack, "opacity");
    anim2->setDuration(duration);
    anim2->setStartValue(1);
    anim2->setEndValue(0);
    anim2->start(QAbstractAnimation::DeleteWhenStopped);

    // удаляем предыдущий сороковник из памяти
    QTimer::singleShot(1000, widgetStack, SLOT(deleteLater()));
    QTimer::singleShot(2000, widgetStack, SLOT(deleteLater())); // на всякий случай

    // добавляем сороковник в схему
    QGraphicsWidget *sorokovnik = sozdatSorokovnik(nachaloRamkhata(leto));
    mStackSorokovnik = dynamic_cast<DBGraphicsStackedLayout *>(sorokovnik->layout()->itemAt(0));
    mStackSorokovnik->setSilentSwitch(true);
    mStackSorokovnik->setCurrentIndex(mSorokovnik - 1);
    mStackSorokovnik->setSilentSwitch(false);

    mShemaLeto->addItem(sorokovnik);

    // анимируем появление нового сороковника
    QPropertyAnimation *anim = new QPropertyAnimation(sorokovnik, "opacity");
    anim->setDuration(duration);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void Sorokovnik::recreateSorokovnikButtonGroup()
{
    qDeleteAll(mSorokovnikButtonGroupList);
    mSorokovnikButtonGroupList.clear();
    for (int i = 0; i < DBCOLMESYACEV; ++i) {
        DBGraphicsButtonGroup *gr = new DBGraphicsButtonGroup(this);
        gr->setExclusive(true);
        connect(gr, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));
        mSorokovnikButtonGroupList.append(gr);
    }
}

bool Sorokovnik::getInfoFromDB(int sorokovnik, int den, QString &title, QString &description, int &rang)
{
    QString table;
    switch (sorokovnik)
    {
    case 1:
        table = QString("ramkhato");
        break;
    case 2:
        table = QString("aileto");
        break;
    case 3:
        table = QString("beileto");
        break;
    case 4:
        table = QString("geileto");
        break;
    case 5:
        table = QString("daileto");
        break;
    case 6:
        table = QString("eileto");
        break;
    case 7:
        table = QString("veileto");
        break;
    case 8:
        table = QString("heileto");
        break;
    case 9:
        table = QString("taileto");
        break;
    default:
        table = QString();
        break;
    }

    QSqlDatabase db = QSqlDatabase::database(mDbConnectionName);
    if (db.isOpen()) {
        QSqlQuery q(db);
        q.prepare(QString("SELECT count FROM %1 WHERE id=?").arg(table));
        q.addBindValue(den);
        q.exec();
        q.next();

        int holidId = q.value(0).toInt();
        if (holidId != 0) {
            q.prepare("SELECT name, descr, rang FROM slavo_arian_holid WHERE id=?");
            q.addBindValue(holidId);
            q.exec();
            q.next();
            title = q.value(0).toString();
            description = q.value(1).toString();
            rang = q.value(2).toInt();
        } else {
            title = "";
            description = "";
            rang = 0;
        }
        return true;
    } else {
        qWarning() << tr("База данных не открыта");
        return false;
    }
}

void Sorokovnik::setInfoForDay(int sorokovnik, int den, const QString &prazdnik,
                           const QString &opisanie, int rang)
{
    if (den == -1 || sorokovnik == 0)
        return;

    DBGraphicsPushButton *b = mSorokovnikButtonGroupList.at(sorokovnik - 1)->button(den);
    SorokovnikDen *denButton = qobject_cast<SorokovnikDen *>(b);
    if (denButton) {
        if (!prazdnik.isNull())
            denButton->setPrazdnik(prazdnik);
        if (!opisanie.isNull())
            denButton->setOpisaniePrazdnika(opisanie);

        denButton->resetOriginCvet();

        denButton->setVeshchiiSon(rang & 1);
        denButton->setRoditelskiiDen(rang & 2);
        denButton->setDenPochitaniyaPredkov(rang & 4);
        denButton->setVazhnyiPrazdnik(rang & 8);

        denButton->setSvyatyiPost(rang & 16);
        denButton->setDazhdbozhyiPost(rang & 32);
        denButton->setChistyiPost(rang & 64);
        denButton->setVelikiiPost(rang & 128);
        denButton->setSvetlyiPost(rang & 256);

        denButton->update();
    }
}

QGraphicsLayout *Sorokovnik::shemaSorokovnika()
{
    QGraphicsGridLayout *shemaSorokovn = new QGraphicsGridLayout;
    shemaSorokovn->setSpacing(3.0);

    // подписываем полный месяц
    for (int i = 0; i < DBCOLMESYACEV; ++i) {
        DBGraphicsLabelItem *label = new DBGraphicsLabelItem(DBMesyaci[i], font().family(),
                                                             Qt::AlignCenter,
                                                             DBCvetaHTMLCveta[i].second,
                                                             true);
        label->setZValue(50 + i);
        label->setFixedHeight(DenItemSize.height());
        label->setToolTip(tr("Нечётный (полный) месяц"));

        mStackMesyac->addItem(label);
    }

    shemaSorokovn->addItem(mStackMesyac, 0, 0, 1, 1);

    // рисуем дни недели
    int col = shemaSorokovn->columnCount();
    for (int i = 1; i < DBCOLDNEYINEDELI + 1; ++i) {
        DBGraphicsLabelItem *label = new DBGraphicsLabelItem(DBDniNedeli[i - 1], font().family(), Qt::AlignCenter,
                                                             DBCvetaHTMLCveta[i - 1].second, true);
        shemaSorokovn->addItem(label, i, col);

        label->setFixedHeight(DenItemSize.height());
    }

    // подписываем дни недели
    DBGraphicsLabelItem *label = new DBGraphicsLabelItem(tr("Дни недели"), font().family(),
                                                         Qt::AlignCenter, "#99ff99", true);
    shemaSorokovn->addItem(label, 0, col);
    label->setMinimumWidth(91); // магическое число 91 :)
    label->setFixedHeight(DenItemSize.height());

    mShemaLeto->addItem(sozdatSorokovnik(nachaloRamkhata(mLeto)));
    shemaSorokovn->addItem(mShemaLeto, 1, 0, 8, 1);

    return shemaSorokovn;
}

QGraphicsWidget *Sorokovnik::dobavitDen(int sorokovnik, int nomer, int cvet, bool nepolnMes)
{
    SorokovnikDen *den = new SorokovnikDen(QString::number(nomer), "test",
                                           DBCvetaHTMLCveta[cvet].second, 0, 0);
    den->setFixedSize(DenItemSize);

    den->setPrazdnik("");
    den->setOpisaniePrazdnika("");

    den->setVeshchiiSon(0);
    den->setRoditelskiiDen(0);
    den->setDenPochitaniyaPredkov(0);
    den->setVazhnyiPrazdnik(0);

    den->setSvyatyiPost(0);
    den->setDazhdbozhyiPost(0);
    den->setChistyiPost(0);
    den->setVelikiiPost(0);
    den->setSvetlyiPost(0);

    mSorokovnikButtonGroupList.at(sorokovnik)->addButton(den, nepolnMes ? nomer + 41 : nomer);

    return den;
}

int Sorokovnik::vybranDen()
{
    int vybranDen = mSorokovnikButtonGroupList.at(mSorokovnik - 1)->checkedId();
    if (vybranDen == -1)
        return 1;
    return vybranDen;
}

int Sorokovnik::nachaloRamkhata(int leto)
{
    int kzh = (leto + 112) % 144;
    kzh = kzh ? kzh : 144;
    for (int i = 0; i < DBCOLDNEYINEDELI; ++i) {
        if (DBDenNedeli_Novoletie[i].second.contains(kzh)) {
            return i + 1;
        }
    }
    return -1;
}
