#ifndef SOROKOVNIKDENDB_H
#define SOROKOVNIKDENDB_H

#include "sorokovnikden.h"

class SorokovnikDenDB : public SorokovnikDen
{
    Q_OBJECT

public:
    SorokovnikDenDB();
    SorokovnikDenDB(const QString &nazvanie, const QString &podskazka, const QColor &cvet);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    SorokovnikDen *clone(const QString &nazvanie, const QString &podskazka, const QColor &cvet);
};

#endif // SOROKOVNIKDENDB_H
