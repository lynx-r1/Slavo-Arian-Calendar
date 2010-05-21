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

#ifndef SOROKOVNIKAPP_H
#define SOROKOVNIKAPP_H

#include <QSqlDatabase>

#include "ui_sorokovnikapp.h"

QT_FORWARD_DECLARE_CLASS(Sorokovnik)
QT_FORWARD_DECLARE_CLASS(SorokovnikAppSettingsDialog)

class SorokovnikApp : public QMainWindow, private Ui::SorokovnikApp
{
    Q_OBJECT

public:
    SorokovnikApp(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void showAbout();

    void daySelected(int den, int sorokovnik, int leto);
    void populateZametkamiLeto(int leto);
    void showPrazdnikInfo(bool show, const QString &info);

private:
    void readSettings();
    void writeSettings();

    void createSorokovnik();
    void setupConnections();

private:
    QSqlDatabase mZametkiDB;
    QString mZametkiDBName;

    Sorokovnik *mSorokovnik;
    SorokovnikAppSettingsDialog *mSettingsDialog;

    int mDen;
    int mSorok;
    int mLeto;
};

#endif // SOROKOVNIKAPP_H
