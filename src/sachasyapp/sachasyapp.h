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

#ifndef SACHASYAPP_H
#define SACHASYAPP_H

#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QLabel)

QT_FORWARD_DECLARE_CLASS(SAChasy)
QT_FORWARD_DECLARE_CLASS(SAChasyAppSettingsDialog)

class SAChasyApp : public QGraphicsView
{
    Q_OBJECT

public:
    SAChasyApp(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void showAbout();

private:
    void readSettings();
    void writeSettings();

    void createActions();
    void createSAChasy();

private:
    QAction *mSettingsAction;
    QAction *mAboutAction;
    QAction *mQuitAction;

    QLabel *mDateLabel;

    QPoint mDragPosition;

    QColor mBgColor;
    QColor mShadowColor;

    SAChasyAppSettingsDialog *mSettingsDialog;
    SAChasy *mSAChasy;
};

#endif // SACHASYAPP_H
