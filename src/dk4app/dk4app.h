#ifndef DK4APP_H
#define DK4APP_H

#include "ui_dk4app.h"

QT_FORWARD_DECLARE_CLASS(DK4)
QT_FORWARD_DECLARE_CLASS(DK4AppSettingsDialog)

class DK4App : public QMainWindow, private Ui::DK4App {
    Q_OBJECT
public:
    DK4App(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void showAbout();

    void kzhizniSelected(int leto);

private:
    void readSettings();
    void writeSettings();

    void createDK4();
    void setupConnections();
    void clearInfoGui();

    QString colorText(const QString &color);

    DK4 *mDK4;
    DK4AppSettingsDialog *mSettingsDialog;

    int mMessageTimeout;
};

#endif // DK4APP_H
