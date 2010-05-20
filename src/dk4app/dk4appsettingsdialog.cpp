#include <QColorDialog>
#include <QDialogButtonBox>
#include <QPlastiqueStyle>
#include <QGraphicsEffect>
#include <QLayout>
#include <QSettings>

#include "sacolorbutton.h"
#include "dk4settingsform.h"

#include "dk4appsettingsdialog.h"

DK4AppSettingsDialog::DK4AppSettingsDialog(DK4 *dk4, QGraphicsDropShadowEffect *dropShadowEffect,
                                           QWidget *parent)
    : QDialog(parent)
    , mDropShadowEffect(dropShadowEffect)
{
    setWindowTitle(tr("Настройки"));
    mSettingsForm = new DK4SettingsForm(dk4);

    mDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                            QDialogButtonBox::Apply |
                                            QDialogButtonBox::Cancel);

    mShadowColorButton = new SAColorButton;

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QLabel(tr("Цвет тени:")));
    hboxLayout->addStretch();
    hboxLayout->addWidget(mShadowColorButton);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(mSettingsForm);
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(mDialogButtonBox);

    connect(mDialogButtonBox, SIGNAL(clicked(QAbstractButton*)),
            this, SLOT(dialogButtonBox_clicked(QAbstractButton*)));
    connect(mShadowColorButton, SIGNAL(clicked()),
            this, SLOT(shadowColorPushButton_clicked()));

    readSettings();
}

void DK4AppSettingsDialog::showEvent(QShowEvent *)
{
    mShadowColorButton->setColor(mDropShadowEffect->color());
}

void DK4AppSettingsDialog::apply()
{
    mSettingsForm->applySettings();

    bool useAltColor;
    QColor conturColor;
    QColor diagonalColor;
    QColor shadowColor = mShadowColorButton->color();

    mSettingsForm->getSettings(useAltColor, conturColor, diagonalColor);

    QSettings s;
    s.beginGroup("/SAKalendar/DK4App");
    s.setValue("AltColor", useAltColor);
    s.setValue("ConturColor", conturColor);
    s.setValue("DiagonalColor", diagonalColor);
    s.setValue("ShadowColor", shadowColor);
    s.endGroup();

    mDropShadowEffect->setColor(shadowColor);
}

void DK4AppSettingsDialog::closeEvent(QCloseEvent *e)
{
    writeSettings();
    QDialog::closeEvent(e);
}

void DK4AppSettingsDialog::dialogButtonBox_clicked(QAbstractButton *button)
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

void DK4AppSettingsDialog::shadowColorPushButton_clicked()
{
    QColor color = QColorDialog::getColor(mShadowColorButton->color(), this,
                                          tr("Цвет тени"), QColorDialog::ShowAlphaChannel);
    if (color.isValid())
        mShadowColorButton->setColor(color);
}

void DK4AppSettingsDialog::readSettings()
{
    QSettings s;
    resize(s.value("/SAKalendar/DK4AppSettingsDialog/size").toSize());
    move(s.value("/SAKalendar/DK4AppSettingsDialog/pos").toPoint());
}

void DK4AppSettingsDialog::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/DK4AppSettingsDialog/size", size());
    s.setValue("/SAKalendar/DK4AppSettingsDialog/pos", pos());
}
