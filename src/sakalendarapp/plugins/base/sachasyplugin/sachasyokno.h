#ifndef SACHASYOKNO_H
#define SACHASYOKNO_H

#include <QGraphicsWidget>
#include <QStateMachine>

QT_FORWARD_DECLARE_CLASS(SAGraphicsWindow)

class SAChasyOkno : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal zValue READ zValue WRITE setZValue)

public:
    SAChasyOkno(QState *groupState, QStateMachine *machine, QGraphicsItem *parent = 0);

private:
    SAGraphicsWindow *mFrame;
};

#endif // SACHASYOKNO_H
