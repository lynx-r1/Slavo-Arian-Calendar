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

#ifndef KRUGOVYIECHASYI_H
#define KRUGOVYIECHASYI_H

#include <QWidget>

class KDTimeTimer;

class KrugovyeChasy : public QWidget
{
    Q_OBJECT

public:
    KrugovyeChasy(bool app = false, QWidget *parent = 0);
    QSize sizeHint() const;

    bool hasEffects() { return mEffects; }
    bool isPaused() { return mPausa; }

public slots:
    void effects(bool effects);
    void pause(bool pause);

protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void resizeEvent(QResizeEvent *);
    bool event(QEvent *);

private slots:
    void quit();
    void setDolyu(int);
    void setChast(int);
    void setChas(int);

private:
    void readSettings();
    void writeSettings();

    void setTime(QString time);

    QList<QPixmap> mChasyKartinki;

    QPoint mDragPos;
    QPoint mPrevPos;

    KDTimeTimer *mTimer;

    int mChas;
    int mChast;
    int mDolya;

    // бегает по кругу и вращает стрелки
    int mVolchok;
    int mInterval;

    bool mPausa;
    bool mEffects;
    bool mApp;
};

#endif // KRUGOVYIECHASYI_H
