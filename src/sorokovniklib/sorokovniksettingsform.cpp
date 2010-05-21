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

#include "sorokovnik.h"

#include "sorokovniksettingsform.h"

SorokovnikSettingsForm::SorokovnikSettingsForm(Sorokovnik *sorokovnik, QWidget *parent)
    : QWidget(parent)
    , mSorokovnik(sorokovnik)
{
    setupUi(this);
}

void SorokovnikSettingsForm::getSettings(QColor &bgColor)
{
    bgColor = mSorokovnik->conturColor();
}

void SorokovnikSettingsForm::applySettings()
{
    mSorokovnik->setConturColor(mSorokovnikBackgroundPushButton->color());
}

void SorokovnikSettingsForm::showEvent(QShowEvent *e)
{
    mSorokovnikBackgroundPushButton->setColor(mSorokovnik->conturColor());
    QWidget::showEvent(e);
}

void SorokovnikSettingsForm::on_mSorokovnikBackgroundPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mSorokovnikBackgroundPushButton->color(), this,
                                          tr("Цвет тени"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mSorokovnikBackgroundPushButton->setColor(color);
}
