#ifndef SOROKOVNIKAPPSETTINGSDIALOG_H
#define SOROKOVNIKAPPSETTINGSDIALOG_H

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QGraphicsDropShadowEffect)

QT_FORWARD_DECLARE_CLASS(SAColorButton)
QT_FORWARD_DECLARE_CLASS(Sorokovnik)
QT_FORWARD_DECLARE_CLASS(SorokovnikSettingsForm)

class SorokovnikAppSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SorokovnikAppSettingsDialog(Sorokovnik *sorokovnik,
                                QGraphicsDropShadowEffect *dropShadowEffect,
                                QWidget *parent = 0);

    void apply();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void dialogButtonBox_clicked(QAbstractButton *button);
    void shadowColorPushButton_clicked();

private:
    void readSettings();
    void writeSettings();

private:
    QDialogButtonBox *mDialogButtonBox;
    QGraphicsDropShadowEffect *mDropShadowEffect;

    SAColorButton *mShadowColorPushButton;
    SorokovnikSettingsForm *mSettingsForm;
};

#endif // SOROKOVNIKAPPSETTINGSDIALOG_H
