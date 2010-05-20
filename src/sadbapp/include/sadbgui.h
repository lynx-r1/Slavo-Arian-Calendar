#ifndef SADBGUI_H
#define SADBGUI_H

#include <QSqlDatabase>

#include "ui/ui_sadbgui.h"

QT_FORWARD_DECLARE_CLASS(DK4DB)
QT_FORWARD_DECLARE_CLASS(SorokovnikDB)

class SADBGui : public QMainWindow, private Ui::SADBGui {
    Q_OBJECT
public:
    SADBGui(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void daySelected(int den, int sorokovnik);
    void kzhizniSelected(int kzhizni);
    void setLeto();
    void showAbout();

    void on_mSrkEditPushButton_clicked(bool);
    void on_mSrkSubmitPushButton_clicked();
    void on_mSrkOpenDBPushButton_clicked();
    void on_mSrkOpenDBToolButton_clicked();
    void on_mSrkCreateDBPushButton_clicked();
    void on_mSrkPopulateDBPushButton_clicked();

    void on_mSrkVesshchieSnyCheckBox_toggled(bool);
    void on_mSrkRoditelskieDniCheckBox_toggled(bool);
    void on_mSrkDniPamyatiCheckBox_toggled(bool);
    void on_mSrkVazhniePrazdnikiCheckBox_toggled(bool);

    void on_mSrkSimpleDayRadioButton_toggled(bool);
    void on_mSrkSvyatiiPostRadioButton_toggled(bool);
    void on_mSrkDazhdbogPostRadioButton_toggled(bool);
    void on_mSrkChistyiPostRadioButton_toggled(bool);
    void on_mSrkVelikiiPostRadioButton_toggled(bool);
    void on_mSrkSvetlyiPostRadioButton_toggled(bool);

    void on_mDK4EditPushButton_clicked(bool);
    void on_mDK4SubmitPushButton_clicked();
    void on_mDK4OpenDBPushButton_clicked();
    void on_mDK4OpenDBToolButton_clicked();
    void on_mDK4CreateDBPushButton_clicked();
    void on_mDK4PopulateDBPushButton_clicked();

private:
    void readSettings();
    void writeSettings();

    void createSorokovnik();
    void createDK4();
    void setupConnections();
    void init();

    void setRangDnya(int rang);
    void setRangDnyaSorokovnika(int rang);
    void resetRangDnya(bool set, int setRang, int &rang);
    void resetInfoWidgets();

private:
    QStringList mTables;

    QString mPrazdnikiDbConnectionName;
    QString mOpisanieLetDbConnectionName;

    SorokovnikDB *mSorokovnik;
    DK4DB *mDK4;

    int mKZhizni;
    int mDen;
    int mSorok;
    int mRangDnya;
    int mMessageTimeout;
};

#endif // SADBGUI_H
