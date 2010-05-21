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

#include "sachasy.h"

#include "sachasysettingsform.h"

SAChasySettingsForm::SAChasySettingsForm(SAChasy *sachasy, QWidget *parent)
    : QWidget(parent)
    , mSAChasy(sachasy)
{
    setupUi(this);
}

SAChasySettingsForm::~SAChasySettingsForm()
{
}

void SAChasySettingsForm::getSettings(bool &pravSutKrug, bool &dStrelka,
                                      bool &ignoreDST, bool &simpleView,
                                      bool &showDate, int &updateInterval,
                                      QColor &bgColor, int &side)
{
    pravSutKrug = mSAChasy->pravSutKrug();
    dStrelka = mSAChasy->dolevayaStrelka();
    ignoreDST = mSAChasy->ignoreDST();
    simpleView = mSAChasy->simpleView();
    showDate = mSAChasy->showDate();
    updateInterval = mSAChasy->updateInterval();
    bgColor = mSAChasy->backgroundColor();
    side = mSAChasy->size().width();
}

void SAChasySettingsForm::applySettings()
{
    mSAChasy->setPravSutKrug(mPravSutKrugCheckBox->isChecked());
    mSAChasy->setDolevayaStrelka(mShowDolyCheckBox->isChecked());
    mSAChasy->setIgnoreDST(mIgnoreDSTCheckBox->isChecked());
    mSAChasy->setSimpleView(mSimpleCheckBox->isChecked());
    mSAChasy->setShowDate(mShowDateCheckBox->isChecked());
    mSAChasy->setUpdateInterval(mUpdateIntervalSpinBox->value());
    mSAChasy->setBackgroundColor(mSAChasyBgPushButton->color());
    int side = mSAChasySizeSpinBox->value();
    mSAChasy->setFixedSize(QSizeF(side, side));
}

void SAChasySettingsForm::showEvent(QShowEvent *e)
{
    mPravSutKrugCheckBox->setChecked(mSAChasy->pravSutKrug());
    mShowDolyCheckBox->setChecked(mSAChasy->dolevayaStrelka());
    mIgnoreDSTCheckBox->setChecked(mSAChasy->ignoreDST());
    mSimpleCheckBox->setChecked(mSAChasy->simpleView());
    mShowDateCheckBox->setChecked(mSAChasy->showDate());
    mUpdateIntervalSpinBox->setValue(mSAChasy->updateInterval());
    mSAChasySizeSpinBox->setValue(mSAChasy->size().width());
    mSAChasyBgPushButton->setColor(mSAChasy->backgroundColor());

    QWidget::showEvent(e);
}

void SAChasySettingsForm::on_mSAChasyBgPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mSAChasyBgPushButton->color(), this,
                                          tr("Цвет тени"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mSAChasyBgPushButton->setColor(color);
}
