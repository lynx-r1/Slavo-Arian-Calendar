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
#include <QPlastiqueStyle>

#include "sacolorbutton.h"
#include "dk4.h"

#include "dk4settingsform.h"

DK4SettingsForm::DK4SettingsForm(DK4 *dk4, QWidget *parent)
    : QWidget(parent),
    mDK4(dk4)
{
    setupUi(this);
}

DK4SettingsForm::~DK4SettingsForm()
{
}

void DK4SettingsForm::getSettings(bool &useAltColor, QColor &conturColor, QColor &diagonalColor)
{
    useAltColor = mDK4->useAltColor();
    conturColor = mDK4->conturColor();
    diagonalColor = mDK4->diagonalColor();
}

void DK4SettingsForm::applySettings()
{
    if (mAltColorCheckBox->isChecked() != mDK4->useAltColor())
        mDK4->setAltColor(mAltColorCheckBox->isChecked(), true);
    mDK4->setConturColor(mDK4ConturBackgroundPushButton->color());
    mDK4->setDiagonalColor(mDK4DiagonalColorPushButton->color());
}

void DK4SettingsForm::showEvent(QShowEvent *e)
{
    mAltColorCheckBox->setChecked(mDK4->useAltColor());
    mDK4ConturBackgroundPushButton->setColor(mDK4->conturColor());
    mDK4DiagonalColorPushButton->setColor(mDK4->diagonalColor());

    QWidget::showEvent(e);
}

void DK4SettingsForm::on_mDK4ConturBackgroundPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mDK4ConturBackgroundPushButton->color(),
                                          this, tr("Цвет тени"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mDK4ConturBackgroundPushButton->setColor(color);
}

void DK4SettingsForm::on_mDK4DiagonalColorPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mDK4DiagonalColorPushButton->color(),
                                          this, tr("Цвет тени"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mDK4DiagonalColorPushButton->setColor(color);
}
