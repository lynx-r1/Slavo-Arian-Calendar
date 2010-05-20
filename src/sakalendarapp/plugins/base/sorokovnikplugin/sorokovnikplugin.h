#ifndef SOROKOVNIKPLUGIN_H
#define SOROKOVNIKPLUGIN_H

#include "interfaces.h"

QT_FORWARD_DECLARE_CLASS(QState)
QT_FORWARD_DECLARE_CLASS(QStateMachine)

class SorokovnikPlugin : public QObject, public SorokovnikInterface {
    Q_OBJECT
    Q_INTERFACES(SorokovnikInterface)

public:
    SorokovnikPlugin();

    QStringList sorokovnik() const;
    QGraphicsWidget *sozdatSorokovnik(QString sorokovnik, QState *group,
                                      QStateMachine *machine);
};

#endif // SOROKOVNIKPLUGIN_H
