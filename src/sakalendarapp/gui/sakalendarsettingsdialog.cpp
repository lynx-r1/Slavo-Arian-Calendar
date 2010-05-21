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
#include <QCloseEvent>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QPlastiqueStyle>
#include <QPushButton>
#include <QSettings>

#include "sagraphicsitemgroup.h"
#include "sagraphicstoolbar.h"
#include "sacolorbutton.h"

#include "sakalendarsettingsdialog.h"

SAKSettingsDialog::SAKSettingsDialog(QMainWindow *mainWindow, QGraphicsScene *scene,
                                     SAGraphicsToolBar *mainToolBar, SAGraphicsItemGroup *logo)
    : QDialog(mainWindow)
    , mMainWindow(mainWindow)
    , mScene(scene)
    , mMainToolBar(mainToolBar)
    , mLogo(logo)
{
    setupUi(this);
}

void SAKSettingsDialog::showEvent(QShowEvent *e)
{
    QSettings s;
    bool pluginEffects = s.value("/SAKalendar/SAKalendarApp/PluginEffects").toBool();

    mFullScreenCheckBox->setChecked(mMainWindow->isFullScreen());
    mSceneBackgroundPushButton->setColor(mScene->backgroundBrush().color());
    mEffectsCheckBox->setChecked(pluginEffects);
    mShowLogoCheckBox->setChecked(mLogo->opacity() == 1);
    mHideMainToolBarCheckBox->setChecked(mMainToolBar->hideOnHoverLeave());

    QDialog::showEvent(e);
    readSettings();
}

void SAKSettingsDialog::closeEvent(QCloseEvent *e)
{
    writeSettings();
    e->accept();
}

void SAKSettingsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    writeSettings();
    if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole) {
        apply();
    } else if (buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole) {
        apply();
        accept();
    } else if (buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole) {
        reject();
    }
}

void SAKSettingsDialog::on_mSceneBackgroundPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mSceneBackgroundPushButton->color(), this,
                                          tr("Цвет фона"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mSceneBackgroundPushButton->setColor(color);
}

void SAKSettingsDialog::readSettings()
{
    QSettings s;
    restoreGeometry(s.value("/SAKalendar/SAKalendarSettingsDialog/Geometry").toByteArray());
}

void SAKSettingsDialog::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SAKalendarSettubgsDialog/Geometry", saveGeometry());
}

void SAKSettingsDialog::apply()
{
    QSettings s;
    if (mFullScreenCheckBox->isChecked()) {
        mMainWindow->showFullScreen();
    } else if (mMainWindow->isFullScreen()) {
        mMainWindow->showNormal();
    }

    mScene->setBackgroundBrush(mSceneBackgroundPushButton->color());
    mMainToolBar->setHideOnHoverLeave(mHideMainToolBarCheckBox->isChecked());

    QPropertyAnimation *anim = new QPropertyAnimation(mLogo, "opacity");
    anim->setDuration(1000);
    anim->setEasingCurve(mShowLogoCheckBox->isChecked() ? QEasingCurve::OutExpo :
                         QEasingCurve::Linear);
    anim->setEndValue(mShowLogoCheckBox->isChecked());
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    s.beginGroup("/SAKalendar/SAKalendarApp");
    s.setValue("FullScreen", mFullScreenCheckBox->isChecked());
    s.setValue("PluginEffects", mEffectsCheckBox->isChecked());
    s.setValue("BackgroundColor", mScene->backgroundBrush().color());
    s.setValue("ShowLogo", mShowLogoCheckBox->isChecked());
    s.setValue("HideMainToolBar", mHideMainToolBarCheckBox->isChecked());
    s.endGroup();
}
