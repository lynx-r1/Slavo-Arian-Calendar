#ifndef SACHASYAPPSETTINGSDIALOG_H
#define SACHASYAPPSETTINGSDIALOG_H

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QCheckBox)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QGraphicsDropShadowEffect)

QT_FORWARD_DECLARE_CLASS(SAChasy)
QT_FORWARD_DECLARE_CLASS(SAChasySettingsForm)
QT_FORWARD_DECLARE_CLASS(SAColorButton)

class SAChasyAppSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SAChasyAppSettingsDialog(SAChasy *sachasy, QGraphicsDropShadowEffect *dropShadowEffect,
                             QWidget *parent);

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
    QGraphicsDropShadowEffect *mDropShadowEffect;
    QDialogButtonBox *mDialogButtonBox;
    QCheckBox *mWindowStaysOnTopCheckBox;

    SAColorButton *mShadowColorButton;
    SAChasySettingsForm *mSettingsForm;
};

#endif // SACHASYAPPSETTINGSDIALOG_H
