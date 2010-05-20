#ifndef NOVOLETIYAPLUGIN_H
#define NOVOLETIYAPLUGIN_H

#include "interfaces.h"

class NachalaSorokovnikovPlugin : public QObject, NachalaSorokovnikovInterface {
    Q_OBJECT
    Q_INTERFACES(NachalaSorokovnikovInterface)

public:
    NachalaSorokovnikovPlugin();

    QStringList nachalaSorokovnikov() const;
    QGraphicsWidget *sozdatNachalaSorokovnikov(QString nachSorok);
};

#endif // NOVOLETIYAPLUGIN_H
