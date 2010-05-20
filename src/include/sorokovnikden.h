#ifndef SOROKOVNIKDEN_H
#define SOROKOVNIKDEN_H

#include "sagraphicspushbutton.h"

class SorokovnikDen : public SAGraphicsPushButton
{
    Q_OBJECT

public:
    SorokovnikDen();
    SorokovnikDen(const QString &nazvanie, const QString &podskazka, const QColor &cvet);

    inline bool segodnya() const { return mSegodnya; }
    inline void setSegodnya(bool segodnya) {
        mSegodnya = segodnya;
        segodnya ? setBackgroundColor(Qt::green) : resetOriginCvet();
    }

    inline bool isVeshchiiSon() const { return mVeshchiiSon; }
    inline void setVeshchiiSon(bool da) {
        mVeshchiiSon = da;
        updateToolTip();
    }

    inline bool isRoditelskiiDen() const { return mRoditelskiiDen; }
    inline void setRoditelskiiDen(bool da) {
        mRoditelskiiDen = da;
        updateToolTip();
    }

    inline bool isDenPochitaniyaPredkov() const { return mDenPochitaniyaPredkov; }
    inline void setDenPochitaniyaPredkov(bool da) {
        mDenPochitaniyaPredkov = da;
        updateToolTip();
    }

    inline bool isVazhnyiPrazdnik() const { return mVazhnyiPrazdnik; }
    inline void setVazhnyiPrazdnik(bool da) {
        mVazhnyiPrazdnik = da;
    }

    inline bool isSvyatyiPost() const { return mSvyatyiPost; }
    inline void setSvyatyiPost(bool da) {
        mSvyatyiPost = da;
        if (da)
            setBackgroundColor(QColor(254, 242, 94, 180));
    }

    inline bool isDazhdbozhyiPost() const { return mDazhdbozhyiPost; }
    inline void setDazhdbozhyiPost(bool da) {
        mDazhdbozhyiPost = da;
        if (da)
            setBackgroundColor(QColor(142, 201, 109, 180));
    }

    inline bool chistyiPost() const { return mChistyiPost; }
    inline void setChistyiPost(bool da) {
        mChistyiPost = da;
        if (da)
            setBackgroundColor(QColor(111, 195, 144, 180));
    }

    inline bool isVelikiiPost() const { return mVelikiiPost; }
    inline void setVelikiiPost(bool da) {
        mVelikiiPost = da;
        if (da)
            setBackgroundColor(QColor(245, 135, 105, 180));
    }

    inline bool isSvetlyiPost() const { return mSvetlyiPost; }
    inline void setSvetlyiPost(bool da) {
        mSvetlyiPost = da;
        if (da)
            setBackgroundColor(QColor(124, 206, 242, 180));
    }

    inline bool isPomechen() const { return mPomechen; }
    inline void setPomechen(bool pomechen) {
        mPomechen = pomechen;
        setUnderline(pomechen);
    }

    inline void resetOriginCvet() {
        setBackgroundColor(mCvet);
    }

    inline QString prazdnik() const {
        return mPrazdnik;
    }
    inline void setPrazdnik(const QString &prazdnik) {
        mPrazdnik = prazdnik;
    }

    inline QString opisaniePrazdnika() const {
        return mOpisaniePrazdnika;
    }
    inline void setOpisaniePrazdnika(const QString &opisanie) {
        mOpisaniePrazdnika = opisanie;
    }

    virtual SorokovnikDen *clone(const QString &nazvanie, const QString &podskazka, const QColor &cvet);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    void updateToolTip() {
        QString toolTip;
        if (isVeshchiiSon())
            toolTip += tr("День вещих снов\n");
        if (isRoditelskiiDen())
            toolTip += tr("Родительский день\n");
        if (isDenPochitaniyaPredkov())
            toolTip += tr("День памяти и почитания предков\n");
        toolTip.chop(1);
        setToolTip(toolTip);
    }

private:
    QString mPodskazka;
    QString mPrazdnik;
    QString mOpisaniePrazdnika;

    QColor mCvet;

    bool mSegodnya;

    bool mVeshchiiSon;
    bool mRoditelskiiDen;
    bool mDenPochitaniyaPredkov;
    bool mVazhnyiPrazdnik;

    bool mSvyatyiPost;
    bool mDazhdbozhyiPost;
    bool mChistyiPost;
    bool mVelikiiPost;
    bool mSvetlyiPost;

    bool mPomechen;
};

#endif // SOROKOVNIKDEN_H
