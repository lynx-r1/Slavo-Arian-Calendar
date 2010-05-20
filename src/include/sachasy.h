#ifndef SACHASYI_H
#define SACHASYI_H

#include <QGraphicsWidget>

#include "sadate_cpp.h"

QT_FORWARD_DECLARE_CLASS(SATime)
QT_FORWARD_DECLARE_CLASS(SATimer)

class SAChasy : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAChasy(QGraphicsItem *parent = 0);
    ~SAChasy();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    inline bool hasEffects() { return mSimpleView; }
    inline bool isPaused() { return mPausa; }

    inline int chas() const { return mChas; }
    inline int chast() const { return mChast; }
    inline int dolya() const { return mDolya; }

    inline int updateInterval() const { return mUpdateInterval; }
    inline void setUpdateInterval(int updateInterval) {
        mUpdateInterval = updateInterval;
        resetTimer(mIgnoreDST, mUpdateInterval);
    }

    inline bool dolevayaStrelka() const { return mDolevayaStrelka; }
    inline void setDolevayaStrelka(bool dolevayaStrelka) { mDolevayaStrelka = dolevayaStrelka; }

    inline bool ignoreDST() const { return mIgnoreDST; }
    inline void setIgnoreDST(bool ignore) {
        mIgnoreDST = ignore;
        resetTimer(mIgnoreDST, mUpdateInterval);
    }

    inline bool simpleView() const { return mSimpleView; }
    inline void setSimpleView(bool simple) {
        mSimpleView = simple;
        update();
    }

    inline bool pravSutKrug() const { return mPravSutKrug; }
    inline void setPravSutKrug(bool prav) {
        mPravSutKrug = prav;
        update();
    }

    inline bool showDate() const { return mShowDate; }
    inline void setShowDate(int show) {
        mShowDate = show;
        update();
    }

    inline QColor backgroundColor() const { return mBackgroundColor; }
    inline void setBackgroundColor(const QColor &color) {
        mBackgroundColor = color;
        update();
    }

    void setFixedWidth(qreal width);
    void setFixedHeight(qreal width);
    void setFixedSize(const QSizeF &size);
    void setFixedSize(int width, int height);

public slots:
    void pause(bool pause);

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private slots:
    void setDolyu(int);
    void setChast(int);
    void setChas(int);

    void updateDate();

private:
    void resetTimer(bool ignoreDST, int updateInterval);
    void setTime(const SATime &time);

private:
    QList<QPixmap> mChasyKartinki;

    QColor mBackgroundColor;
    QColor mShadowColor;

    SADate mDate;
    SATimer *mTimer;

    int mChas;
    int mChast;
    int mDolya;

    // бегает по кругу и вращает стрелки
    int mVolchok;
    int mUpdateInterval;

    bool mPravSutKrug;
    bool mPausa;
    bool mSimpleView;
    bool mDolevayaStrelka;
    bool mIgnoreDST;
    bool mShowDate;
};

#endif // SACHASYI_H
