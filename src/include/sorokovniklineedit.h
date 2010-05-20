#ifndef SOROKOVNIKLINEEDIT_H
#define SOROKOVNIKLINEEDIT_H

#include <QLineEdit>
#include <QMap>

class SorokovnikLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    SorokovnikLineEdit(QWidget *parent = 0);
    void getDate(int &den, int &sorokovnik, int &leto);

protected:
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

private slots:
    void parseDate();
    void highlite(const QString &text);

private:
    QString mGreating;
    QMap<QString, int> mSorokovniki;

    int mDen;
    int mSorokovnik;
    int mLeto;
};

#endif // SOROKOVNIKLINEEDIT_H
