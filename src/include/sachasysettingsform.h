#ifndef SACHASYSETTINGSFORM_H
#define SACHASYSETTINGSFORM_H

#include "ui/ui_sachasysettingsform.h"

QT_FORWARD_DECLARE_CLASS(SAChasy)

class SAChasySettingsForm : public QWidget, private Ui::SAChasySettingsForm
{
    Q_OBJECT

public:
    SAChasySettingsForm(SAChasy *sachasy, QWidget *parent = 0);
    ~SAChasySettingsForm();

    void getSettings(bool &pravSutKrug, bool &dStrelka, bool &ignoreDST,
                     bool &simpleView, bool &showDate,
                     int &updateInterval, QColor &bgColor,
                     int &side);

public slots:
    void applySettings();

protected:
    void showEvent(QShowEvent *);

private slots:
    void on_mSAChasyBgPushButton_clicked();

private:
    SAChasy *mSAChasy;
};

#endif // SACHASYSETTINGSFORM_H
