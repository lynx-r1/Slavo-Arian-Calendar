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

#ifndef CHISLOVYIECHASYI_H
#define CHISLOVYIECHASYI_H

#include <QWidget>
#include <QMap>
#include <QPen>

#include "kd.h"

class ChislovyeChasy : public QWidget {
    Q_OBJECT

public:
    ChislovyeChasy(int chas, int chast, int dolya, bool app = false, QWidget *parent = 0);
    QSize sizeHint() const;

    void setPaused(bool pause) { mPausa = pause; }
    bool isPaused() { return mPausa; }

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void resizeEvent(QResizeEvent *);
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *);
    bool event(QEvent *);

private slots:
    void quit();

private:
    void readSettings();
    void writeSettings();
    void init();

    void updateSize(QSize size);
    void chislovayLiniya(QPainter &p, QPen verhLiniiPen, QPen nizLiniiPen,
                         QFont nadpisFont);
    void drawBackground(QPainter &p);
    QPainterPath roundRectPath(QRect r);

    void tickChasov();
    QColor cvetChasaDark() const;
    QColor cvetChasaLight() const;

    QList<QPixmap> mChasyKartinki;

    QRect mZaglavyeRect;
    QRect mChertyRezyChasRect;
    QRect mLineinyeChasyRect;

    QPoint mDragPos;

    QPen mVerhLiniiPen;
    QPen mNizLiniiPen;

    QFont mNadpisFont;
    QFont mZaglavnFont;

    QFontMetrics *mNadpisFontMetrics;
    QFontMetrics *mZaglavnFontMetrics;

    int mChas;
    int mChast;
    int mDolya;

    int mVysotaStrelki;
    int mShyrinaStrelki;
    int mVysotaCherty;

    QPoint mOffset;

    qreal mdxChas;
    qreal mdxChast;
    qreal mdxDolya;

    bool mPausa;
    bool mKrasota;
    bool mApp;
};

#endif // CHISLOVYIECHASYI_H
