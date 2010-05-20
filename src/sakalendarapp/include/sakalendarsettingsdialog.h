#ifndef SAKALENDARSETTINGSDIALOG_H
#define SAKALENDARSETTINGSDIALOG_H

#include "ui_sakalendarsettingsdialog.h"

QT_FORWARD_DECLARE_CLASS(QMainWindow)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)

class SAKSettingsDialog : public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

public:
    SAKSettingsDialog(QMainWindow *mainWindow, QGraphicsScene *scene);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_mSceneBackgroundPushButton_clicked();

private:
    void readSettings();
    void writeSettings();

    void apply();

private:
    QMainWindow *mMainWindow;
    QGraphicsScene *mScene;
};

#endif // SAKALENDARSETTINGSDIALOG_H
