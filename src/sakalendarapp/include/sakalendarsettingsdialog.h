#ifndef SAKALENDARSETTINGSDIALOG_H
#define SAKALENDARSETTINGSDIALOG_H

#include "ui_sakalendarsettingsdialog.h"

QT_FORWARD_DECLARE_CLASS(QMainWindow)
QT_FORWARD_DECLARE_CLASS(QGraphicsItem)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)

QT_FORWARD_DECLARE_CLASS(SAGraphicsItemGroup)
QT_FORWARD_DECLARE_CLASS(SAGraphicsToolBar)

class SAKSettingsDialog : public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

public:
    SAKSettingsDialog(QMainWindow *mainWindow, QGraphicsScene *scene,
                      SAGraphicsToolBar *mainToolBar, SAGraphicsItemGroup *logo);

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

    SAGraphicsToolBar *mMainToolBar;
    SAGraphicsItemGroup *mLogo;
};

#endif // SAKALENDARSETTINGSDIALOG_H
