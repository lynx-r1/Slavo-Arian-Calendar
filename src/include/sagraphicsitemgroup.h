#ifndef SAGRAPHICSITEMGROUP_H
#define SAGRAPHICSITEMGROUP_H

#include <QGraphicsItem>

class SAGraphicsItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    SAGraphicsItemGroup(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
};

#endif // SAGRAPHICSITEMGROUP_H
