#include <QColorDialog>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QPlastiqueStyle>
#include <QPushButton>
#include <QSettings>

#include "sacolorbutton.h"
#include "sakalendarsettingsdialog.h"

SAKSettingsDialog::SAKSettingsDialog(QMainWindow *mainWindow, QGraphicsScene *scene)
    : QDialog(mainWindow)
    , mMainWindow(mainWindow)
    , mScene(scene)
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
    resize(s.value("/SAKalendar/SAKalendarSettingsDialog/size").toSize());
    move(s.value("/SAKalendar/SAKalendarSettingsDialog/pos").toPoint());
}

void SAKSettingsDialog::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SAKalendarSettingsDialog/size", size());
    s.setValue("/SAKalendar/SAKalendarSettingsDialog/pos", pos());
}

void SAKSettingsDialog::apply()
{
    if (mFullScreenCheckBox->isChecked())
        mMainWindow->showFullScreen();
    else
        mMainWindow->showNormal();

    mScene->setBackgroundBrush(mSceneBackgroundPushButton->color());

    QSettings s;
    s.setValue("/SAKalendar/SAKalendarApp/FullScreen", mFullScreenCheckBox->isChecked());
    s.setValue("/SAKalendar/SAKalendarApp/PluginEffects", mEffectsCheckBox->isChecked());
    s.setValue("/SAKalendar/SAKalendarApp/BackgroundColor", mScene->backgroundBrush().color());
}
