#ifndef CHASYIPLUGIN_H
#define CHASYIPLUGIN_H

#include "interfaces.h"

class ChasyPlugin : public QObject, public ChasyInterface {
    Q_OBJECT
    Q_INTERFACES(ChasyInterface)

public:
    ChasyPlugin();

    QStringList chasy() const;
    QGraphicsWidget *sozdatGraphicsChasy(QString chasy, QState *group,
                                         QStateMachine *machine);
};

#endif // CHASYIPLUGIN_H
