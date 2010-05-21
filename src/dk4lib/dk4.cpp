/*****************************************************************************
 *   Copyright (C) 2009-2010  Jack R  <lynx21.12.12@yandex.ru>               *
 *                                                                           *
 *   This file is part of SAKalendar.                                        *
 *                                                                           *
 *   SAKalendar is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

#include <QDebug>
#include <QDialogButtonBox>

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>

#include <QMessageBox>

#include <QPainter>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QStyleOptionGraphicsItem>

#include "sadefines.h"
#include "sagraphicslabelitem.h"
#include "sagraphicsbuttongroup.h"
#include "sagraphicswindow.h"
#include "sadate_cpp.h"

#include "dk4leto.h"
#include "dk4kzhiznilineedit.h"
#include "dk4.h"

#define CHET(x) ((x) % 2 == 0)

const QSizeF LetoItemSize = QSizeF(33.0, 33.0);

DK4Base::DK4Base(QGraphicsItem *parent, bool deflt)
    : QGraphicsWidget(parent)
    , mLetoPrototype(0)
    , mAltColor(false)
{
    init();

    if (deflt)
        createDK4();
}

void DK4Base::init()
{
    setAcceptHoverEvents(true);

    mDK4ButtonGroup = new SAGraphicsButtonGroup(this);
    mDK4ButtonGroup->setExclusive(true);
    connect(mDK4ButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(kzhizniClicked(int)));
}

void DK4Base::setKZhizni(int theKzhizni) {
    mKZhizni = theKzhizni;
}

int DK4Base::checkedKZhizniId() const
{
    return mDK4ButtonGroup->checkedId();
}

DK4Leto *DK4Base::checkedKZhizni() const
{
    return qobject_cast<DK4Leto *>(mDK4ButtonGroup->checkedButton());
}

DK4Leto *DK4Base::lvkzhizni(int lvkzhizni) const
{
    return qobject_cast<DK4Leto *>(mDK4ButtonGroup->button(lvkzhizni));
}

void DK4Base::createDK4()
{
    setLayout(shemaKrugoleta());
}

void DK4Base::deleteDK4()
{
    QGraphicsGridLayout *gl = static_cast<QGraphicsGridLayout *>(layout());
    while (gl->count() > 0) {
        QGraphicsLayoutItem *li = gl->itemAt(0);
        delete li->graphicsItem();
    }

    setLayout(0);
}

// ----------------------------- public slots ------------------------------ //
void DK4Base::sledKZhizniOtSMZH()
{
    mKZhizniOtSMZH++;
    emit kzhizniOtSMZHChanged(mKZhizniOtSMZH);
}

void DK4Base::predKZhizniOtSMZH()
{
    mKZhizniOtSMZH--;
    emit kzhizniOtSMZHChanged(mKZhizniOtSMZH);
}

void DK4Base::kzhizniClicked(int kzhizni)
{
    mKZhizni = kzhizni;
    emit selected(kzhizni);
}

// ------------------------------ protected -------------------------------- //
DK4Leto *DK4Base::createLeto(int nomerLeta, int denNedeliNovoletiya, int krugLet,
                             int stikhiyaICvet, int nachalo)
{
    return mLetoPrototype ? mLetoPrototype->clone(
            nomerLeta, denNedeliNovoletiya, krugLet, stikhiyaICvet, nachalo)
                : new DK4Leto(nomerLeta, denNedeliNovoletiya, krugLet, stikhiyaICvet, nachalo);
}

QGraphicsLayout *DK4Base::shemaKrugoleta()
{
    QFont saFont(SAFONTFAMILYIZHITSAC);

    QGraphicsGridLayout *shemaKrugolet = new QGraphicsGridLayout;
    shemaKrugolet->setSpacing(0);
    shemaKrugolet->setColumnSpacing(0, 2.0);

    // подписываем Природные Стихии
    SAGraphicsLabelItem *item;
    int c = 0; // цвет
    int col = 1; // колонки
    for (int i = 0; i < SACOLSTIHIYI; i++) {
        item = new SAGraphicsLabelItem(SAStihiya_Cvet[i].first, saFont);
        shemaKrugolet->addItem(item, 0, col, 1, 2);
        item->setMaximumHeight(LetoItemSize.height());
        item->setMinimumHeight(LetoItemSize.height());
        item->setToolTip(tr("Природные стихии"));

        QFont f = QFont(SAFONTFAMILYIZHITSAC, 10);
        item = new SAGraphicsLabelItem(SAStihiya_Cvet[i].second + tr("\nЖ      М"),
                                       f, Qt::AlignCenter, SACvetaHTMLCveta[i].second, true);
        shemaKrugolet->addItem(item, 1, col, 1, 2);
        item->setMaximumHeight(LetoItemSize.height());
        item->setMinimumHeight(LetoItemSize.height());
        item->setToolTip(tr("Цвет\nНачало"));

        col += 2;
    }

    // подписываем Круги Лет
    for (int i = 2; i < SAKRUGLET + 2; ++i) {
        QFont f = saFont;
        f.setPointSize(10);
        QString krugLet = QString::number(i - 1) + ". " + SAKrugiLet[i - 2];
        if (i == SAKRUGLET + 1) {
            f.setBold(true);
            item = new SAGraphicsLabelItem(krugLet, f, (Qt::AlignLeft | Qt::AlignVCenter));
            item->setColor(Qt::darkRed);
        } else {
            item = new SAGraphicsLabelItem(krugLet, f, Qt::AlignLeft | Qt::AlignVCenter);
        }

        shemaKrugolet->addItem(item, i, 0);
        item->setFixedSize(75, LetoItemSize.height());
        item->setToolTip(tr("Круг Лет"));
    }

    // строим нижнюю часть календаря
    QGraphicsWidget *leto;
    int l = 1; // порядковый номер лета
    c = 0;
    for (int j = 1; j < SAKRUGLET + 1; ++j) {
        l = j;
        for (int i = j + 1; i < SAKRUGLET + 2; ++i) {
            if ((CHET(j) && !CHET(i)) || (!CHET(j) && CHET(i))) {
                leto = dobavitLeto(l, i - 2, c, j % 2 == 0);
                shemaKrugolet->addItem(leto, i, j);
                l += 18;
            }
        }

        if (CHET(j)) c++;
    }

    // строим верхнюю часть календаря
    c = 1;
    for (int j = 3; j < SAKRUGLET + 3; ++j) {
        l = 126 + j; // 129 + j - 3
        for (int i = j; i > 1; --i) {
            if ((CHET(j) && !CHET(i)) || (!CHET(j) && CHET(i))) {
                leto = dobavitLeto(l, i - 2, c, j % 2 == 0);
                shemaKrugolet->addItem(leto, i, j);
                l -= 18;
            }
        }

        if (CHET(j)) c++;
    }

    return shemaKrugolet;
}

// ------------------------------ private ---------------------------------- //
QGraphicsWidget *DK4Base::dobavitLeto(int nomer, int krugLet, int cvet, int nachalo)
{
    int denNedeliNovoletiya = SADate::nachRamkhata(nomer + 128) - 1;
    if (mAltColor)
        cvet = denNedeliNovoletiya;

    DK4Leto *leto = createLeto(nomer, denNedeliNovoletiya, krugLet, cvet, nachalo);
    leto->setMaximumSize(LetoItemSize);
    if (nomer % SAKRUGLET == 0) {
        QFont f(SAFONTFAMILYIZHITSAC);
        f.setBold(true);
        leto->setFont(f);
    }

    mDK4ButtonGroup->addButton(leto, nomer);
    return leto;
}

// ************************************************************************* //

// ------------------------------ public ----------------------------------- //
DK4DB::DK4DB(QGraphicsItem *parent, bool deflt)
    : DK4Base(parent, false)
    , mOpisanieLetDbConnectionName("")
    , mOpisanieLetDbName("")
{
    if (deflt)
        createDK4();

    connect(this, SIGNAL(kzhizniOtSMZHChanged(int)), this, SLOT(reopenOpisanieLetDb(int)));
}

QSqlDatabase DK4DB::openOpisanieLetDb(int kzhizniFromSMZH)
{
#if defined(Q_OS_UNIX)
    QString dbName = QString("../share/databases/%1_%2.db").arg(SADBOPISANIELETNAME)
                     .arg(kzhizniFromSMZH);
#elif defined(Q_OS_WIN)
    QString dbName = QString("databases/%1_%2.db").arg(SADBOPISANIELETNAME)
                     .arg(kzhizniFromSMZH);
#endif
    QString dbConnectionName = QString("%1_%2.db").arg(SADBOPISANIELETNAME)
                               .arg(kzhizniFromSMZH);
    return openOpisanieLetDb(dbName, dbConnectionName);
}

QSqlDatabase DK4DB::openOpisanieLetDb(const QString &name, const QString &connectionName)
{
    if (!name.isEmpty()) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(name);

        QString errorMessage = tr("Не удалось открыть базу данных описаний лет, либо база данных пуста "
#if defined(Q_OS_UNIX)
                                  "(для создания и заполнения базы данных описаний лет воспользуйтесь приложением sadb).<br>"
#elif defined(Q_OS_WIN)
                                  "(для создания и заполнения базы данных описаний лет воспользуйтесь приложением sadb.exe).<br>"
#endif
                                  "Описания лет будут недоступны.");
        if (!db.open()) {
            qCritical() << tr("Не удалось открыть базу данных") << db.lastError();
            QMessageBox::critical(0, tr("Ошибка"), errorMessage);
            return QSqlDatabase();
        }

        if (db.tables().count() != 1) {
            qCritical() << tr("Число таблиц в базе данных меньше необходимого");
            QMessageBox::critical(0, tr("Ошибка"), errorMessage);
            return QSqlDatabase();
        }

        mOpisanieLetDbConnectionName = connectionName;
        qWarning() << tr("База данных %1 открыта").arg(name);
        return db;
    } else {
        qWarning() << tr("Введите имя файла базы данных");
        return QSqlDatabase();
    }
}

// ----------------------------- protected --------------------------------- //
bool DK4DB::getInfoFromDB(int kzhizni, QString &name, QString &descr,
                          QString &symbol, QString &power, QString &color)
{
    if (mOpisanieLetDbConnectionName.isEmpty()) {
        qWarning() << tr("Не задано имя соединения с базой данных");
        name = "";
        descr = "";
        symbol = "";
        power = "";
        color = "";

        return false;
    }

    QSqlDatabase db = QSqlDatabase::database(mOpisanieLetDbConnectionName);
    if (db.isOpen()) {
        QSqlQuery q(db);
        q.prepare("SELECT name, descr, symbol, power, color FROM opisanie_let WHERE id=?");
        q.addBindValue(kzhizni);
        q.exec();
        q.next();

        if (q.isValid()) {
            name = q.value(0).toString();
            descr = q.value(1).toString();
            symbol = q.value(2).toString();
            power = q.value(3).toString();
            color = q.value(4).toString();
        }

        return true;
    } else {
        qWarning() << tr("База данных не открыта");
        return false;
    }
}

void DK4DB::reopenOpisanieLetDb(int kzhizniFromSMZH)
{
    QSqlDatabase::removeDatabase(mOpisanieLetDbConnectionName);
    openOpisanieLetDb(kzhizniFromSMZH);
}

// ************************************************************************* //

DK4::DK4(QGraphicsItem *parent, bool deflt)
    : DK4DB(parent, false)
    , mConturColor(QColor(100, 200, 100, 127))
    , mDiagonalColor(QColor(0, 200, 200, 64))
    , mKZhizniLabelOtSMZH(0)
    , mKZhizniLineEdit(0)
{
    QDialogButtonBox::ButtonLayout layout =
            QDialogButtonBox::ButtonLayout(style()->styleHint(QStyle::SH_DialogButtonLayout));
    if (layout == QDialogButtonBox::GnomeLayout) {
        setContentsMargins(0, 0, 0, 0);
    } else {
        setContentsMargins(5, 5, 5, 5);
    }

    if (deflt)
        createDK4();
}

void DK4::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);

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
        QGraphicsGridLayout *shemaKrugoleta = dynamic_cast<QGraphicsGridLayout *>(
                layout()->itemAt(1));
        if (shemaKrugoleta)
        {
            int col = shemaKrugoleta->columnCount() - 1;
            int row = shemaKrugoleta->rowCount() - 1;

            QPointF p1 = shemaKrugoleta->itemAt(1, 1)->geometry().topLeft();
            QPointF p2 = shemaKrugoleta->itemAt(row, col)->geometry().bottomLeft();
            QPointF p3 = shemaKrugoleta->itemAt(row, 0)->geometry().bottomRight() +
                         QPointF(shemaKrugoleta->columnSpacing(0), 0);
            QPointF p4 = shemaKrugoleta->itemAt(row, col)->geometry().bottomRight();
            QPointF p5 = shemaKrugoleta->itemAt(1, col)->geometry().topRight();

            QPolygonF krugoletPolyNiz(QPolygonF() << p1 << p2 << p3);
            QPolygonF krugoletPolyVerkh(QPolygonF() << p1 << p2 << p4 << p5);

            painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                                    QPainter::SmoothPixmapTransform);
            painter->setClipRect(option->exposedRect);

            // рисуем треугольник с градиентом от бирюзового до белого для нижней части календаря
            QRectF r(krugoletPolyNiz[0], krugoletPolyNiz[1]);
            QLinearGradient lgr(r.center(), r.bottomLeft());
            lgr.setColorAt(0, mDiagonalColor);
            lgr.setColorAt(1, QColor(255, 255, 255, 64));

            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(lgr));
            painter->drawPolygon(QPolygonF(krugoletPolyNiz));

            // рисуем треугольник с градиентом от бирюзового до белого для верхней части календаря
            r = QRectF(krugoletPolyVerkh[0], krugoletPolyVerkh[1]);
            lgr.setStart(r.topRight());
            lgr.setFinalStop(r.center());

            painter->setBrush(QBrush(lgr));
            painter->drawPolygon(QPolygonF(krugoletPolyVerkh));
        }
    }
}

void DK4::createDK4()
{
    init();

    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    l->setOrientation(Qt::Vertical);
    l->addItem(shemaNavig());
    l->addItem(shemaKrugoleta());

    updateDate();
    updateSeichasKZhizni();
    mKZhizniLineEdit->setText(QString::number(kzhizniOtSMZH()));

    setupConnections();
}

void DK4::deleteDK4()
{
    // удаляем панель навигации
    QGraphicsLinearLayout *ll = static_cast<QGraphicsLinearLayout *>(layout()->itemAt(0));
    // удаляем кнопку predKZhizni
    delete ll->itemAt(0)->graphicsItem();

    QGraphicsLinearLayout *lll = static_cast<QGraphicsLinearLayout *>(ll->itemAt(0));
    ll->removeAt(0);
    // удаляем mKZhizniLineEdit
    delete lll->itemAt(0)->graphicsItem();
    // удаляем mKZhizniLabelOtSMZH
    delete lll->itemAt(0)->graphicsItem();

    // удаляем кнопку sledKZhizni
    delete ll->itemAt(0)->graphicsItem();

    delete dk4ButtonGroup();

    // удаляем кнопки Круголета
    QGraphicsGridLayout *gl = static_cast<QGraphicsGridLayout *>(layout()->itemAt(1));
    while (gl->count() > 0) {
        QGraphicsLayoutItem *li = gl->itemAt(0);
        delete li->graphicsItem();
    }

    setLayout(0);
}

QGraphicsWidget *DK4::dobavitLeto(int nomer, int krugLet, int cvet, int nachalo)
{
    DK4Leto *leto = qobject_cast<DK4Leto *>(
            DK4Base::dobavitLeto(nomer, krugLet, cvet, nachalo));
    connect(leto, SIGNAL(clicked(bool)), this, SLOT(showInfoHtml(bool)));

    return leto;
}

void DK4::showInfoHtml(bool show)
{
    if (show) {
        int kzhizniID = checkedKZhizniId();
        if (kzhizniID != -1) {
            QString name, description, symbol, power, color;
            getInfoFromDB(kzhizniID, name, description, symbol, power, color);
            QString info = QString("<h3 style=\"color: %1\">%2</h3><br>%3")
                           .arg(color).arg(name).arg(description);

            emit showInfoRequest(true, info);
        }
    } else {
        emit showInfoRequest(false, "");
    }
}

void DK4::updateDate()
{
    SADate date = SADate::currentDate();
    mSeichasKZhizniOtSMZH = date.kzhizniFromSMZH();
    mSeichasLetoVKZhizni = date.lvkzhizni();
    setKZhizniOtSMZH(date.kzhizniFromSMZH());
    setKZhizni(date.lvkzhizni());

    updateInfoLabel();
}

// -------------------------------- private -------------------------------- //
void DK4::setupConnections()
{
    connect(this, SIGNAL(kzhizniOtSMZHChanged(int)), this, SLOT(updateInfoLabel()));
    connect(this, SIGNAL(kzhizniOtSMZHChanged(int)), this, SLOT(updateInfoHtml()));
    connect(this, SIGNAL(kzhizniOtSMZHChanged(int)), this, SLOT(updateSeichasKZhizni()));
}

void DK4::updateInfoLabel()
{
    mKZhizniLineEdit->setText(QString::number(kzhizniOtSMZH()));
}

void DK4::updateInfoHtml()
{
    int denId = checkedKZhizniId();
    if (denId != -1) {
        showInfoHtml(checkedKZhizni()->checked());
    } else {
        showInfoHtml(false);
    }
}

void DK4::updateKZhizniOtSMZH()
{
    setKZhizniOtSMZH(mKZhizniLineEdit->text().toInt());
}

void DK4::updateSeichasKZhizni()
{
    if (DK4Leto *k = lvkzhizni(mSeichasLetoVKZhizni))
        k->setSeichasLeto(kzhizniOtSMZH() == mSeichasKZhizniOtSMZH);
}

// -------------------------------- private -------------------------------- //
QGraphicsLayout *DK4::shemaNavig()
{
    const qreal jump = 1.2;
    QGraphicsLinearLayout *shema = new QGraphicsLinearLayout;
    shema->setItemSpacing(0, 44);
    QSizeF ButtonSize(LetoItemSize.height(), LetoItemSize.height());

    // кнопка перехода на предыдущий Круг Жизни
    SAGraphicsPushButton *predKZhizni = new SAGraphicsPushButton("", Qt::yellow);
    shema->addItem(predKZhizni);
    predKZhizni->setToolTip(tr("Перейти на предыдущее Круг Жизни"));
    predKZhizni->setFixedSize(ButtonSize);
    predKZhizni->setJump(jump);
    predKZhizni->setPixmap(QPixmap(":/SAKalendar/dk4lib/icons/prev.png"));
    connect(predKZhizni, SIGNAL(clicked()), this, SLOT(predKZhizniOtSMZH()));

    // надпись указывающая текущее лето
    QColor labelColor(170, 18, 18);
    QFont saFont(QFont(SAFONTFAMILYIZHITSAC, 16));
    mKZhizniLabelOtSMZH = new SAGraphicsLabelItem(tr("Круг Жизни от Сотворения Мира в Звёздном Храме"),
                                                  saFont, Qt::AlignCenter,
                                                  labelColor.name());
    mKZhizniLabelOtSMZH->setFixedHeight(ButtonSize.height());

    mKZhizniLineEdit = new DK4KZhizniLineEdit;
    mKZhizniLineEdit->setTextColor(labelColor);
    connect(mKZhizniLineEdit, SIGNAL(editingFinished()),
            this, SLOT(updateKZhizniOtSMZH()));

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;
    proxy->setWidget(mKZhizniLineEdit);

    QGraphicsLinearLayout *ll = new QGraphicsLinearLayout;
    ll->addItem(proxy);
    ll->addItem(mKZhizniLabelOtSMZH);
    shema->addItem(ll);

    // кнопка перехода на следующий Круг Жизни
    SAGraphicsPushButton *sledKZhizni = new SAGraphicsPushButton("", Qt::yellow);
    shema->addItem(sledKZhizni);
    sledKZhizni->setToolTip(tr("Перейти на предыдущее Круг Жизни"));
    sledKZhizni->setFixedSize(ButtonSize);
    sledKZhizni->setJump(jump);
    sledKZhizni->setPixmap(QPixmap(":/SAKalendar/dk4lib/icons/next.png"));
    connect(sledKZhizni, SIGNAL(clicked()), this, SLOT(sledKZhizniOtSMZH()));

    return shema;
}
