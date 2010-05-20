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
