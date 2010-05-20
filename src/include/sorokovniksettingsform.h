#ifndef SOROKOVNIKSETTINGSFORM_H
#define SOROKOVNIKSETTINGSFORM_H

#include "ui/ui_sorokovniksettingsform.h"

QT_FORWARD_DECLARE_CLASS(Sorokovnik)

class SorokovnikSettingsForm : public QWidget, private Ui::SorokovnikSettingsForm
{
    Q_OBJECT

public:
    SorokovnikSettingsForm(Sorokovnik *sorokovnik, QWidget *parent = 0);

    void getSettings(QColor &bgColor);

public slots:
    void applySettings();

protected:
    void showEvent(QShowEvent *);

private slots:
    void on_mSorokovnikBackgroundPushButton_clicked();

private:
    Sorokovnik *mSorokovnik;
};

#endif // SOROKOVNIKSETTINGSFORM_H
