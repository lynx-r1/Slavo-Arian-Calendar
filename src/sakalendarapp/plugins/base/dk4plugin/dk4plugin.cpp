#include <QGraphicsView>
#include <QStringList>

#include "dk4okno.h"

#include "dk4plugin.h"

DK4Plugin::DK4Plugin()
{
}

QStringList DK4Plugin::krugolet() const
{
    return QStringList() << tr("Даарийский Круголет Числобога");
}

QGraphicsWidget *DK4Plugin::sozdatKrugolet(QString krugolet, QState *group, QStateMachine *machine)
{
    if (krugolet == tr("Даарийский Круголет Числобога")) {
        DK4Okno *k = new DK4Okno(group, machine);
        return k;
    }

    return 0;
}

Q_EXPORT_PLUGIN2(dk4plugin, DK4Plugin)
