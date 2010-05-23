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

#include <QAction>
#include <QApplication>
#include <QBitmap>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOptionGraphicsItem>
#include <QToolTip>
#include <QTimer>

#include "sadefines.h"
#include "satime_cpp.h"
#include "satimer_cpp.h"

#include "sachasy.h"

#define CHETVERT(n) (n != 16 ? n / 4 : 0)

SAChasy::SAChasy(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mBackgroundColor(QColor(10, 120, 10, 127))
    , mShadowColor(Qt::blue)
    , mTimer(0)
    , mVolchok(0)
    , mUpdateInterval(16)
    , mPravSutKrug(true)
    , mPausa(false)
    , mSimpleView(false)
    , mDolevayaStrelka(true)
    , mIgnoreDST(false)
    , mShowDate(false)
{
    setObjectName("theSAChasy");
    setAcceptHoverEvents(true);

    resetTimer(mIgnoreDST, mUpdateInterval);

    mChasyKartinki << QPixmap(":/SAKalendar/sachasylib/images/1paobed.png")
            << QPixmap(":/SAKalendar/sachasylib/images/2vecher.png")
            << QPixmap(":/SAKalendar/sachasylib/images/3nich.png")
            << QPixmap(":/SAKalendar/sachasylib/images/4polich.png")
            << QPixmap(":/SAKalendar/sachasylib/images/5zautra.png")
            << QPixmap(":/SAKalendar/sachasylib/images/6zaura.png")
            << QPixmap(":/SAKalendar/sachasylib/images/7zaurnice.png")
            << QPixmap(":/SAKalendar/sachasylib/images/8nastya.png")
            << QPixmap(":/SAKalendar/sachasylib/images/9swaor.png")
            << QPixmap(":/SAKalendar/sachasylib/images/10utros.png")
            << QPixmap(":/SAKalendar/sachasylib/images/11poutros.png")
            << QPixmap(":/SAKalendar/sachasylib/images/12obestina.png")
            << QPixmap(":/SAKalendar/sachasylib/images/13obed.png")
            << QPixmap(":/SAKalendar/sachasylib/images/14podani.png")
            << QPixmap(":/SAKalendar/sachasylib/images/15utdayini.png")
            << QPixmap(":/SAKalendar/sachasylib/images/16poudani.png")
            ;
}

SAChasy::~SAChasy()
{
    delete mTimer;
}

void SAChasy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    Q_UNUSED(widget);

    static const QPoint chasStrelka[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    static const QPoint chastStrelka[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    static const QColor chasCvet(127, 0, 127);
    static const QColor chastCvet(0, 127, 127, 191);

    static const QColor foneKrai(255, 255, 255, 127);

    static const qreal povorot = 360.0 / 1296.0;

    QRectF r = option->rect;
    QColor chetvertCvet = SAChetvertySutok_Cveta[CHETVERT(mChas)].second;
    QColor denCvet = SACvetaHTMLCveta[mDate.dayOfWeek() - 1].second;
    QPointF center(r.width() / 2, r.height() / 2);
    int side = qMin(r.width(), r.height()) - 9;

    QFont saFont(SAFONTFAMILYIZHITSAC);
    painter->setFont(saFont);
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setClipRect(option->exposedRect.toRect());

    // рисуем фон
    if (!mSimpleView) {
        QLinearGradient lgr(r.topLeft(), r.bottomRight());
        lgr.setColorAt(1, mBackgroundColor);
        lgr.setColorAt(0, foneKrai);

        painter->setPen(QPen(QBrush(lgr), 4.0));
        painter->setBrush(QBrush(lgr));
        painter->drawEllipse(center, side / 2, side / 2);

        lgr.setColorAt(0, mBackgroundColor);
        lgr.setColorAt(1, foneKrai);

        painter->setPen(QPen(QBrush(lgr), 4.0));
        painter->setBrush(QBrush(lgr));
        painter->drawEllipse(center, side / 2 - 3, side / 2 - 3);

        lgr.setColorAt(1, mBackgroundColor);
        lgr.setColorAt(0, foneKrai);

        painter->setPen(QPen(QBrush(lgr), 4.0));
        painter->setBrush(QBrush(lgr));
        painter->drawEllipse(center, side / 2 - 8, side / 2 - 8);
    }

    // переносим систему координат в центр виджета
    painter->translate(center);
    painter->scale(side / 200.0, side / 200.0);

    // рисуем часы
    painter->save();
    mSimpleView ? painter->setPen(Qt::darkRed) : painter->setPen(Qt::yellow);

    if (mPravSutKrug)
        painter->rotate(180);
    for (int i = 1; i <= 16; ++i) {
        painter->drawLine(0, -86, 0, -94);
        painter->rotate(22.5); // 360 / 16
        if (!mSimpleView) {
            painter->save();

            QTransform transform;
            transform.translate(0, -73);
            if (mPravSutKrug)
                transform.rotate(-i * 22.5 + 180);
            else
                transform.rotate(-i * 22.5);

            painter->setTransform(transform, true);
            painter->setPen(SAChetvertySutok_Cveta[CHETVERT(i)].second);

#if defined(Q_OS_UNIX)
            painter->drawText(QRect(-7, -7, 16, 14), Qt::AlignBottom | Qt::AlignHCenter, QString::number(i));
#elif defined(Q_OS_WIN)
            painter->drawText(QRect(-7, -7, 16, 14), Qt::AlignCenter, QString::number(i));
#endif

            painter->restore();
        }
    }

    painter->restore();

    // рисуем части
    if (!mSimpleView) {
        painter->save();
        painter->setPen(Qt::yellow);
        for (int i = 0; i < 144; ++i) {
            if ((i % 9) != 0) // 144 / 16
                painter->drawLine(-90, 0, -94, 0);

            painter->rotate(2.5); // 360 / 144
        }

        painter->restore();

        // подписываем час
        QFontMetrics fm(painter->fontMetrics());
        saFont.setPointSize(9);
        QString chas = SAChasy_Opisaniya[mChas - 1].first;
        int w = fm.width(chas);
        int h = fm.height();

        painter->setFont(saFont);
        painter->setPen(chetvertCvet);
        painter->drawText(QRect(-w / 2 - 2, -50, w + 4, h), Qt::AlignCenter, chas);

        // подписываем четверти суток
        saFont.setPointSize(7);
        h = painter->fontMetrics().height();

        int vecher = mPravSutKrug ? 2 : 0;
        painter->setFont(saFont);
        painter->setPen(SAChetvertySutok_Cveta[vecher].second);
        w = fm.width(SAChetvertySutok_Cveta[vecher].first);
        painter->drawText(QRect(22, -30, w, h), Qt::AlignCenter,
                          SAChetvertySutok_Cveta[vecher].first);

        int noch = mPravSutKrug ? 3 : 1;
        painter->setPen(SAChetvertySutok_Cveta[noch].second);
        w = fm.width(SAChetvertySutok_Cveta[noch].first);
        painter->drawText(QRect(22, 22, w, h), Qt::AlignCenter,
                          SAChetvertySutok_Cveta[noch].first);

        int den = mPravSutKrug ? 0 : 2;
        painter->setPen(SAChetvertySutok_Cveta[den].second);
        w = fm.width(SAChetvertySutok_Cveta[den].first);
        painter->drawText(QRect(-55, 22, w, h), Qt::AlignCenter,
                          SAChetvertySutok_Cveta[den].first);

        int utro = mPravSutKrug ? 1 : 3;
        painter->setPen(SAChetvertySutok_Cveta[utro].second);
        w = fm.width(SAChetvertySutok_Cveta[utro].first);
        painter->drawText(QRect(-55, -30, w, h), Qt::AlignCenter,
                          SAChetvertySutok_Cveta[utro].first);

        // рисуем картинка с чертами, резами, рунами, числом часа
        QRect r = QRect(-35, 40, 70, 16);
        if (mShowDate) {
            QString dateText = mDate.toString("dd/ss/ll");

            denCvet.setAlpha(100);
            saFont.setPointSize(9);

            painter->setFont(saFont);
            painter->setPen(Qt::black);
            painter->setBrush(QBrush(denCvet));
#if defined(Q_OS_UNIX)
            painter->drawText(r, Qt::AlignHCenter | Qt::AlignBottom, dateText);
#elif defined(Q_OS_WIN)
            painter->drawText(r, Qt::AlignCenter, dateText);
#endif
            painter->setPen(denCvet.darker(200));
        } else {
            QPixmap pm = mChasyKartinki[mChas - 1];

            chetvertCvet.setAlpha(100);
            painter->setPen(chetvertCvet.darker(200));
            painter->setBrush(QBrush(chetvertCvet));
            painter->drawPixmap(r, pm);
        }
        painter->drawRect(r);
    }

    int rotationPrav = mPravSutKrug ? 180 : 0;
    // рисуем часовую стрелку
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(chasCvet);
    painter->rotate(povorot * mVolchok / 2304 + rotationPrav); // 144 * 16
    painter->drawConvexPolygon(chasStrelka, 3);
    painter->restore();

    // рисуем частную(?) стрелку
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(chastCvet);
    painter->rotate(povorot * mVolchok / 144 + rotationPrav);
    painter->drawConvexPolygon(chastStrelka, 3);
    painter->restore();

    // рисуем долевую стрелку
    if (mDolevayaStrelka) {
        painter->setBrush(QBrush(Qt::darkMagenta));
        painter->drawEllipse(-1, -1, 2, 2);
        painter->save();
        painter->rotate(povorot * mVolchok + rotationPrav);
        painter->setPen(Qt::darkCyan);
        painter->drawLine(0, 5, 0, -82);
        painter->restore();
    }
}

void SAChasy::setFixedWidth(qreal width)
{
    setMinimumWidth(width);
    setMaximumWidth(width);
}

void SAChasy::setFixedHeight(qreal height)
{
    setMinimumHeight(height);
    setMaximumHeight(height);
}

void SAChasy::setFixedSize(const QSizeF &size)
{
    setMinimumSize(size);
    setMaximumSize(size);
}

void SAChasy::setFixedSize(int width, int height)
{
    setFixedSize(QSizeF(width, height));
}

void SAChasy::pause(bool pause)
{
    mPausa = pause;
    if (pause && !mTimer->paused()) {
        mTimer->pause();
        update();
    } else if (!pause) {
        mTimer->contin();
        setTime(SATime::currentTime());
    }
}

// ------------------------------ protected -------------------------------- //
void SAChasy::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QString timeText = SATime(mChas, mChast, mDolya).toString("cc:CCC:DDDD");
    QString chasText = SAChasy_Opisaniya[mChas - 1].first;
    chasText = chasText.replace("h", "е");

    QString toolTip(tr("<b>%1</b><br><sp style='white-space:pre'><b style='color: %2'>%3</b> - %4</sp>")
                    .arg(timeText).arg(SAChetvertySutok_Cveta[CHETVERT(mChas)].second)
                    .arg(chasText).arg(SAChasy_Opisaniya[mChas - 1].second));
    QToolTip::showText(event->screenPos(), toolTip);

    QGraphicsWidget::hoverMoveEvent(event);
}

// ----------------------------- private slots ----------------------------- //
void SAChasy::setDolyu(int dolya)
{
    update();

    mDolya = dolya;
    mVolchok += mUpdateInterval;
    if (mVolchok >= 3173904)
        mVolchok = 187920;
}

void SAChasy::setChast(int chast)
{
    mChast = chast;
}

void SAChasy::setChas(int chas)
{
    mChas = chas;
}

void SAChasy::updateDate()
{
    mDate = SADate::currentDate();
}

// -------------------------------- private -------------------------------- //
void SAChasy::resetTimer(bool ignoreDST, int updateInterval)
{
    if (mTimer) {
        disconnect(mTimer, SIGNAL(dolyaChanged(int)), this, SLOT(setDolyu(int)));
        disconnect(mTimer, SIGNAL(chastChanged(int)), this, SLOT(setChast(int)));
        disconnect(mTimer, SIGNAL(chasChanged(int)), this, SLOT(setChas(int)));
        disconnect(mTimer, SIGNAL(denChanged(int)), this, SLOT(updateDate()));

        delete mTimer;
    }

    SATime time = SATime::currentTime(ignoreDST);

    mTimer = new SATimer(this);
    mTimer->start(updateInterval, time.dolya(), time.chast(), time.chas());

    connect(mTimer, SIGNAL(dolyaChanged(int)), this, SLOT(setDolyu(int)));
    connect(mTimer, SIGNAL(chastChanged(int)), this, SLOT(setChast(int)));
    connect(mTimer, SIGNAL(chasChanged(int)), this, SLOT(setChas(int)));
    connect(mTimer, SIGNAL(denChanged(int)), this, SLOT(updateDate()));

    setTime(time);
    updateDate();
}

void SAChasy::setTime(const SATime &time)
{
    mChas = time.chas();
    mChast = time.chast();
    mDolya = time.dolya();

    mVolchok = mChas * 144 * 1296 + mChast* 1296 + mDolya;
}
