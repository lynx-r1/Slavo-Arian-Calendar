#ifndef DENNEDELI_H
#define DENNEDELI_H

#include "kdgraphicspushbutton.h"

class DenNedeli : public KDGraphicsPushButton
{
    Q_OBJECT

public:
    DenNedeli();
    DenNedeli(QString nazvanie, QString podskazka, QColor cvet, int mesyac, bool svyashch);

    void vzyatInfo(int &mesyac, bool &svyashch);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

private:
    QString mPodskazka;
    QColor mCvet;
    int mMesyac;
    bool mSvyashch;
};

#endif // DENNEDELI_H
