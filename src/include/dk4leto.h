#ifndef DK4LETO_H
#define DK4LETO_H

#include "sagraphicspushbutton.h"

class DK4Leto : public SAGraphicsPushButton
{
    Q_OBJECT

public:
    DK4Leto();
    DK4Leto(int nomerLeta, int denNedeliNovoletiya, int krugLet,
            int stikhiyaICvet, int nachalo);

    inline void setSeichasLeto(bool seyichas) {
        mSeyichas = seyichas;
        seyichas ? setBackgroundColor(Qt::green) : resetOriginCvet();
    }
    inline bool isSeyichasLeto() const {
        return mSeyichas;
    }

    inline void resetOriginCvet() {
        setBackgroundColor(mColorLeta);
    }

    inline QString name() const { return mName; }
    inline void setName(const QString &name) { mName = name; }

    inline QString description() const { return mDescription; }
    inline void setDescription(const QString &description) { mDescription = description; }

    inline QString symbol() const { return mSymbol; }
    inline void setSymbol(const QString &symbol) { mSymbol = symbol; }

    inline QString power() const { return mPower; }
    inline void setPower(const QString &power) { mPower = power; }

    inline QString colorLeta() const { return mColorLeta; }
    inline void setColorLeta(const QString &colorLeta) {
        mColorLeta = colorLeta;
        setBackgroundColor(QColor(mColorLeta));
    }

    virtual DK4Leto *clone(int nomerLeta, int denNedeliNovoletiya, int krugLet,
                           int stikhiyaICvet, int nachalo);

private:
    void initToolTips();

private:
    QStringList mStihiyaCvet;
    QStringList mKrugiLet;
    QStringList mDniNedeli;

    QString mName;
    QString mDescription;
    QString mSymbol;
    QString mPower;
    QString mColorLeta;

    int mNomerLeta;
    int mDenNedeliNovoletiya;
    int mKrugLet;
    int mStikhiyaICvet;
    int mNachalo;

    bool mSeyichas;
};

#endif // DK4LETO_H
