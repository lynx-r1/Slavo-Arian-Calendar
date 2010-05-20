#ifndef DK4PLUGIN_H
#define DK4PLUGIN_H

#include "interfaces.h"

QT_FORWARD_DECLARE_CLASS(QState)
QT_FORWARD_DECLARE_CLASS(QStateMachine)

class DK4Plugin : public QObject, public KrugoletChislobogaInterface {
    Q_OBJECT
    Q_INTERFACES(KrugoletChislobogaInterface)

public:
    DK4Plugin();

    QStringList krugolet() const;
    QGraphicsWidget *sozdatKrugolet(QString krugolet, QState *group, QStateMachine *machine);
};

#endif // DK4PLUGIN_H
