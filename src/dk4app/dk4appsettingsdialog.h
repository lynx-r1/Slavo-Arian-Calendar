#ifndef DK4APPSETTINGSDIALOG_H
#define DK4APPSETTINGSDIALOG_H

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QGraphicsDropShadowEffect)

QT_FORWARD_DECLARE_CLASS(DK4)
QT_FORWARD_DECLARE_CLASS(DK4SettingsForm)
QT_FORWARD_DECLARE_CLASS(SAColorButton)

class DK4AppSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    DK4AppSettingsDialog(DK4 *dk4, QGraphicsDropShadowEffect *dropShadowEffect,
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

    SAColorButton *mShadowColorButton;
    DK4SettingsForm *mSettingsForm;
};

#endif // DK4APPSETTINGSDIALOG_H
