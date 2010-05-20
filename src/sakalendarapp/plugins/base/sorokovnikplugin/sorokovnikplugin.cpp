#include <QGraphicsWidget>
#include <QStringList>

#include "sorokovnikokno.h"

#include "sorokovnikplugin.h"

SorokovnikPlugin::SorokovnikPlugin()
{
}

QStringList SorokovnikPlugin::sorokovnik() const
{
    return QStringList() << tr("Сороковник");
}

QGraphicsWidget *SorokovnikPlugin::sozdatSorokovnik(QString sorokovnik, QState *group,
                                                    QStateMachine *machine)
{
    if (sorokovnik == tr("Сороковник")) {
        SorokovnikOkno *s = new SorokovnikOkno(group, machine);
        return s;
    }

    return 0;
}

Q_EXPORT_PLUGIN2(sorokovnikplugin, SorokovnikPlugin)
