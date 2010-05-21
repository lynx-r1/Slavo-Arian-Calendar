/*****************************************************************************
 *   Copyright (C) 2009-2010  Jack R  <lynx21.12.12@yandex.ru>               *
 *                                                                           *
 *   This file is part of SAKalendar.                                        *
 *                                                                           *
 *   SAKalendar is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

#ifndef SOROKOVNIK_H
#define SOROKOVNIK_H

#include <QGraphicsWidget>
#include <QSqlDatabase>

#include "sagraphicsstackedlayout.h"

QT_FORWARD_DECLARE_CLASS(QEasingCurve)
QT_FORWARD_DECLARE_CLASS(QGraphicsGridLayout)
QT_FORWARD_DECLARE_CLASS(QGraphicsLinearLayout)

QT_FORWARD_DECLARE_CLASS(SAGraphicsButtonGroup)
QT_FORWARD_DECLARE_CLASS(SAGraphicsPushButton)
QT_FORWARD_DECLARE_CLASS(SAGraphicsLabelItem)
QT_FORWARD_DECLARE_CLASS(SATimer)

QT_FORWARD_DECLARE_CLASS(SorokovnikDen)

class SorokovnikBase : public QGraphicsWidget
{
    Q_OBJECT

public:
    SorokovnikBase(QGraphicsItem *parent = 0, bool deflt = true);
    ~SorokovnikBase();

    inline QEasingCurve switchSorokovnikEasingCurve() const {
        if (mStackSorokovnik)
            return mStackSorokovnik->switchEasingCurve();
        return QEasingCurve();
    }
    inline void setSwitchSorokovnikEasingCurve(const QEasingCurve &curve) {
        if (mStackSorokovnik) {
            mStackSorokovnik->setSwitchEasingCurve(curve);
            mSwitchSorokovnikEasingCurve = curve;
        }
    }

    inline QEasingCurve switchMesyacEasingCurve() const {
        if (mStackMesyac)
            return mStackMesyac->switchEasingCurve();
        return QEasingCurve();
    }
    inline void setSwitchMesyacEasingCurve(const QEasingCurve &curve) {
        if (mStackMesyac)
            mStackMesyac->setSwitchEasingCurve(curve);
    }

    inline int switchSorokovnikDuration() const {
        if (mStackSorokovnik)
            return mStackSorokovnik->switchDuration();
        return 0;
    }
    inline void setSwitchSorokovnikDuration(int duration) {
        if (mStackSorokovnik) {
            mStackSorokovnik->setSwitchDuration(duration);
            mSwitchSorokovnikDuration = duration;
        }
    }

    inline int switchMesyacDuration() const {
        if (mStackMesyac)
            return mStackMesyac->switchDuration();
        return 0;
    }
    inline void setSwitchMesyacDuration(int duration) {
        if (mStackMesyac)
            mStackMesyac->setSwitchDuration(duration);
    }

    inline SAGraphicsStackedLayout::SwitchDirection switchSorokovnikNextDirection() const {
        return mSorokovnikNextDirection;
    }
    inline void setSwitchSorokovnikNextDirection(SAGraphicsStackedLayout::SwitchDirection direction) {
        mSorokovnikNextDirection = direction;
    }

    inline SAGraphicsStackedLayout::SwitchDirection switchSorokovnikPrevDirection() const {
        return mSorokovnikPrevDirection;
    }
    inline void setSwitchSorokovnikPrevDirection(SAGraphicsStackedLayout::SwitchDirection direction) {
        mSorokovnikPrevDirection = direction;
    }

    inline SAGraphicsStackedLayout::SwitchDirection switchMesyacNextDirection() const {
        return mMesyacNextDirection;
    }
    inline void setSwitchMesyacNextDirection(SAGraphicsStackedLayout::SwitchDirection direction) {
        mMesyacNextDirection = direction;
    }

    inline SAGraphicsStackedLayout::SwitchDirection switchMesyacPrevDirection() const {
        return mMesyacPrevDirection;
    }
    inline void setSwitchMesyacPrevDirection(SAGraphicsStackedLayout::SwitchDirection direction) {
        mMesyacPrevDirection = direction;
    }

    inline bool silentSwitchSorokovnik() const {
        if (mStackSorokovnik)
            return mStackSorokovnik->silentSwitch();
        return false;
    }
    inline void setSilentSwitchSorokovnik(bool silent) {
        if (mStackSorokovnik) {
            mStackSorokovnik->setSilentSwitch(silent);
            mSilentSwitchSorokovnik = silent;
        }
    }

    inline bool silentSwitchMesyac() const {
        if (mStackMesyac)
            return mStackMesyac->silentSwitch();
        return false;
    }
    inline void setSilentSwitchMesyac(bool silent) {
        if (mStackMesyac)
            mStackMesyac->setSilentSwitch(silent);
    }

    inline int den() const { return mDen; }
    void setDen(int den);

    inline int leto() const { return mLeto; }
    void setLeto(int leto, bool silent = true);

    inline int kzhizni() const { return mKZhizni; }
    inline int klet() const { return mKLet; }

    inline int sorokovnik() const { return mSorokovnik; }
    inline void setSorokovnik(int sorokovnik, bool silent = true) {
        if (sorokovnik != mSorokovnik) {
            mSorokovnik = sorokovnik;
            if (silent)
                silentSwitchToSorokovnik(sorokovnik - 1);
            else
                pereitiNaSorokovnik(sorokovnik - 1, false);
            emit sorokovnikChanged(mSorokovnik);
        }
    }

    int checkedDenId() const;
    SorokovnikDen *checkedDen() const;
    SorokovnikDen *den(int den, int sorokovnik) const;

    virtual void createSorokovnik();
    virtual void deleteSorokovnik();

    virtual SorokovnikDen *denPrototype() const { return mDenPrototype; }
    virtual void setDenPrototype(SorokovnikDen *denPrototype) { mDenPrototype = denPrototype; }

public slots:
    void sledLeto();
    void predLeto();
    void sledSorokovnik();
    void predSorokovnik();

signals:
    void letoChanged(int leto);
    void sorokovnikChanged(int sorokovnik);
    void selected(int den, int mesyac, int leto);

private slots:
    void denClicked(int day);
    void stackedLayoutIndexChanged(int);

protected:
    inline QGraphicsLinearLayout *shemaLeto() const { return mShemaLeto; }
    inline SAGraphicsStackedLayout *stackMesyac() const { return mStackMesyac; }
    inline SAGraphicsStackedLayout *stackSorokovnik() const { return mStackSorokovnik; }
    inline QList<SAGraphicsButtonGroup *> sorokovnikButtonGroupList() const { return mSorokovnikButtonGroupList; }

    SorokovnikDen *createDen(const QString &name, const QString & toolTip,
                             const QColor &color);
    void silentSwitchToSorokovnik(int sorokovnik);

    virtual QGraphicsWidget *dobavitDen(int sorokovnik, int nomer, int cvet,
                                        bool nepolnMes = false);
    virtual QGraphicsWidget *sozdatSorokovnik(int nachRamhata);
    virtual QGraphicsLayout *shemaSorokovnika();

private:
    void recreateSorokovnikButtonGroup();
    void pereitiNaLeto(int leto, bool silent = false);
    void pereitiNaSorokovnik(int sorokovnik, bool next);

    QGraphicsWidget *setupStackSorokovnik(int leto);

private:
    QList<SAGraphicsButtonGroup *> mSorokovnikButtonGroupList;
    QGraphicsGridLayout *mSorokovnikShema;

    QGraphicsLinearLayout *mShemaLeto;
    QEasingCurve mSwitchSorokovnikEasingCurve;

    SAGraphicsStackedLayout *mStackMesyac;
    SAGraphicsStackedLayout *mStackSorokovnik;
    SorokovnikDen *mDenPrototype;

    SAGraphicsStackedLayout::SwitchDirection mMesyacNextDirection;
    SAGraphicsStackedLayout::SwitchDirection mMesyacPrevDirection;
    SAGraphicsStackedLayout::SwitchDirection mSorokovnikNextDirection;
    SAGraphicsStackedLayout::SwitchDirection mSorokovnikPrevDirection;

    int mSwitchSorokovnikDuration;
    int mDen;
    int mLeto;
    int mKZhizni;
    int mKLet;
    int mSorokovnik;

    bool mSilentSwitchSorokovnik;
    bool mBlockSorokovnikSwitch;
};

class SorokovnikDB : public SorokovnikBase
{
    Q_OBJECT

public:
    SorokovnikDB(QGraphicsItem *parent = 0, bool deflt = true);

    QSqlDatabase openPrazdnikiDb(const QString &name, const QString &connectionName);
    void updateCheckedDen(const QString &prazdnik, const QString &opisanie, int rang);
    void populatePrazdnikami();

    inline QString dbPrazdnikiName() const { return mPrazdnikiDbName; }
    inline void setDbPraznikiName(const QString &dbName) { mPrazdnikiDbName = dbName; }

    inline QString dbPrazdnikiConnectionName() const { return mPrazdnikiDbConnectionName; }
    inline void setDbPrazdnikiConnectionName(const QString &conName) { mPrazdnikiDbConnectionName = conName; }

public slots:
    void populatePrazdnikamiLeto(int leto);

private:
    bool getInfoFromDB(int sorokovnik, int den, QString &title, QString &description,
                       int &rang);
    void setInfoForDay(int sorokovnik, int den, const QString &prazdnik,
                       const QString &opisanie, int rang);

    QGraphicsLayout *shemaNavig();

private:
    QString mPrazdnikiDbConnectionName;
    QString mPrazdnikiDbName;
};

class Sorokovnik: public SorokovnikDB
{
    Q_OBJECT

public:
    Sorokovnik(QGraphicsItem *parent = 0, bool deflt = true);
    ~Sorokovnik();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void createSorokovnik();
    void deleteSorokovnik();

    inline int chas() const { return mChas; }
    inline QString chasText() const {
        QString chas = QString::number(mChas);
        if (chas.length() < 2)
            chas.prepend("0");
        return chas;
    }

    inline int chast() const { return mChast; }
    inline QString chastText() const {
        QString chast = QString::number(mChast);
        if (chast.length() < 2)
            chast.prepend("00");
        else if (chast.length() < 3)
            chast.prepend("0");
        return chast;
    }

    inline QColor conturColor() const { return mConturColor; }
    inline void setConturColor(const QColor &color) {
        mConturColor = color;
        update();
    }

public slots:
    void segodnya();
    void naDatu(bool na);

signals:
    void showInfoRequest(bool, const QString &);

private slots:
    void updateChas(int chas);
    void updateChast(int chast);
    void updateDate();
    void updateInfoLabel();
    void updateSegodnyaDen();
    void updateInfoHtml();
    void pereitiNaDatu();
    void showInfoHtml(bool show);

protected:
    QGraphicsWidget *dobavitDen(int sorokovnik, int nomer, int cvet, bool nepolnMes);

private:
    void setupConnections();

    QGraphicsLayout *shemaNavig();

private:
    QColor mConturColor;

    SAGraphicsStackedLayout *mStackData;
    SAGraphicsLabelItem *mLetoLabel;

    SAGraphicsPushButton *mPrevLetoPushButton;
    SAGraphicsPushButton *mNextLetoPushButton;
    SAGraphicsPushButton *mPrevSorokovnikPushButton;
    SAGraphicsPushButton *mNextSorokovnikPushButton;

    SATimer *mTimer;

    int mSeichasSorokovnik;
    int mSeichasLeto;

    int mSegodnyaDen;
    int mDenNedeli;
    int mChas;
    int mChast;

    int mSvyashchennoeLeto;
};

#endif // SOROKOVNIK_H
