#ifndef DK4KRUGOLETOKNO_H
#define DK4KRUGOLETOKNO_H

#include <QGraphicsWidget>

QT_FORWARD_DECLARE_CLASS(QState)
QT_FORWARD_DECLARE_CLASS(QStateMachine)

QT_FORWARD_DECLARE_CLASS(SAGraphicsWindow)

class DK4Okno : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal zValue READ zValue WRITE setZValue)

public:
    DK4Okno(QState *groupState, QStateMachine *machine, QGraphicsItem *parent = 0);

private:
    SAGraphicsWindow *mFrame;
};

#endif // DK4KRUGOLETOKNO_H

