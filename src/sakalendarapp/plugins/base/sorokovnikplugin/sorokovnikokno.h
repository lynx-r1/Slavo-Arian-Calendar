#ifndef SOROKOVNIKOKNO_H
#define SOROKOVNIKOKNO_H

#include <QGraphicsWidget>
#include <QStateMachine>

QT_FORWARD_DECLARE_CLASS(SAGraphicsWindow)

class SorokovnikOkno : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal zValue READ zValue WRITE setZValue)

public:
    SorokovnikOkno(QState *groupState, QStateMachine *machine, QGraphicsItem *parent = 0);

private:
    SAGraphicsWindow *mFrame;
};

#endif // SOROKOVNIKOKNO_H
