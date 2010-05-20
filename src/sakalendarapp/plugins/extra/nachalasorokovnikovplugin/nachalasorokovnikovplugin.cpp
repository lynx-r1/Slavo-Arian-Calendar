#include <QGraphicsWidget>
#include <QStringList>

#include "nachalasorokovnikov/nachalasorokovnikovokno.h"

#include "nachalasorokovnikovplugin.h"

NachalaSorokovnikovPlugin::NachalaSorokovnikovPlugin()
{
}

QStringList NachalaSorokovnikovPlugin::nachalaSorokovnikov() const
{
    return QStringList() << tr("Начала Сороковников");
}

QGraphicsWidget *NachalaSorokovnikovPlugin::sozdatNachalaSorokovnikov(QString nachSorok)
{
    if (nachSorok == tr("Начала Сороковников"))
        return new NachalaSorokovnikovOkno;

    return 0;
}

Q_EXPORT_PLUGIN2(nachalasorokovnikovplugin, NachalaSorokovnikovPlugin)
