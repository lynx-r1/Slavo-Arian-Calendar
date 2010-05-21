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

#ifndef SOROKOVNIKAPPSETTINGSDIALOG_H
#define SOROKOVNIKAPPSETTINGSDIALOG_H

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QGraphicsDropShadowEffect)

QT_FORWARD_DECLARE_CLASS(SAColorButton)
QT_FORWARD_DECLARE_CLASS(Sorokovnik)
QT_FORWARD_DECLARE_CLASS(SorokovnikSettingsForm)

class SorokovnikAppSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SorokovnikAppSettingsDialog(Sorokovnik *sorokovnik,
                                QGraphicsDropShadowEffect *dropShadowEffect,
                                QWidget *parent = 0);

    void apply();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void dialogButtonBox_clicked(QAbstractButton *button);
    void shadowColorPushButton_clicked();

private:
    void readSettings();
    void writeSettings();

private:
    QDialogButtonBox *mDialogButtonBox;
    QGraphicsDropShadowEffect *mDropShadowEffect;

    SAColorButton *mShadowColorPushButton;
    SorokovnikSettingsForm *mSettingsForm;
};

#endif // SOROKOVNIKAPPSETTINGSDIALOG_H
