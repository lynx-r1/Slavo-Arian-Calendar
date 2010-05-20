#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>

class QGraphicsWidget;
class QState;
class QStateMachine;
class QStringList;
class QWidget;

class KDTimeTimer;

class KrugoletChislobogaInterface {
public:
    virtual ~KrugoletChislobogaInterface() {}

    virtual QStringList krugolet() const = 0;
    virtual QGraphicsWidget *sozdatKrugolet(QString krugolet, QState *group, QStateMachine *machine) = 0;
};
#if 0
class NachalaSorokovnikovInterface {
public:
    virtual ~NachalaSorokovnikovInterface() {}

    virtual QStringList nachalaSorokovnikov() const = 0;
    virtual QGraphicsWidget *sozdatNachalaSorokovnikov(QString nachSorok) = 0;
};
#endif
class SorokovnikInterface {
public:
    virtual ~SorokovnikInterface() {}

    virtual QStringList sorokovnik() const = 0;
    virtual QGraphicsWidget *sozdatSorokovnik(QString sorokovnik, QState *group,
                                              QStateMachine *machine) = 0;
};

class ChasyInterface {
public:
    virtual ~ChasyInterface() {}

    virtual QStringList chasy() const = 0;
    virtual QGraphicsWidget *sozdatGraphicsChasy(QString chasy, QState *group,
                                                 QStateMachine *machine) = 0;
};

class RaschetyInterface {
public:
    virtual ~RaschetyInterface() {}

    virtual QStringList raschety() const = 0;
    virtual QWidget *sozdatRaschet(QString raschet) = 0;
};

Q_DECLARE_INTERFACE(KrugoletChislobogaInterface, "KalyadyDar.KrugoletChislobogaInterface/0.1")
//Q_DECLARE_INTERFACE(NachalaSorokovnikovInterface, "KalyadyDar.NachalaSorokovnikovInterface/0.1")
Q_DECLARE_INTERFACE(SorokovnikInterface, "KalyadyDar.SorokovnikInterface/0.1")
Q_DECLARE_INTERFACE(ChasyInterface, "KalyadyDar.ChasyInterface/0.1")
Q_DECLARE_INTERFACE(RaschetyInterface, "KalyadyDar.RaschetyInterface/0.1")

#endif // INTERFACES_H
