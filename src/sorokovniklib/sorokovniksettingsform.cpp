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
