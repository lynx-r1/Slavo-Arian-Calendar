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

    s.beginGroup("/SAKalendar/SAKalendarApp");
    s.setValue("FullScreen", mFullScreenCheckBox->isChecked());
    s.setValue("PluginEffects", mEffectsCheckBox->isChecked());
    s.setValue("BackgroundColor", mScene->backgroundBrush().color());
    s.endGroup();
}
