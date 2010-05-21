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

#ifndef DK4SETTINGSFORM_H
#define DK4SETTINGSFORM_H

#include "ui/ui_dk4settingsform.h"

QT_FORWARD_DECLARE_CLASS(DK4)
QT_FORWARD_DECLARE_CLASS(SAColorButton)

class DK4SettingsForm : public QWidget, private Ui::DK4SettingsForm
{
    Q_OBJECT

public:
    DK4SettingsForm(DK4 *dk4, QWidget *parent = 0);
    ~DK4SettingsForm();

    void getSettings(bool &useAltColor, QColor &conturColor, QColor &diagonalColor);

public slots:
    void applySettings();

protected:
    void showEvent(QShowEvent *);

private slots:
    void on_mDK4ConturBackgroundPushButton_clicked();
    void on_mDK4DiagonalColorPushButton_clicked();

private:
    DK4 *mDK4;
};

#endif // DK4SETTINGSFORM_H
