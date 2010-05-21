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

#ifndef DK4APP_H
#define DK4APP_H

#include "ui_dk4app.h"

QT_FORWARD_DECLARE_CLASS(DK4)
QT_FORWARD_DECLARE_CLASS(DK4AppSettingsDialog)

class DK4App : public QMainWindow, private Ui::DK4App {
    Q_OBJECT
public:
    DK4App(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void showAbout();

    void kzhizniSelected(int leto);

private:
    void readSettings();
    void writeSettings();

    void createDK4();
    void setupConnections();
    void clearInfoGui();

    QString colorText(const QString &color);

    DK4 *mDK4;
    DK4AppSettingsDialog *mSettingsDialog;

    int mMessageTimeout;
};

#endif // DK4APP_H
