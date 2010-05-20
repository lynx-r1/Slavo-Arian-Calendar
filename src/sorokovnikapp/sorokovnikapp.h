#ifndef SOROKOVNIKAPP_H
#define SOROKOVNIKAPP_H

#include <QSqlDatabase>

#include "ui_sorokovnikapp.h"

QT_FORWARD_DECLARE_CLASS(Sorokovnik)
QT_FORWARD_DECLARE_CLASS(SorokovnikAppSettingsDialog)

class SorokovnikApp : public QMainWindow, private Ui::SorokovnikApp
{
    Q_OBJECT

public:
    SorokovnikApp(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void showAbout();

    void daySelected(int den, int sorokovnik, int leto);
    void populateZametkamiLeto(int leto);
    void showPrazdnikInfo(bool show, const QString &info);

private:
    void readSettings();
    void writeSettings();

    void createSorokovnik();
    void setupConnections();

private:
    QSqlDatabase mZametkiDB;
    QString mZametkiDBName;

    Sorokovnik *mSorokovnik;
    SorokovnikAppSettingsDialog *mSettingsDialog;

    int mDen;
    int mSorok;
    int mLeto;
};

#endif // SOROKOVNIKAPP_H
