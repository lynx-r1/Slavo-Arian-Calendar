#ifndef DK4LETODB_H
#define DK4LETODB_H

#include "dk4leto.h"

class DK4LetoDB : public DK4Leto
{
    Q_OBJECT

public:
    DK4LetoDB();
    DK4LetoDB(int nomerLeta, int denNedeliNovoletiya, int krugLet,
              int stikhiyaICvet, int nachalo);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    DK4LetoDB *clone(int nomerLeta, int denNedeliNovoletiya, int krugLet,
                             int stikhiyaICvet, int nachalo);
};

#endif // DK4LETODB_H
