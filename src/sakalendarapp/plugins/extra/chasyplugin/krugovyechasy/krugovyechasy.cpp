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
#include <QPainter>
#include <QPaintEvent>
#include <QSettings>
#include <QToolTip>
#include <QTimer>

#include "kd.h"
#include "kdtimetimer.h"
#include "kdtime.h"

#include "krugovyechasy.h"

#define CHETVERT(n)     n != 16 ? n / 4 : 0

KrugovyeChasy::KrugovyeChasy(bool app, QWidget *parent)
    : QWidget(parent)
    , mPausa(false)
    , mApp(app)
{
    setObjectName("KrugovyeChasy");
    setMinimumSize(50, 50);

    if (mApp) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

        QAction *effectsAction = new QAction(tr("Включить э&ффекты"), this);
        effectsAction->setShortcut(QKeySequence(Qt::Key_E));
        effectsAction->setCheckable(true);
        connect(effectsAction, SIGNAL(triggered(bool)), this, SLOT(effects(bool)));

        QAction *pauseAction = new QAction(tr("Прио&становить"), this);
        pauseAction->setShortcut(QKeySequence(Qt::Key_Space));
        pauseAction->setCheckable(true);
        connect(pauseAction, SIGNAL(triggered(bool)), this, SLOT(pause(bool)));

        QAction *sepAction = new QAction(this);
        sepAction->setSeparator(true);

        QAction *vyhodAction = new QAction(tr("В&ыход"), this);
        vyhodAction->setShortcuts(QList<QKeySequence>() << QKeySequence(Qt::Key_Q)
                                  << QKeySequence(Qt::Key_Escape)
                                  << QKeySequence(QKeySequence::Close));
        connect(vyhodAction, SIGNAL(triggered()), this, SLOT(quit()));

        addAction(effectsAction);
        addAction(pauseAction);
        addAction(sepAction);
        addAction(vyhodAction);

        setContextMenuPolicy(Qt::ActionsContextMenu);
        setToolTip(tr("Славяно-Арийские круговые часы"));
        setWindowTitle(tr("Славяно-Арийские круговые часы"));
    }

    KDTime time = KDTime::currentTime();
    mTimer = new KDTimeTimer();
    mTimer->start(4, time.dolya(), time.chast(), time.chas());

    setTime(mTimer->time());

    connect(mTimer, SIGNAL(dolyaChanged(int)), this, SLOT(setDolyu(int)));
    connect(mTimer, SIGNAL(chastChanged(int)), this, SLOT(setChast(int)));
    connect(mTimer, SIGNAL(chasChanged(int)), this, SLOT(setChas(int)));

    readSettings();

    mChasyKartinki << QPixmap(":/kalyadydar/chasy/1paobed.png")
            << QPixmap(":/kalyadydar/chasy/2vecher.png")
            << QPixmap(":/kalyadydar/chasy/3nich.png")
            << QPixmap(":/kalyadydar/chasy/4polich.png")
            << QPixmap(":/kalyadydar/chasy/5zautra.png")
            << QPixmap(":/kalyadydar/chasy/6zaura.png")
            << QPixmap(":/kalyadydar/chasy/7zaurnice.png")
            << QPixmap(":/kalyadydar/chasy/8nastya.png")
            << QPixmap(":/kalyadydar/chasy/9swaor.png")
            << QPixmap(":/kalyadydar/chasy/10utros.png")
            << QPixmap(":/kalyadydar/chasy/11poutros.png")
            << QPixmap(":/kalyadydar/chasy/12obestina.png")
            << QPixmap(":/kalyadydar/chasy/13obed.png")
            << QPixmap(":/kalyadydar/chasy/14podani.png")
            << QPixmap(":/kalyadydar/chasy/15utdayini.png")
            << QPixmap(":/kalyadydar/chasy/16poudani.png")
            ;
}

QSize KrugovyeChasy::sizeHint() const
{
    return QSize(100, 100);
}

void KrugovyeChasy::effects(bool effects)
{
    mEffects = effects;
    update();
}

void KrugovyeChasy::pause(bool pause)
{
    mPausa = pause;
    if (pause && !mTimer->paused()) {
        mTimer->pause();
        update();
    } else if (!pause) {
        mTimer->contin();
        setTime(mTimer->time());
    }
}

// ------------------------------ protected -------------------------------- //
void KrugovyeChasy::paintEvent(QPaintEvent *e)
{
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

    int delta = mPausa ? 91 : 191;

    const QColor chasCvet(127, 0, 127);
    const QColor chastCvet(0, 127, 127, 191);

    const QColor foneCentr(255, 255, 255, delta);
    const QColor foneSer(70, 200, 70, delta);
    const QColor foneKrai(QColor(230, 255, 230));
    const QColor foneTemnKrai(QColor(10, 140, 10));

    QColor chetvertCvet = KDChetvertySutok_Cveta[CHETVERT(mChas)].second;

    static const qreal povorot = 360.0 / 1296.0;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRegion(e->region());

    int side = qMin(width(), height()) - 9;
    // рисуем фон
    if (mEffects) {
        QRadialGradient rgr(width() / 2, height() / 2, side / 2);
        rgr.setColorAt(0.0, foneCentr);
        rgr.setColorAt(0.9, foneSer);
        rgr.setColorAt(1.0, foneKrai);
        rgr.setSpread(QGradient::ReflectSpread);

        QLinearGradient lgr(rect().topLeft(), rect().bottomRight());
        lgr.setColorAt(0, foneKrai);
        lgr.setColorAt(1, foneTemnKrai);

        painter.setPen(QPen(QBrush(lgr), 8.0));
        painter.setBrush(QBrush(rgr));
        painter.drawEllipse(QPoint(width() / 2, height() / 2), side / 2, side / 2);
    }

    // переносим систему координат в центр виджета
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // рисуем часы
    painter.save();
    mEffects ? painter.setPen(Qt::yellow) : painter.setPen(Qt::darkRed);
    for (int i = 1; i <= 16; ++i) {
        painter.drawLine(0, -88, 0, -96);
        painter.rotate(22.5); // 360 / 16
        if (mEffects) {
            painter.save();
            painter.translate(0, -73);
            painter.rotate(-i * 22.5);
            painter.setPen(KDChetvertySutok_Cveta[CHETVERT(i)].second);

            painter.drawText(QRect(-7, -7, 16, 14), Qt::AlignCenter, QString::number(i));
            painter.restore();
        }
    }

    painter.restore();

    // рисуем части
    if (mEffects) {
        painter.save();
        painter.setPen(Qt::yellow);
        for (int i = 0; i < 144; ++i) {
            if ((i % 9) != 0) // 144 / 16
                painter.drawLine(-92, 0, -96, 0);

            painter.rotate(2.5); // 360 / 144
        }

        painter.restore();

        // подписываем час
        QFontMetrics fm(painter.fontMetrics());
        QFont f = font();
        f.setPointSize(8);
        QString chas = KDChasy_Opisaniya[mChas - 1].first;
        int w = fm.width(chas);
        int h = fm.height();

        painter.setFont(f);
        painter.setPen(chetvertCvet);
        painter.drawText(QRect(-w / 2 + 4, -50, w, h), chas);

        // подписываем четверти суток
        f.setPointSize(7);
        h = painter.fontMetrics().height();

        painter.setFont(f);
        painter.setPen(KDChetvertySutok_Cveta[0].second);
        w = fm.width(KDChetvertySutok_Cveta[0].first);
        painter.drawText(QRect(20, -30, w, h), Qt::AlignCenter,
                         KDChetvertySutok_Cveta[0].first);

        painter.setPen(KDChetvertySutok_Cveta[1].second);
        w = fm.width(KDChetvertySutok_Cveta[1].first);
        painter.drawText(QRect(22, 22, w, h), Qt::AlignCenter,
                         KDChetvertySutok_Cveta[1].first);

        painter.setPen(KDChetvertySutok_Cveta[2].second);
        w = fm.width(KDChetvertySutok_Cveta[2].first);
        painter.drawText(QRect(-55, 22, w, h), Qt::AlignCenter,
                         KDChetvertySutok_Cveta[2].first);

        painter.setPen(KDChetvertySutok_Cveta[3].second);
        w = fm.width(KDChetvertySutok_Cveta[3].first);
        painter.drawText(QRect(-55, -30, w, h), Qt::AlignCenter,
                         KDChetvertySutok_Cveta[3].first);

        // рисуем картинка с чертами/резами, рунами, числом часа
        chetvertCvet.setAlpha(100);
        painter.setPen(Qt::darkGreen);
        painter.setBrush(QBrush(chetvertCvet));

        QPixmap pm = mChasyKartinki[mChas - 1];
        QRect r = QRect(-35, 40, 70, 16);
        painter.drawPixmap(r, pm);
        painter.drawRect(r);
    }

    // рисуем часовую стрелку
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(chasCvet);
    painter.rotate(povorot * mVolchok / 2304); // 144 * 16
    painter.drawConvexPolygon(chasStrelka, 3);
    painter.restore();

    // рисуем частную(?) стрелку
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(chastCvet);
    painter.rotate(povorot * mVolchok / 144);
    painter.drawConvexPolygon(chastStrelka, 3);
    painter.restore();

    // рисуем долевую стрелку
    painter.setBrush(QBrush(Qt::darkMagenta));
    painter.drawEllipse(-1, -1, 2, 2);
    painter.save();
    painter.rotate(povorot * mVolchok);
    painter.setPen(Qt::darkCyan);
    painter.drawLine(0, 5, 0, -85);
    painter.restore();
}

void KrugovyeChasy::mousePressEvent(QMouseEvent *e)
{
    if (mApp) {
        if (e->button() == Qt::LeftButton) {
            mDragPos = e->globalPos() - frameGeometry().topLeft();
            mPrevPos = e->pos();
            e->accept();
        }
    }
}

void KrugovyeChasy::mouseMoveEvent(QMouseEvent *e)
{
    if (mApp) {
        if (e->buttons() & Qt::LeftButton) {
            if (e->modifiers() & Qt::ShiftModifier) {
                int side = qMax(width() + (e->pos().x() - mPrevPos.x()),
                                height() + (e->pos().y() - mPrevPos.y()));
                resize(side, side);
                mPrevPos = e->pos();
                mPausa = true;
                e->accept();
            } else {
                move(e->globalPos() - mDragPos);
                e->accept();
            }
        }
    }
}

void KrugovyeChasy::mouseReleaseEvent(QMouseEvent *)
{
    if (mPausa)
        mPausa = false;
}

void KrugovyeChasy::resizeEvent(QResizeEvent *)
{
    if (mApp) {
        int side = qMin(width(), height());
        QBitmap mask(side, side);
        mask.fill(Qt::color0);

        QPainter p(&mask);
        p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        p.setPen(Qt::color1);
        p.setBrush(Qt::color1);
        p.drawEllipse(QPointF(side / 2, side / 2), side / 2, side / 2);

        setMask(mask);
    }
}

bool KrugovyeChasy::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::ToolTip:
        {
            QHelpEvent *he = static_cast<QHelpEvent *>(e);

            QString chas = QString::number(mChas);
            QString chast = QString::number(mChast);
            QString dolya = QString::number(mDolya);

            if (chas.length() < 2)
                chas.prepend("0");

            if (chast.length() < 2)
                chast.prepend("00");
            else if (chast.length() < 3)
                chast.prepend("0");

            if (dolya.length() < 2)
                dolya.prepend("000");
            else if (dolya.length() < 3)
                dolya.prepend("00");
            else if (dolya.length() < 4)
                dolya.prepend("0");

            QString toolTip(tr("<b>%1:%2:%3</b><p style='white-space:pre'><b style='color: %4'>%5</b> - %6")
                            .arg(chas).arg(chast).arg(dolya).arg(KDChetvertySutok_Cveta[CHETVERT(mChas)].second)
                            .arg(KDChasy_Opisaniya[mChas - 1].first).arg(KDChasy_Opisaniya[mChas - 1].second));
            QToolTip::showText(he->globalPos(), toolTip, this);
            return true;
        }
    default:
        return QWidget::event(e);
    }
}

// ----------------------------- private slots ----------------------------- //
void KrugovyeChasy::quit()
{
    writeSettings();
    qApp->quit();
}

void KrugovyeChasy::setDolyu(int dolya)
{
    update();

    mDolya = dolya;
    mVolchok += mInterval;
    if (mVolchok >= 3173904)
        mVolchok = 187920;
}

void KrugovyeChasy::setChast(int chast)
{
    mChast = chast;
}

void KrugovyeChasy::setChas(int chas)
{
    mChas = chas;
}

// -------------------------------- private -------------------------------- //
void KrugovyeChasy::readSettings()
{
    QSettings s;
    mEffects = s.value("/KalyadyDar/SlavyanskieChislovyeChasy/Effects", true).toBool();

    if (mApp) {
        resize(s.value("/KalyadyDar/SlavyanskieChislovyeChasy/Size",
                       QSize(280, 130)).toSize());
        move(s.value("/KalyadyDar/SlavyanskieChislovyeChasy/Pos").toPoint());
    }
}

void KrugovyeChasy::writeSettings()
{
    QSettings s;
    s.setValue("/KalyadyDar/SlavyanskieChislovyeChasy/Effects", mEffects);

    if (mApp) {
        s.setValue("/KalyadyDar/SlavyanskieChislovyeChasy/Size", size());
        s.setValue("/KalyadyDar/SlavyanskieChislovyeChasy/Pos", pos());
    }
}

void KrugovyeChasy::setTime(QString time)
{
    QStringList tm = time.split(":");
    mChas = tm.at(0).toInt();
    mChast = tm.at(1).toInt();
    mDolya = tm.at(2).toInt();

    mVolchok = mChas * 144 * 1296 + mChast* 1296 + mDolya;
    mInterval = mTimer->interval();
}
