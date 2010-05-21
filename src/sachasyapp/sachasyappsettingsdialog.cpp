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

#include <QColorDialog>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGraphicsEffect>
#include <QLabel>
#include <QLayout>
#include <QSettings>

#include "sacolorbutton.h"
#include "sachasysettingsform.h"

#include "sachasyappsettingsdialog.h"

SAChasyAppSettingsDialog::SAChasyAppSettingsDialog(SAChasy *sachasy,
                                                   QGraphicsDropShadowEffect *dropShadowEffect,
                                                   QWidget *parent)
    : QDialog(parent)
    , mDropShadowEffect(dropShadowEffect)
{
    setWindowTitle(tr("Настройки"));

    mWindowStaysOnTopCheckBox = new QCheckBox(tr("Поверх всех окон"), this);
    mSettingsForm = new SAChasySettingsForm(sachasy);

    mDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                            QDialogButtonBox::Apply |
                                            QDialogButtonBox::Cancel);

    mShadowColorButton = new SAColorButton;

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QLabel(tr("Цвет тени:")));
    hboxLayout->addStretch();
    hboxLayout->addWidget(mShadowColorButton);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(mWindowStaysOnTopCheckBox);
    vboxLayout->addWidget(mSettingsForm);
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(mDialogButtonBox);

    connect(mDialogButtonBox, SIGNAL(clicked(QAbstractButton*)),
            this, SLOT(dialogButtonBox_clicked(QAbstractButton*)));
    connect(mShadowColorButton, SIGNAL(clicked()),
            this, SLOT(shadowColorPushButton_clicked()));

    readSettings();
}

void SAChasyAppSettingsDialog::showEvent(QShowEvent *)
{
    mShadowColorButton->setColor(mDropShadowEffect->color());
}

void SAChasyAppSettingsDialog::closeEvent(QCloseEvent *e)
{
    writeSettings();
    QDialog::closeEvent(e);
}

void SAChasyAppSettingsDialog::dialogButtonBox_clicked(QAbstractButton *button)
{
    writeSettings();
    if (mDialogButtonBox->buttonRole(button) == QDialogButtonBox::ApplyRole) {
        apply();
    } else if (mDialogButtonBox->buttonRole(button) == QDialogButtonBox::AcceptRole) {
        apply();
        accept();
    } else if (mDialogButtonBox->buttonRole(button) == QDialogButtonBox::RejectRole) {
        reject();
    }
}

void SAChasyAppSettingsDialog::shadowColorPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mShadowColorButton->color(), this,
                                          tr("Цвет тени"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mShadowColorButton->setColor(color);
}

void SAChasyAppSettingsDialog::readSettings()
{
    QSettings s;
    restoreGeometry(s.value("/SAKalendar/SAChasyAppSettingsDialog/Geometry").toByteArray());
    mWindowStaysOnTopCheckBox->setChecked(s.value("/SAKalendar/SAChasyApp/StayOnTop", false).toBool());
}

void SAChasyAppSettingsDialog::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SAChasyAppSettingsDialog/Geometry", saveGeometry());
}

void SAChasyAppSettingsDialog::apply()
{
    mSettingsForm->applySettings();

    bool pravSutKur, dStrelka, ignoreDST, simpleView, showDate;
    int updateInterval, side;
    QColor backgroundColor;
    QColor shadowColor = mShadowColorButton->color();

    mSettingsForm->getSettings(pravSutKur, dStrelka, ignoreDST, simpleView,
                               showDate, updateInterval, backgroundColor, side);

    QSettings s;
    s.beginGroup("/SAKalendar/SAChasyApp");
    s.setValue("PravSutKrug", pravSutKur);
    s.setValue("ShowDoly", dStrelka);
    s.setValue("IgnoreDST", ignoreDST);
    s.setValue("SimpleView", simpleView);
    s.setValue("ShowDate", showDate);
    s.setValue("UpdateInterval", updateInterval);
    s.setValue("BackgroundColor", backgroundColor);
    s.setValue("SideSize", side);
    s.setValue("StayOnTop", mWindowStaysOnTopCheckBox->isChecked());
    s.setValue("ShadowColor", shadowColor);
    s.endGroup();

    QWidget *parentW = parentWidget();
    parentW->resize(side, side);
    parentW->setWindowFlags(mWindowStaysOnTopCheckBox->isChecked() ?
                            parentW->windowFlags() | Qt::WindowStaysOnTopHint
                                : parentW->windowFlags() ^ Qt::WindowStaysOnTopHint);
    parentW->show();

    mDropShadowEffect->setColor(shadowColor);
}
