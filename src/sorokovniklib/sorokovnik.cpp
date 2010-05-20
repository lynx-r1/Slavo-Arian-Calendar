#include <QDebug>
#include <QDialogButtonBox>

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneHoverEvent>

#include <QMessageBox>

#include <QPainter>
#include <QPropertyAnimation>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QStyleOptionGraphicsItem>
#include <QTimer>

#include "sadefines.h"
#include "sagraphicspushbutton.h"
#include "sagraphicsbuttongroup.h"
#include "sagraphicslabelitem.h"

#include "sadate_cpp.h"
#include "satime_cpp.h"
#include "satimer_cpp.h"

#include "sorokovnikden.h"
#include "sorokovniklineedit.h"

#include "sorokovnik.h"

const QSize DenItemSize = QSize(45, 30);


// ************************************************************************* //

SorokovnikBase::SorokovnikBase(QGraphicsItem *parent, bool deflt)
    : QGraphicsWidget(parent)
    , mShemaLeto(new QGraphicsLinearLayout)
    , mSwitchSorokovnikEasingCurve(QEasingCurve::OutExpo)
    , mStackMesyac(new SAGraphicsStackedLayout)
    , mStackSorokovnik(0)
    , mDenPrototype(0)
    , mMesyacNextDirection(SAGraphicsStackedLayout::Right2Left)
    , mMesyacPrevDirection(SAGraphicsStackedLayout::Left2Right)
    , mSorokovnikNextDirection(SAGraphicsStackedLayout::Right2Left)
    , mSorokovnikPrevDirection(SAGraphicsStackedLayout::Left2Right)
    , mSwitchSorokovnikDuration(3000)
    , mDen(-1)
    , mLeto(0)
    , mKZhizni(0)
    , mKLet(0)
    , mSorokovnik(1)
    , mSilentSwitchSorokovnik(false)
    , mBlockSorokovnikSwitch(false)
{
    setAcceptHoverEvents(true);
    recreateSorokovnikButtonGroup();

    if (deflt)
        createSorokovnik();
}

SorokovnikBase::~SorokovnikBase()
{
    qDeleteAll(mSorokovnikButtonGroupList);
    delete mStackMesyac;
    delete mStackSorokovnik;
}

void SorokovnikBase::setDen(int theDen) {
    mDen = theDen;
    if (SorokovnikDen *d = den(theDen, mSorokovnik))
        d->setChecked(true);
}

void SorokovnikBase::setLeto(int leto, bool silent)
{
    if (leto != mLeto) {
        mLeto = leto;
        mKZhizni = SADate::lvkzhizni(leto);
        mKLet = SADate::lvklet(leto);

        pereitiNaLeto(leto, silent);
    }
}

int SorokovnikBase::checkedDenId() const
{
    return mSorokovnikButtonGroupList.at(mSorokovnik - 1)->checkedId();
}

SorokovnikDen *SorokovnikBase::checkedDen() const
{
    return qobject_cast<SorokovnikDen *>(
            mSorokovnikButtonGroupList.at(mSorokovnik - 1)->checkedButton());
}

SorokovnikDen *SorokovnikBase::den(int den, int sorokovnik) const
{
    return qobject_cast<SorokovnikDen *>(
            mSorokovnikButtonGroupList.at(sorokovnik - 1)->button(den));
}

void SorokovnikBase::createSorokovnik()
{
    setLayout(shemaSorokovnika());
    silentSwitchToSorokovnik(mSorokovnik - 1);
}

void SorokovnikBase::deleteSorokovnik()
{
    QGraphicsGridLayout *gl = static_cast<QGraphicsGridLayout *>(layout());
    QGraphicsWidget *w = static_cast<QGraphicsWidget *>(mShemaLeto->itemAt(0));

    delete w;
    delete mShemaLeto;
    delete mStackMesyac;
    qDeleteAll(mSorokovnikButtonGroupList);

    mShemaLeto = 0, mStackMesyac = mStackSorokovnik = 0;

    while (gl->count() > 0) {
        QGraphicsLayoutItem *li = gl->itemAt(0);
        delete li->graphicsItem();
    }

    setLayout(0);
}

// --------------------------- public slots -------------------------------- //
void SorokovnikBase::sledLeto()
{
    if (mShemaLeto) {
        ++mLeto;

        mKZhizni = SADate::lvkzhizni(mLeto);
        mKLet = SADate::lvklet(mLeto);

        pereitiNaLeto(mLeto, false);
    }
}

void SorokovnikBase::predLeto()
{
    if (mShemaLeto) {
        --mLeto;

        mKZhizni = SADate::lvkzhizni(mLeto);
        mKLet = SADate::lvklet(mLeto);

        pereitiNaLeto(mLeto, false);
    }
}

void SorokovnikBase::sledSorokovnik()
{
    if (mBlockSorokovnikSwitch)
        return;

    int index = mStackSorokovnik->currentIndex() + 1;
    index = index > mStackSorokovnik->count() - 1 ? 0 : index;
    mDen = checkedDenId();

    pereitiNaSorokovnik(index, true);
}

void SorokovnikBase::predSorokovnik()
{
    if (mBlockSorokovnikSwitch)
        return;

    int index = mStackSorokovnik->currentIndex() - 1;
    index = index < 0 ? mStackSorokovnik->count() - 1 : index;
    mDen = checkedDenId();

    pereitiNaSorokovnik(index, false);
}

// -------------------------- private slots -------------------------------- //
void SorokovnikBase::denClicked(int den)
{
    mDen = den;
    emit selected(den, mSorokovnik, mLeto);
}

void SorokovnikBase::stackedLayoutIndexChanged(int index)
{
    mBlockSorokovnikSwitch = index == -1;
}

// --------------------------- protected ----------------------------------- //
QGraphicsWidget *SorokovnikBase::dobavitDen(int sorokovnik, int nomer, int cvet,
                                            bool nepolnMes)
{
    SorokovnikDen *den = createDen(QString::number(nomer), "", SACvetaHTMLCveta[cvet].second);
    if (!den)
        return 0;
    den->setFixedSize(DenItemSize);
    mSorokovnikButtonGroupList.at(sorokovnik)->addButton(den, nepolnMes ? nomer + 41 : nomer);

    return den;
}

QGraphicsWidget *SorokovnikBase::sozdatSorokovnik(int nachRamhata)
{
    SAGraphicsStackedLayout *stack = new SAGraphicsStackedLayout;
    stack->setContentsMargins(0, 0, 0, 0);

    for (int s = 0; s < SACOLMESYACEV; ++s) {
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
        int dneiVSorok = (s + 1) % 2 ? SACOLDNEYIVPOLNOMMESYACE + m : SACOLDNEYIVNEPOLNOMMESYACE + m;
        for (int j = 0; j < chasti; ++j) {
            for (int i = 0; i < SACOLDNEYINEDELI; ++i) {
                if (dni >= m && dni < (dneiVSorok)) {
                    den = dobavitDen(s, dni - m + 1, i);
                    shema->addItem(den, i, j);
                } else {
                    SAGraphicsLabelItem *rect = new SAGraphicsLabelItem(
                            "", QFont(), Qt::AlignCenter,
                            SACvetaHTMLCveta[i].second, true);

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
    sorokovnik->setLayout(stack);

    return sorokovnik;
}

QGraphicsLayout *SorokovnikBase::shemaSorokovnika()
{
    QFont saFont(SAFONTFAMILYIZHITSAC, 12);

    QGraphicsGridLayout *shemaSorokovn = new QGraphicsGridLayout;
    shemaSorokovn->setSpacing(3.0);

    // подписываем полный месяц
    for (int i = 0; i < SACOLMESYACEV; ++i) {
        SAGraphicsLabelItem *label = new SAGraphicsLabelItem(SAMesyaci_Opisanie[i].first, saFont,
                                                             Qt::AlignCenter,
                                                             SACvetaHTMLCveta[i].second,
                                                             true);
        label->setZValue(50 + i);
        label->setFixedHeight(DenItemSize.height());
        label->setToolTip(SAMesyaci_Opisanie[i].second);

        mStackMesyac->addItem(label);
    }

    shemaSorokovn->addItem(mStackMesyac, 0, 0, 1, 1);

    // рисуем дни недели
    int col = shemaSorokovn->columnCount();
    for (int i = 1; i < SACOLDNEYINEDELI + 1; ++i) {
        SAGraphicsLabelItem *label = new SAGraphicsLabelItem(SADniNedeli_Opisanie[i - 1].first,
                                                             saFont,
                                                             Qt::AlignCenter,
                                                             SACvetaHTMLCveta[i - 1].second,
                                                             true);
        label->setToolTip(SADniNedeli_Opisanie[i - 1].second);
        shemaSorokovn->addItem(label, i, col, Qt::AlignHorizontal_Mask);
        label->setFixedHeight(DenItemSize.height());
    }

    // подписываем дни недели
    SAGraphicsLabelItem *label = new SAGraphicsLabelItem(tr("Дни недели"),
                                                         saFont,
                                                         Qt::AlignCenter,
                                                         "#99ff99", true);
    shemaSorokovn->addItem(label, 0, col, Qt::AlignHorizontal_Mask);
    label->setMinimumWidth(100);
    label->setFixedHeight(DenItemSize.height());

    QGraphicsWidget *sorokovnik = setupStackSorokovnik(SADate::currentDate().leto());
    mShemaLeto->addItem(sorokovnik);
    shemaSorokovn->addItem(mShemaLeto, 1, 0, 8, 1);

    return shemaSorokovn;
}

SorokovnikDen *SorokovnikBase::createDen(const QString &name, const QString & toolTip,
                                           const QColor &color)
{
    return mDenPrototype ? mDenPrototype->clone(name, toolTip, color)
        : new SorokovnikDen(name, toolTip, color);
}

void SorokovnikBase::silentSwitchToSorokovnik(int sorokovnik)
{
    if (mStackMesyac) {
        mStackMesyac->setSilentSwitch(true);
        mStackMesyac->setCurrentIndex(sorokovnik);
        mStackMesyac->setSilentSwitch(false);
    }

    if (mStackSorokovnik) {
        mStackSorokovnik->setSilentSwitch(true);
        mStackSorokovnik->setCurrentIndex(sorokovnik);
        mStackSorokovnik->setSilentSwitch(false);
    }

    update();
}

// ---------------------------- private ------------------------------------ //
void SorokovnikBase::recreateSorokovnikButtonGroup()
{
    qDeleteAll(mSorokovnikButtonGroupList);
    mSorokovnikButtonGroupList.clear();
    for (int i = 0; i < SACOLMESYACEV; ++i) {
        SAGraphicsButtonGroup *gr = new SAGraphicsButtonGroup(this);
        gr->setExclusive(true);
        connect(gr, SIGNAL(buttonClicked(int)), this, SLOT(denClicked(int)));
        mSorokovnikButtonGroupList.append(gr);
    }
}

void SorokovnikBase::pereitiNaLeto(int leto, bool silent)
{
    if (!mShemaLeto)
        return;

    const int duration = 1000;

    recreateSorokovnikButtonGroup();

    // удаляем предыдущий сороковник из схемы
    QGraphicsWidget *widgetStack = static_cast<QGraphicsWidget *>(mShemaLeto->itemAt(0));
    mShemaLeto->removeAt(0);

    if (!silent) {
        // анимируем исчезновение предыдущего сороковника
        QPropertyAnimation *anim2 = new QPropertyAnimation(widgetStack, "opacity");
        anim2->setDuration(duration);
        anim2->setStartValue(1);
        anim2->setEndValue(0);
        anim2->start(QAbstractAnimation::DeleteWhenStopped);

        // удаляем предыдущий сороковник из памяти
        QTimer::singleShot(duration, widgetStack, SLOT(deleteLater()));
        QTimer::singleShot(duration + 1000, widgetStack, SLOT(deleteLater())); // на всякий случай
    } else {
        delete widgetStack;
    }

    // добавляем сороковник в схему
    QGraphicsWidget *sorokovnik = setupStackSorokovnik(leto);
    mShemaLeto->addItem(sorokovnik);

    if (!silent) {
        // анимируем появление нового сороковника
        QPropertyAnimation *anim = new QPropertyAnimation(sorokovnik, "opacity");
        anim->setDuration(duration);
        anim->setStartValue(0);
        anim->setEndValue(1);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    emit letoChanged(mLeto);
}

void SorokovnikBase::pereitiNaSorokovnik(int index, bool next)
{
    mStackMesyac->setSwitchDirection(next ? mMesyacNextDirection : mMesyacPrevDirection);
    mStackSorokovnik->setSwitchDirection(next ? mSorokovnikNextDirection : mSorokovnikPrevDirection);

    mStackSorokovnik->setCurrentIndex(index);
    mStackMesyac->setCurrentIndex(index);

    if (!mBlockSorokovnikSwitch) {
        mSorokovnik = index + 1;
        emit sorokovnikChanged(mSorokovnik);
    }
}

QGraphicsWidget *SorokovnikBase::setupStackSorokovnik(int leto)
{
    QGraphicsWidget *sorokovnik = sozdatSorokovnik(SADate::nachRamkhata(leto));
    mStackSorokovnik = static_cast<SAGraphicsStackedLayout *>(sorokovnik->layout());
    mStackSorokovnik->setSwitchEasingCurve(mSwitchSorokovnikEasingCurve);
    mStackSorokovnik->setSwitchDuration(mSwitchSorokovnikDuration);
    mStackSorokovnik->setSilentSwitch(mSilentSwitchSorokovnik);

    mStackSorokovnik->setSilentSwitch(true);
    mStackSorokovnik->setCurrentIndex(mSorokovnik - 1);
    mStackSorokovnik->setSilentSwitch(false);
    connect(mStackSorokovnik, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stackedLayoutIndexChanged(int)));

    return sorokovnik;
}

// ************************************************************************* //

// --------------------------------- public -------------------------------- //
SorokovnikDB::SorokovnikDB(QGraphicsItem *parent, bool deflt)
    : SorokovnikBase(parent, false)
    , mPrazdnikiDbConnectionName("")
    , mPrazdnikiDbName("")
{
    if (deflt)
        createSorokovnik();

    connect(this, SIGNAL(letoChanged(int)), this, SLOT(populatePrazdnikamiLeto(int)));
}

QSqlDatabase SorokovnikDB::openPrazdnikiDb(const QString &name, const QString &connectionName)
{
    if (!name.isEmpty()) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(name);

        QString errorMessage = tr("Не удалось открыть базу данных праздников, либо база данных пуста "
                                  "(для создания и заполнения базы данных праздников воспользуйтесь приложением <b>sadb</b>).<br>"
                                  "Описания праздников будут недоступны.");
        if (!db.open()) {
            qCritical() << tr("Не удалось открыть базу данных") << db.lastError();
            QMessageBox::critical(0, tr("Ошибка"), errorMessage);
            return QSqlDatabase();
        }

        if (db.tables().count() != 10) {
            qCritical() << tr("Число таблиц в базе данных меньше необходимого");
            QMessageBox::critical(0, tr("Ошибка"), errorMessage);
            return QSqlDatabase();
        }

        mPrazdnikiDbName = name;
        mPrazdnikiDbConnectionName = connectionName;
        qWarning() << tr("База данных %1 открыта").arg(name);
        return db;
    } else {
        qWarning() << tr("Введите имя файла базы данных");
        return QSqlDatabase();
    }
}

void SorokovnikDB::updateCheckedDen(const QString &prazdnik, const QString &opisanie, int rang)
{
    setInfoForDay(sorokovnik(), sorokovnikButtonGroupList().at(sorokovnik() - 1)->checkedId(),
                  prazdnik, opisanie, rang);
}

void SorokovnikDB::populatePrazdnikami()
{
    int snum = sorokovnikButtonGroupList().count();
    for (int i = 0; i < snum; ++i) {
        int dnum = sorokovnikButtonGroupList().at(i)->buttons().count();
        for (int j = 0; j < dnum; ++j) {
            QString prazdnik;
            QString opisanie;
            int rang;
            if (getInfoFromDB(i + 1, j + 1, prazdnik, opisanie, rang))
                setInfoForDay(i + 1, j + 1, prazdnik, opisanie, rang);
        }
    }
}

// ---------------------------- public slots ------------------------------- //
void SorokovnikDB::populatePrazdnikamiLeto(int leto)
{
    QSqlDatabase::removeDatabase(mPrazdnikiDbConnectionName);
#if defined(Q_OS_UNIX)
    mPrazdnikiDbName = QString("../share/%1_%2.db").arg(SADBPRAZDNIKINAME).arg(leto);
#elif defined(Q_OS_WIN)
    mPrazdnikiDbName = QString("databases/%1_%2.db").arg(SADBPRAZDNIKINAME).arg(leto);
#endif
    mPrazdnikiDbConnectionName = QString("%1_%2.db").arg(SADBPRAZDNIKINAME).arg(leto);

    QSqlDatabase db = openPrazdnikiDb(mPrazdnikiDbName, mPrazdnikiDbConnectionName);
    if (db.isOpen())
        populatePrazdnikami();
}

// ------------------------------ private ---------------------------------- //
bool SorokovnikDB::getInfoFromDB(int sorokovnik, int den, QString &title,
                                 QString &description, int &rang)
{
    if (mPrazdnikiDbConnectionName.isEmpty()) {
        qWarning() << tr("Не задано имя соединения с базой данных");
        title = "";
        description = "";
        rang = 0;

        return false;
    }

    QStringList tables = QStringList() << QString("ramkhato") << QString("aileto")
                     << QString("beileto") << QString("geileto") << QString("daileto")
                     << QString("eileto") << QString("veileto") << QString("heileto")
                     << QString("taileto");

    QSqlDatabase db = QSqlDatabase::database(mPrazdnikiDbConnectionName);
    if (db.isOpen()) {
        QSqlQuery q(db);
        q.prepare(QString("SELECT count FROM %1 WHERE id=?").arg(tables.at(sorokovnik - 1)));
        q.addBindValue(den);
        q.exec();
        q.next();

        if (q.isValid()) {
            int holidId = q.value(0).toInt();
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

void SorokovnikDB::setInfoForDay(int theSorokovnik, int theDen, const QString &prazdnik,
                                 const QString &opisanie, int rang)
{
    if (theDen == -1 || theSorokovnik == 0)
        return;

    if (SorokovnikDen *denButton = den(theDen, theSorokovnik)) {
        if (!prazdnik.isNull())
            denButton->setPrazdnik(prazdnik);
        if (!opisanie.isNull())
            denButton->setOpisaniePrazdnika(opisanie);

        if (!denButton->segodnya())
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

// ************************************************************************* //

// --------------------------------- public -------------------------------- //
Sorokovnik::Sorokovnik(QGraphicsItem *parent, bool deflt)
    : SorokovnikDB(parent, false)
    , mConturColor(QColor(100, 200, 100, 127))
    , mStackData(new SAGraphicsStackedLayout)
    , mSeichasSorokovnik(1)
    , mSeichasLeto(1)
    , mSegodnyaDen(1)
    , mDenNedeli(1)
    , mChas(1)
    , mChast(1)
    , mSvyashchennoeLeto(false)
{
    QDialogButtonBox::ButtonLayout layout =
            QDialogButtonBox::ButtonLayout(style()->styleHint(QStyle::SH_DialogButtonLayout));
    if (layout == QDialogButtonBox::GnomeLayout) {
        setContentsMargins(10, 0, 0, 0);
    } else {
        setContentsMargins(15, 5, 5, 5);
    }

    mStackData->setSwitchEasingCurve(QEasingCurve::OutExpo);
    mStackData->setSwitchDuration(1000);

    SATime time = SATime::currentTime();
    mTimer = new SATimer(this);
    mTimer->start(32, time.dolya(), time.chast(), time.chas());
    mChas = time.chas();
    mChast = time.chast();

    if (deflt)
        createSorokovnik();
}

Sorokovnik::~Sorokovnik()
{
    delete mLetoLabel;
    delete mStackData;
}

void Sorokovnik::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setClipRect(option->exposedRect);

    // рисуем закругленный квадрат вокруг календаря
    int lighter = 140;
    int darker = 120;
    int round = 8;
    QRectF r = option->rect;
    QLinearGradient grad(r.topLeft(), r.bottomLeft());
    grad.setColorAt(0, mConturColor.lighter(lighter));
    grad.setColorAt(1, mConturColor.darker(darker));
    painter->setPen(Qt::NoPen);
    painter->setBrush(grad);
    painter->drawRoundedRect(r, round, round);

    QLinearGradient lgr(r.topLeft(), r.bottomLeft());
    lgr.setColorAt(0, mConturColor.lighter(180));
    lgr.setColorAt(0.4, mConturColor.darker(115));
    lgr.setColorAt(0.6, mConturColor.darker(115));
    lgr.setColorAt(1, mConturColor.lighter(120));

    painter->setBrush(lgr);
    painter->drawRoundedRect(r.adjusted(2, 2, -2, -2), round, round);

    if (layout()) {
        QRectF r = layout()->itemAt(1)->geometry();

        // рисуем линию сороковника
        QPointF topLeft(r.topLeft() - QPoint(10, 10));
        QPointF topRight(r.topRight() - QPointF(0, 10));
        QPointF bottomLeft(r.bottomLeft() - QPointF(10, 0));

        QColor lineSorokColor(SACvetaHTMLCveta[sorokovnik() - 1].second);
        painter->setPen(QPen(QBrush(lineSorokColor), 2.0));
        painter->drawLine(topLeft, topRight);

        // рисуем линию дня
        QLinearGradient lgr(topLeft, bottomLeft);
        if (mDenNedeli == 9) {
            lgr.setColorAt(0, SACvetaHTMLCveta[mDenNedeli - 1].second);
            lgr.setColorAt(1, SACvetaHTMLCveta[0].second);
        } else {
            lgr.setColorAt(0, SACvetaHTMLCveta[mDenNedeli - 1].second);
            lgr.setColorAt(1, SACvetaHTMLCveta[mDenNedeli].second);
        }

        painter->setPen(QPen(QBrush(lgr), 2.0));
        painter->drawLine(topLeft, bottomLeft);

        // рисуем деление, указывающее выбранный день
        int dneyVSorok = sorokovnik() % 2 ? 41 : 40;
        qreal del = (topRight.x() - topLeft.x() - 15) / dneyVSorok;
        int d = den();
        d = d == -1 ? 1 : d;
        QPointF denDel1(del * d + 10, topLeft.y() + 3.0);
        QPointF denDel2(del * d + 17, topLeft.y() - 3.0);

        painter->setPen(lineSorokColor);
        painter->setBrush(lineSorokColor);
        painter->drawRoundedRect(QRectF(denDel1, denDel2), 1, 1);

        // рисуем деление, указывающее час
        del = r.height() / SACOLCHASOV;
        QPointF chastDel1(topLeft.x() - 3, r.y() + del * (mChas - 1));
        QPointF chastDel2(topLeft.x() + 3, r.y() + del * (mChas - 1) + 7);

        painter->setPen(QPen(QBrush(lgr), 1.0));
        painter->setBrush(lgr);
        painter->drawRoundedRect(QRectF(chastDel1, chastDel2), 1, 1);
    }

    SorokovnikDB::paint(painter, option, widget);
}

void Sorokovnik::createSorokovnik()
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    l->setOrientation(Qt::Vertical);
    l->setItemSpacing(0, 20);
    l->addItem(shemaNavig());
    l->addItem(shemaSorokovnika());

    updateDate();
    updateSegodnyaDen();
    setupConnections();
}

void Sorokovnik::deleteSorokovnik()
{
    // удаляем панель навигации
    delete mStackData;

    QGraphicsLinearLayout *ll = static_cast<QGraphicsLinearLayout *>(layout()->itemAt(0));
    while (ll->count() > 0) {
        QGraphicsLayoutItem *li = ll->itemAt(0);
        delete li->graphicsItem();
    }

    QGraphicsWidget *w = static_cast<QGraphicsWidget *>(shemaLeto()->itemAt(0));

    delete w;
    delete shemaLeto();
    delete stackMesyac();
    qDeleteAll(sorokovnikButtonGroupList());

    // удаляем кнопки Сороковника
    QGraphicsGridLayout *gl = static_cast<QGraphicsGridLayout *>(layout()->itemAt(1));
    while (gl->count() > 0) {
        QGraphicsLayoutItem *li = gl->itemAt(0);
        delete li->graphicsItem();
    }

    setLayout(0);
}

// ------------------------------- public slots ---------------------------- //
void Sorokovnik::segodnya()
{
    setLeto(mSeichasLeto);
    setSorokovnik(mSeichasSorokovnik, false);
    setDen(mSegodnyaDen);

    updateInfoLabel();
}

void Sorokovnik::naDatu(bool na)
{
    if (na) {
        mStackData->setSwitchDirection(SAGraphicsStackedLayout::Top2Bottom);
        mStackData->setCurrentIndex(1);

        QGraphicsProxyWidget *proxy = static_cast<QGraphicsProxyWidget *>(mStackData->itemAt(1));
        SorokovnikLineEdit *le = qobject_cast<SorokovnikLineEdit *>(proxy->widget());
        le->setFocus();
        connect(le, SIGNAL(editingFinished()), this, SLOT(pereitiNaDatu()));
    } else {
        mStackData->setSwitchDirection(SAGraphicsStackedLayout::Bottom2Top);
        mStackData->setCurrentIndex(0);
    }
}

// ------------------------------- private slots --------------------------- //
void Sorokovnik::updateChas(int chas)
{
    mChas = chas;
    update();
}

void Sorokovnik::updateChast(int chast)
{
    mChast = chast;
}

void Sorokovnik::updateDate()
{
    SADate date = SADate::currentDate();
    mSeichasLeto = date.leto();
    setLeto(mSeichasLeto);
    mSeichasSorokovnik = date.sorokovnik();
    setSorokovnik(mSeichasSorokovnik);
    mSegodnyaDen = date.day();
    mSvyashchennoeLeto = date.isSvyashchLeto();
    mDenNedeli = date.dayOfWeek();

    updateInfoLabel();
}

void Sorokovnik::updateInfoLabel()
{
    mLetoLabel->setText(tr("Лето %1/%2/%3\n%4 сороковник")
                        .arg(leto()).arg(kzhizni()).arg(klet())
                        .arg(sorokovnik()));
    mLetoLabel->update();
    showInfoHtml(false);
}

void Sorokovnik::updateSegodnyaDen()
{
    if (SorokovnikDen *dn = den(mSegodnyaDen, mSeichasSorokovnik))
        dn->setSegodnya(leto() == mSeichasLeto);
}

void Sorokovnik::updateInfoHtml()
{
    int denId = checkedDenId();
    if (denId != -1) {
        showInfoHtml(checkedDen()->checked());
    } else {
        showInfoHtml(false);
    }
}

void Sorokovnik::pereitiNaDatu()
{
    SorokovnikLineEdit *le = qobject_cast<SorokovnikLineEdit *>(sender());
    if (le) {
        int den, sorokovnik, leto;
        le->getDate(den, sorokovnik, leto);

        setLeto(leto, false);
        setSorokovnik(sorokovnik, false);
        setDen(den);
        updateInfoLabel();
    }
}

void Sorokovnik::showInfoHtml(bool show)
{
    if (show) {
        SorokovnikDen *den = checkedDen();
        if (den) {
            QString info;
            QString title = den->prazdnik();
            QString description = den->opisaniePrazdnika();
            if (den->isVazhnyiPrazdnik())
                info = QString("<h3 style=\"color: red\">%1</h3>\n%2").arg(title).arg(description);
            else
                info = QString("<h3>%1</h3>\n%2").arg(title).arg(description);
            emit showInfoRequest(true, info);
        }
    } else {
        emit showInfoRequest(false, "");
    }
}

// ------------------------------ protected -------------------------------- //
QGraphicsWidget *Sorokovnik::dobavitDen(int sorokovnik, int nomer, int cvet,
                                        bool nepolnMes)
{
    SorokovnikDen *den = qobject_cast<SorokovnikDen *>(
            SorokovnikDB::dobavitDen(sorokovnik, nomer, cvet, nepolnMes));
    connect(den, SIGNAL(toggled(bool)), this, SLOT(showInfoHtml(bool)));

    return den;
}

// -------------------------------- private -------------------------------- //
void Sorokovnik::setupConnections()
{
    connect(mTimer, SIGNAL(chastChanged(int)), this, SLOT(updateChast(int)));
    connect(mTimer, SIGNAL(chasChanged(int)), this, SLOT(updateChas(int)));
    connect(mTimer, SIGNAL(denChanged(int)), this, SLOT(updateDate()));

    connect(this, SIGNAL(sorokovnikChanged(int)), this, SLOT(updateInfoLabel()));
    connect(this, SIGNAL(sorokovnikChanged(int)), this, SLOT(updateInfoHtml()));
    connect(this, SIGNAL(letoChanged(int)), this, SLOT(updateInfoLabel()));
    connect(this, SIGNAL(letoChanged(int)), this, SLOT(updateSegodnyaDen()));
}

QGraphicsLayout *Sorokovnik::shemaNavig()
{
    const qreal jump = 1.2;
    QGraphicsLinearLayout *shema = new QGraphicsLinearLayout;
    QSizeF ButtonSize(DenItemSize.height(), DenItemSize.height());

    // кнопка перехода на предыдущее лето
    mPrevLetoPushButton = new SAGraphicsPushButton("", Qt::yellow);
    shema->addItem(mPrevLetoPushButton);
    mPrevLetoPushButton->setToolTip(tr("Перейти на предыдущее лето"));
    mPrevLetoPushButton->setFixedSize(ButtonSize);
    mPrevLetoPushButton->setJump(jump);
    mPrevLetoPushButton->setPixmap(QPixmap(":/SAKalendar/sorokovniklib/images/prevprev.png"));
    connect(mPrevLetoPushButton, SIGNAL(clicked()), this, SLOT(predLeto()));

    // кнопка перехода на предыдущий сороковник
    mPrevSorokovnikPushButton = new SAGraphicsPushButton("", Qt::yellow);
    shema->addItem(mPrevSorokovnikPushButton);
    mPrevSorokovnikPushButton->setToolTip(tr("Перейти на предыдущий сороковник"));
    mPrevSorokovnikPushButton->setFixedSize(ButtonSize);
    mPrevSorokovnikPushButton->setJump(jump);
    mPrevSorokovnikPushButton->setPixmap(QPixmap(":/SAKalendar/sorokovniklib/images/prev.png"));
    connect(mPrevSorokovnikPushButton, SIGNAL(clicked()), this, SLOT(predSorokovnik()));

    // кнопка перехода на дату
    SAGraphicsPushButton *naDatu = new SAGraphicsPushButton("", Qt::red);
    shema->addItem(naDatu);
    naDatu->setToolTip(tr("Перейти на дату"));
    naDatu->setFixedSize(ButtonSize);
    naDatu->setJump(jump);
    naDatu->setCheckable(true);
    connect(naDatu, SIGNAL(clicked(bool)), this, SLOT(naDatu(bool)));

    // надпись указывающая текущее лето
    QFont saFont(QFont(SAFONTFAMILYIZHITSAC, 12));
    mLetoLabel = new SAGraphicsLabelItem(tr(""), saFont, Qt::AlignCenter, "#aa1212", true);
    mLetoLabel->setFixedHeight(ButtonSize.height() + 8);

    SorokovnikLineEdit *le = new SorokovnikLineEdit;
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;
    proxy->setWidget(le);

    mStackData->addItem(mLetoLabel);
    mStackData->addItem(proxy);
    shema->addItem(mStackData);
    updateInfoLabel();

    // кнопка перехода на сегодняшний день
    SAGraphicsPushButton *segodnya = new SAGraphicsPushButton("", Qt::green);
    shema->addItem(segodnya);
    segodnya->setToolTip(tr("Перейти на сегодняшний день"));
    segodnya->setFixedSize(ButtonSize);
    segodnya->setJump(jump);
    connect(segodnya, SIGNAL(clicked()), this, SLOT(segodnya()));

    // кнопка перехода на следующий сороковник
    mNextSorokovnikPushButton = new SAGraphicsPushButton("", Qt::white);
    shema->addItem(mNextSorokovnikPushButton);
    mNextSorokovnikPushButton->setToolTip(tr("Перейти на следующий сороковник"));
    mNextSorokovnikPushButton->setFixedSize(ButtonSize);
    mNextSorokovnikPushButton->setJump(jump);
    mNextSorokovnikPushButton->setPixmap(QPixmap(":/SAKalendar/sorokovniklib/images/next.png"));
    connect(mNextSorokovnikPushButton, SIGNAL(clicked()), this, SLOT(sledSorokovnik()));

    // кнопка перехода на следующее лето
    mNextLetoPushButton = new SAGraphicsPushButton("", Qt::white);
    shema->addItem(mNextLetoPushButton);
    mNextLetoPushButton->setToolTip(tr("Перейти на следующее лето"));
    mNextLetoPushButton->setFixedSize(ButtonSize);
    mNextLetoPushButton->setJump(jump);
    mNextLetoPushButton->setPixmap(QPixmap(":/SAKalendar/sorokovniklib/images/nextnext.png"));
    connect(mNextLetoPushButton, SIGNAL(clicked()), this, SLOT(sledLeto()));

    return shema;
}
