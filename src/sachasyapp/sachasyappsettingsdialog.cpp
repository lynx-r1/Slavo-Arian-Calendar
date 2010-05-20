#include <QColorDialog>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGraphicsEffect>
#include <QLabel>
#include <QLayout>
#include <QSettings>

#include "sacolorbutton.h"
#include "sachasysettingsform.h"

#include "sachasyappsettingsdialog.h"

SAChasyAppSettingsDialog::SAChasyAppSettingsDialog(SAChasy *sachasy,
                                                   QGraphicsDropShadowEffect *dropShadowEffect,
                                                   QWidget *parent)
    : QDialog(parent)
    , mDropShadowEffect(dropShadowEffect)
{
    setWindowTitle(tr("Настройки"));

    mWindowStaysOnTopCheckBox = new QCheckBox(tr("Поверх всех окон"), this);
    mSettingsForm = new SAChasySettingsForm(sachasy);

    mDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                            QDialogButtonBox::Apply |
                                            QDialogButtonBox::Cancel);

    mShadowColorButton = new SAColorButton;

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QLabel(tr("Цвет тени:")));
    hboxLayout->addStretch();
    hboxLayout->addWidget(mShadowColorButton);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(mWindowStaysOnTopCheckBox);
    vboxLayout->addWidget(mSettingsForm);
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(mDialogButtonBox);

    connect(mDialogButtonBox, SIGNAL(clicked(QAbstractButton*)),
            this, SLOT(dialogButtonBox_clicked(QAbstractButton*)));
    connect(mShadowColorButton, SIGNAL(clicked()),
            this, SLOT(shadowColorPushButton_clicked()));

    readSettings();
}

void SAChasyAppSettingsDialog::showEvent(QShowEvent *)
{
    mShadowColorButton->setColor(mDropShadowEffect->color());
}

void SAChasyAppSettingsDialog::closeEvent(QCloseEvent *e)
{
    writeSettings();
    QDialog::closeEvent(e);
}

void SAChasyAppSettingsDialog::dialogButtonBox_clicked(QAbstractButton *button)
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

void SAChasyAppSettingsDialog::shadowColorPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mShadowColorButton->color(), this,
                                          tr("Цвет тени"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mShadowColorButton->setColor(color);
}

void SAChasyAppSettingsDialog::readSettings()
{
    QSettings s;
    resize(s.value("/SAKalendar/SAChasyAppSettingsDialog/size").toSize());
    move(s.value("/SAKalendar/SAChasyAppSettingsDialog/pos").toPoint());

    mWindowStaysOnTopCheckBox->setChecked(s.value("/SAKalendar/SAChasyApp/StayOnTop", false).toBool());
}

void SAChasyAppSettingsDialog::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SAChasyAppSettingsDialog/size", size());
    s.setValue("/SAKalendar/SAChasyAppSettingsDialog/pos", pos());
}

void SAChasyAppSettingsDialog::apply()
{
    mSettingsForm->applySettings();

    bool pravSutKur, dStrelka, ignoreDST, simpleView, showDate;
    int updateInterval, side;
    QColor backgroundColor;
    QColor shadowColor = mShadowColorButton->color();

    mSettingsForm->getSettings(pravSutKur, dStrelka, ignoreDST, simpleView,
                               showDate, updateInterval, backgroundColor, side);

    QSettings s;
    s.beginGroup("/SAKalendar/SAChasyApp");
    s.setValue("PravSutKrug", pravSutKur);
    s.setValue("ShowDoly", dStrelka);
    s.setValue("IgnoreDST", ignoreDST);
    s.setValue("SimpleView", simpleView);
    s.setValue("ShowDate", showDate);
    s.setValue("UpdateInterval", updateInterval);
    s.setValue("BackgroundColor", backgroundColor);
    s.setValue("SideSize", side);
    s.setValue("StayOnTop", mWindowStaysOnTopCheckBox->isChecked());
    s.setValue("ShadowColor", shadowColor);
    s.endGroup();

    QWidget *parentW = parentWidget();
    parentW->resize(side, side);
    parentW->setWindowFlags(mWindowStaysOnTopCheckBox->isChecked() ?
                            parentW->windowFlags() | Qt::WindowStaysOnTopHint
                                : parentW->windowFlags() ^ Qt::WindowStaysOnTopHint);
    parentW->show();

    mDropShadowEffect->setColor(shadowColor);
}
