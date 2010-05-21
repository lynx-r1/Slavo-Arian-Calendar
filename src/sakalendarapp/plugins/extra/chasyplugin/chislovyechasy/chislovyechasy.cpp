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

#include <QApplication>
#include <QAction>
#include <QBitmap>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QSettings>
#include <QToolTip>

#include <math.h>

#include "kd.h"
#include "chislovyechasy.h"

ChislovyeChasy::ChislovyeChasy(int chas, int chast, int dolya, bool app, QWidget *parent) :
        QWidget(parent)
        , mChas(chas)
        , mChast(chast)
        , mDolya(dolya)
        , mApp(app)
{
    setObjectName("ChislovyeChasy");
    setMouseTracking(true);
    setMinimumSize(QSize(380, 130));
    setMaximumHeight(200);

    if (mApp) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
        QAction *vyhodAction = new QAction(tr("Выход"), this);
        vyhodAction->setShortcuts(QList<QKeySequence>() << QKeySequence(Qt::Key_Q)
                                   << QKeySequence(Qt::Key_Escape)
                                   << QKeySequence(QKeySequence::Close));
        connect(vyhodAction, SIGNAL(triggered()), this, SLOT(quit()));
        addAction(vyhodAction);

        setContextMenuPolicy(Qt::ActionsContextMenu);
        setToolTip(tr("Славяно-Арийские числовые часы"));
        setWindowTitle(tr("Славяно-Арийские числовые часы"));
    }

    readSettings();
    init();

    mPausa = isEnabled();

    // обновлять нужно каждые 28.93518518518518518518 милисекунды, но так как
    // в таймере можно задать только целые значение, то округляем время обновления
    // 29 милисекунд слишком мало, поэтому удваеваем его 29 * 2 (57.87037037037037037037)
    startTimer(29 * KDTIMERINTERVAL);
}

QSize ChislovyeChasy::sizeHint() const
{
    return QSize(432, 144);
}

// ---------------------------- protected ---------------------------------- //
void ChislovyeChasy::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setClipRegion(e->region());

    // рисуем фон в виде панели
    if (mKrasota)
        drawBackground(painter);

    // пишем название часа
    painter.setFont(mZaglavnFont);
    painter.setPen(QPen(cvetChasaDark()));

    painter.drawText(mZaglavyeRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap,
                     KDChasy_Opisaniya[mChas].first);

    // рисуем картинку часа с фоном цвета текущего часа
    if (mKrasota) {
        QColor transpChasCvet = cvetChasaDark();
        transpChasCvet.setAlpha(16);
        painter.setBrush(QBrush(transpChasCvet));
    }

    painter.drawPixmap(mChertyRezyChasRect, mChasyKartinki[mChas - 1]);
    painter.drawRect(mChertyRezyChasRect);

    chislovayLiniya(painter, mVerhLiniiPen, mNizLiniiPen, mNadpisFont);
}

void ChislovyeChasy::timerEvent(QTimerEvent *)
{
    if (!mPausa) {
        update();
        tickChasov();
    }
}

void ChislovyeChasy::mousePressEvent(QMouseEvent *e)
{
    if (mApp) {
        if (e->button() == Qt::LeftButton) {
            mDragPos = e->globalPos() - frameGeometry().topLeft();
            e->accept();
        }
    }
}

void ChislovyeChasy::mouseMoveEvent(QMouseEvent *e)
{
    if (mApp) {
        if (e->buttons() & Qt::LeftButton) {
            move(e->globalPos() - mDragPos);
            e->accept();
        }
    }
}

void ChislovyeChasy::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    if (mApp) {
        QBitmap mask(rect().size());
        mask.fill(Qt::color0);

        QPainter p(&mask);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillPath(roundRectPath(rect()), Qt::color1);

        setMask(mask);
    }

    updateSize(e->size());
}

void ChislovyeChasy::changeEvent(QEvent *e)
{
    mPausa = !mPausa;
    QWidget::changeEvent(e);
}

void ChislovyeChasy::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    updateSize(size());
}

bool ChislovyeChasy::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::ToolTip:
        {
            QHelpEvent *he = static_cast<QHelpEvent *>(e);
            QPoint p = he->pos();
            QPoint gp = he->globalPos();
            if (mZaglavyeRect.contains(p)) {
                QToolTip::showText(gp, KDChasy_Opisaniya[mChas].second,
                                   this, mZaglavyeRect);
            } else if (mLineinyeChasyRect.contains(p)) {
                QToolTip::showText(gp, tr("%1:%2:%3").arg(mChas).arg(mChast).arg(mDolya),
                                   this, mLineinyeChasyRect);
            } else if (mChertyRezyChasRect.contains(p)) {
                QToolTip::showText(gp, tr("Здравия!"),
                                   this, mChertyRezyChasRect);
            }

            return true;
        }
    default:
        return QWidget::event(e);
    }
}

// ----------------------------- private slots ----------------------------- //
void ChislovyeChasy::quit()
{
    writeSettings();
    qApp->quit();
}

// -------------------------------- private -------------------------------- //
void ChislovyeChasy::readSettings()
{
    QSettings s;
    mVysotaStrelki = s.value("/KalyadyDar/SlavyanskieKrugovyeChasy/VysotaStrelki", 7).toInt();
    mShyrinaStrelki = s.value("/KalyadyDar/SlavyanskieKrugovyeChasy/ShirinaStrelki", 5).toInt();
    mVysotaCherty = s.value("/KalyadyDar/SlavyanskieKrugovyeChasy/VysotaCherty", 3).toInt();

    mKrasota = s.value("/KalyadyDar/SlavyanskieKrugovyeChasy/Krasota", true).toBool();

    if (mApp) {
        resize(s.value("/KalyadyDar/SlavyanskieKrugovyeChasy/Size", QSize(432, 144)).toSize());
        move(s.value("/KalyadyDar/SlavyanskieKrugovyeChasy/Pos").toPoint());
    }
}

void ChislovyeChasy::writeSettings()
{
    QSettings s;
    s.setValue("/KalyadyDar/SlavyanskieKrugovyeChasy/VysotaStrelki", mVysotaStrelki);
    s.setValue("/KalyadyDar/SlavyanskieKrugovyeChasy/ShirinaStrelki", mShyrinaStrelki);
    s.setValue("/KalyadyDar/SlavyanskieKrugovyeChasy/VysotaCherty", mVysotaCherty);

    s.setValue("/KalyadyDar/SlavyanskieKrugovyeChasy/Krasota", mKrasota);

    if (mApp) {
        s.setValue("/KalyadyDar/SlavyanskieKrugovyeChasy/Size", size());
        s.setValue("/KalyadyDar/SlavyanskieKrugovyeChasy/Pos", pos());
    }
}

void ChislovyeChasy::init()
{
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
            << QPixmap(":/kalyadydar/chasy/15utdayni.png")
            << QPixmap(":/kalyadydar/chasy/16poudani.png")
            ;

    // инициализируем переменные
    mVerhLiniiPen = QPen(QColor(200, 0, 0), 3.0, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin);
    mNizLiniiPen = QPen(QColor(0, 0, 200), 1.0, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin);

    mNadpisFont = QFont(font().family(), 11);
    mNadpisFontMetrics = new QFontMetrics(mNadpisFont);

    mZaglavnFont = QFont(font().family(), 14);
    mZaglavnFontMetrics = new QFontMetrics(mZaglavnFont);

    mOffset.setX(18);
    mOffset.setY(20);
}

void ChislovyeChasy::updateSize(QSize size)
{
    mdxChas = (size.width() - 2 * mOffset.x()) / 16;
    mdxChast = mdxChas / 9;
    mdxDolya = mdxChast / 9;

    int chasNazvWidth = mZaglavnFontMetrics->width(KDChasy_Opisaniya[mChas].first);
    mZaglavyeRect = QRect((size.width() - chasNazvWidth) / 2, mOffset.y(),
                            chasNazvWidth, mZaglavnFontMetrics->height());

    mLineinyeChasyRect = QRect(mOffset.x(), mZaglavyeRect.bottom() + mVysotaStrelki,
                                  16 * mdxChas, 2 * mNadpisFontMetrics->height() +
                                  2 * mVysotaStrelki + 2 * mVysotaCherty);

    mChertyRezyChasRect = mChasyKartinki[mChas - 1].rect();
    mChertyRezyChasRect.moveTopLeft(QPoint((size.width() - mChertyRezyChasRect.width()) / 2,
                                              mLineinyeChasyRect.bottom()));

    mOffset.setY(size.height() / 2 - mLineinyeChasyRect.height() - mVysotaStrelki);
    repaint();
}

void ChislovyeChasy::chislovayLiniya(QPainter &p, QPen mVerhLiniiPen,
                                           QPen mNizLiniiPen, QFont mNadpisFont)
{
    QPen penChasa(cvetChasaDark());
    QBrush brushChasa(cvetChasaDark());
    QPen penChasti(cvetChasaDark());
    QBrush brushChasti(cvetChasaDark());

    QFontMetrics mNadpisFontMetrics(mNadpisFont);

    // устанавливаем шрифт
    if (mKrasota)
        mVerhLiniiPen.setColor(KDChetvertySutok_Cveta[0].second);
    else
        mVerhLiniiPen.setColor(cvetChasaLight());

    p.setPen(mVerhLiniiPen);
    p.setFont(mNadpisFont);

    // создаем кисть с градиентом ото дня к вечеру (красный -> бирюзовый)
    // и рисуем линию часов с числовыми подписями от 1 до 16 часов
    int c = 0;
    QLinearGradient lgr;
    qreal tmpX = mOffset.x();
    int offsY = mLineinyeChasyRect.center().y();
    for (int i = 1; i <= 16; ++i) {
        switch (i)
        {
        case 2:
            if (mKrasota) {
                lgr.setStart(-2 * mdxChas, offsY);
                lgr.setFinalStop(mdxChas, offsY);
                lgr.setColorAt(0, KDChetvertySutok_Cveta[3].second);
                lgr.setColorAt(1, KDChetvertySutok_Cveta[0].second);

                mVerhLiniiPen.setBrush(QBrush(lgr));

            }

            p.setPen(mVerhLiniiPen);
            p.drawRect(mOffset.x(), offsY, mdxChas, 1);

        case 6:
        case 10:
        case 14:
            if (mKrasota) {
                lgr.setStart(tmpX, offsY);
                lgr.setFinalStop(tmpX + 4 * mdxChas, offsY);
                lgr.setColorAt(0, KDChetvertySutok_Cveta[c].second);
                lgr.setColorAt(1, KDChetvertySutok_Cveta[c != 3 ? c + 1 : 0].second);

                mVerhLiniiPen.setBrush(QBrush(lgr));
                c++;
            }

            p.setPen(mVerhLiniiPen);
            p.drawRect(tmpX, offsY, (i != 14 ? 4 : 3) * mdxChas, 1);
            break;
        }

        QString chas = QString::number(i);
        QRectF r = QRectF(tmpX - mNadpisFontMetrics.width(chas) / 2 - 0.5, mLineinyeChasyRect.top(),
                        mNadpisFontMetrics.width(chas), mNadpisFontMetrics.height() - 4);

        p.save();
        if (mKrasota) {
            p.setPen(QPen(QBrush(lgr), 1.0, Qt::DotLine, Qt::FlatCap, Qt::BevelJoin));
            p.drawRect(r);
        }

        p.setPen(mVerhLiniiPen);
        p.drawText(r, Qt::AlignCenter, chas);
        p.restore();

        p.drawLine(tmpX, offsY, tmpX, offsY - mVysotaCherty);
        tmpX += mdxChas;
    }

    // рисуем чёрточки нижней части полосы, делая поправку через каждые 18 черточек
    p.setPen(mNizLiniiPen);
    int penWidth = mVerhLiniiPen.width();
    for (int i = 0; i < 8; ++i) {
        tmpX = 2 * i * mdxChas + mOffset.x();
        for (int j = 0 ; j < 18; ++j) {
            p.drawLine(tmpX, offsY + penWidth, tmpX, offsY + penWidth + mVysotaCherty);
            tmpX += mdxChast;
        }
    }

    // включаяем сглаживание
    p.setRenderHint(QPainter::Antialiasing, mKrasota);

    // создаем эффект непрерывного передвижения делений
    qreal chasCentr = mOffset.x() + (mChas - 1) * mdxChas +
                      (mChast - 1) * mdxChas / 144;
    qreal chastCentr = mOffset.x() + (mChast - 1) * mdxChast +
                       (mDolya) * mdxChast / 1296;
    qreal dolyaCentr = mOffset.x() + (mDolya) * mdxDolya;

    // рисуем стрелку часа
    p.setPen(penChasa);
    p.setBrush(brushChasa);
    QPointF chasStrelka[3] = { QPointF(chasCentr, offsY - mVysotaCherty),
                               QPointF(chasCentr - mShyrinaStrelki,
                                       offsY - mVysotaCherty - mVysotaStrelki),
                               QPointF(chasCentr + mShyrinaStrelki,
                                       offsY - mVysotaCherty - mVysotaStrelki) };
    p.drawConvexPolygon(chasStrelka, 3);

    // рисуем стрелку части
    p.setPen(penChasti);
    p.setBrush(brushChasti);
    QPointF chastStrelka[3] = { QPointF(chastCentr, offsY + penWidth + mVysotaCherty),
                                QPointF(chastCentr - mShyrinaStrelki,
                                        offsY + penWidth + mVysotaCherty + mVysotaStrelki),
                                QPointF(chastCentr + mShyrinaStrelki,
                                        offsY + penWidth + mVysotaCherty + mVysotaStrelki) };
    p.drawConvexPolygon(chastStrelka, 3);

    // подписываем стрелку текущей частью
    QString chast = QString::number(mChast);
    p.drawText(QRect(chastCentr - mNadpisFontMetrics.width(chast) / 2,
                           offsY + mVysotaCherty + mVysotaStrelki + penWidth,
                           mNadpisFontMetrics.width(chast), mNadpisFontMetrics.height()),
                     Qt::AlignCenter, chast);

    // рисуем полоску доли
    p.setPen(QPen(QColor(0, 0, 255)));
    p.drawLine(mOffset.x(), offsY, dolyaCentr, offsY);
}

void ChislovyeChasy::drawBackground(QPainter &p)
{
    p.setRenderHint(QPainter::Antialiasing, mKrasota);

    QPainterPath roundRect = roundRectPath(rect());

    int delta = mPausa ? 100 : 255;
    QBrush brush(QColor(220, 200, 240, delta));

    delta = 32 + mPausa ? 10 : 0;
    QColor semiTransparentWhite(255, 255, 255, 127 + delta);
    QColor semiTransparentBlack(0, 0, 0, 127 - delta);

    p.fillPath(roundRect, brush);

    int penWidth = 7;
    int radius = qMin(width(), height()) / 2;
    QRect r = rect();
    int diam = 2 * radius;

    QLinearGradient lgr(r.x(), r.height() - radius, r.x() + radius, r.height());
    lgr.setColorAt(0, semiTransparentWhite);
    lgr.setColorAt(1, semiTransparentBlack);
    QPen middlePen = QPen(QBrush(lgr), penWidth);
    p.setPen(middlePen);

    QPainterPath path;
    path.moveTo(r.x(), r.height() - radius);
    path.arcTo(r.x(), r.height() - diam, diam, diam, 180.0, 90.0);
    p.setClipPath(path);
    p.drawPath(path);

    lgr.setStart(r.width() - radius, r.y());
    lgr.setFinalStop(r.width(), r.y() + radius);
    lgr.setColorAt(0, semiTransparentWhite);
    lgr.setColorAt(1, semiTransparentBlack);
    middlePen.setBrush(QBrush(lgr));

    QPainterPath path2;
    path2.moveTo(r.width(), r.y() + radius);
    p.setPen(middlePen);

    path2.arcTo(r.width() - diam, r.y(), diam, diam, 0.0, 90.0);
    p.setClipPath(path2);
    p.drawPath(path2);

    QPen topPen(semiTransparentWhite, penWidth);
    QPen bottomPen(semiTransparentBlack, penWidth);

    delta = radius - 4;
    int x1 = r.x();
    int x2 = r.x() + radius;
    int x3 = r.width() - radius;
    int x4 = r.width();

    QPolygon topHalf;
    topHalf << QPoint(x1, r.y())
            << QPoint(x4 - delta, r.y())
            << QPoint(x3, r.y() + radius)
            << QPoint(x2, r.height() - radius)
            << QPoint(x1, r.height() - delta);

    p.setClipPath(roundRect);
    p.setClipRegion(topHalf, Qt::IntersectClip);
    p.setPen(topPen);
    p.drawPath(roundRect);

    QPolygon bottomHalf = topHalf;
    bottomHalf[0] = QPoint(x4, r.height());
    bottomHalf[1] = QPoint(x4, r.y() + delta);
    bottomHalf[4] = QPoint(x1 + delta, r.height());

    p.setClipPath(roundRect);
    p.setClipRegion(bottomHalf, Qt::IntersectClip);
    p.setPen(bottomPen);
    p.drawPath(roundRect);

    p.setPen(Qt::darkGray);
    p.setClipping(false);
    p.drawPath(roundRect);

    p.setRenderHint(QPainter::Antialiasing, false);
}

QPainterPath ChislovyeChasy::roundRectPath(QRect rect)
{
    int radius = qMin(rect.width(), rect.height()) / 2;
    int diam = 2 * radius;

    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);

    QPainterPath path;
    path.moveTo(x2, y1 + radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, 90.0);
    path.lineTo(x1 + radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, 90.0);
    path.lineTo(x1, y2 - radius);
    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, 90.0);
    path.lineTo(x1 + radius, y2);
    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, 90.0);
    path.closeSubpath();
    return path;
}

void ChislovyeChasy::tickChasov()
{
    mDolya += KDTIMERINTERVAL;
    if (mDolya >= 1296) {
        mDolya = KDTIMERINTERVAL;
        mChast++;
        if (mChast == 144) {
            mChast = 1;
            mChas++;
            if (mChas > 16) {
                mChas = 1;
            }
        }
    }
}

QColor ChislovyeChasy::cvetChasaDark() const
{
    return QColor(KDChetvertySutok_Cveta[mChas != 16 ? mChas / 4 : 0].second).darker(150);
}

QColor ChislovyeChasy::cvetChasaLight() const
{
    return QColor(KDChetvertySutok_Cveta[mChas != 16 ? mChas / 4 : 0].second).lighter(150);
}
