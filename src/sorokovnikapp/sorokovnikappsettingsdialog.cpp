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
#include <QDialogButtonBox>
#include <QGraphicsEffect>
#include <QLayout>
#include <QPlastiqueStyle>
#include <QSettings>

#include "sacolorbutton.h"
#include "sorokovniksettingsform.h"

#include "sorokovnikappsettingsdialog.h"

SorokovnikAppSettingsDialog::SorokovnikAppSettingsDialog(Sorokovnik *sorokovnik,
                                                         QGraphicsDropShadowEffect *dropShadowEffect,
                                                         QWidget *parent)
    : QDialog(parent)
    , mDropShadowEffect(dropShadowEffect)
{
    setWindowTitle(tr("Настройки"));
    mSettingsForm = new SorokovnikSettingsForm(sorokovnik);

    mDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                            QDialogButtonBox::Apply |
                                            QDialogButtonBox::Cancel);

    mShadowColorPushButton = new SAColorButton;

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QLabel(tr("Цвет тени:")));
    hboxLayout->addStretch();
    hboxLayout->addWidget(mShadowColorPushButton);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(mSettingsForm);
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(mDialogButtonBox);

    connect(mDialogButtonBox, SIGNAL(clicked(QAbstractButton*)),
            this, SLOT(dialogButtonBox_clicked(QAbstractButton*)));
    connect(mShadowColorPushButton, SIGNAL(clicked()),
            this, SLOT(shadowColorPushButton_clicked()));

    readSettings();
}

void SorokovnikAppSettingsDialog::showEvent(QShowEvent *)
{
    mShadowColorPushButton->setColor(mDropShadowEffect->color());
}

void SorokovnikAppSettingsDialog::apply()
{
    mSettingsForm->applySettings();

    QColor conturColor;
    QColor shadowColor = mShadowColorPushButton->color();

    mSettingsForm->getSettings(conturColor);

    QSettings s;
    s.setValue("/SAKalendar/SorokovnikApp/ConturColor", conturColor);
    s.setValue("/SAKalendar/SorokovnikApp/ShadowColor", shadowColor);

    mDropShadowEffect->setColor(shadowColor);
}

void SorokovnikAppSettingsDialog::closeEvent(QCloseEvent *e)
{
    writeSettings();
    QDialog::closeEvent(e);
}

void SorokovnikAppSettingsDialog::dialogButtonBox_clicked(QAbstractButton *button)
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

void SorokovnikAppSettingsDialog::shadowColorPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mShadowColorPushButton->color(),
                                          this, tr("Цвет тени"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mShadowColorPushButton->setColor(color);
}

void SorokovnikAppSettingsDialog::readSettings()
{
    QSettings s;
    restoreGeometry(s.value("/SAKalendar/SorokovnikAppSettingsDialog/Geometry").toByteArray());
}

void SorokovnikAppSettingsDialog::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SorokovnikAppSettingsDialog/Geometry", saveGeometry());
}
