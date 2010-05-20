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
