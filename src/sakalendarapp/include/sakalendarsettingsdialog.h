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

#ifndef SAKALENDARSETTINGSDIALOG_H
#define SAKALENDARSETTINGSDIALOG_H

#include "ui_sakalendarsettingsdialog.h"

QT_FORWARD_DECLARE_CLASS(QMainWindow)
QT_FORWARD_DECLARE_CLASS(QGraphicsItem)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)

QT_FORWARD_DECLARE_CLASS(SAGraphicsItemGroup)
QT_FORWARD_DECLARE_CLASS(SAGraphicsToolBar)

class SAKSettingsDialog : public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

public:
    SAKSettingsDialog(QMainWindow *mainWindow, QGraphicsScene *scene,
                      SAGraphicsToolBar *mainToolBar, SAGraphicsItemGroup *logo);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_mSceneBackgroundPushButton_clicked();

private:
    void readSettings();
    void writeSettings();

    void apply();

private:
    QMainWindow *mMainWindow;
    QGraphicsScene *mScene;

    SAGraphicsToolBar *mMainToolBar;
    SAGraphicsItemGroup *mLogo;
};

#endif // SAKALENDARSETTINGSDIALOG_H
