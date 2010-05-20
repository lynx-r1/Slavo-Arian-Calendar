#include <QHBoxLayout>

#include "sachasyokno.h"

#include "sachasyplugin.h"

ChasyPlugin::ChasyPlugin()
{
}

QStringList ChasyPlugin::chasy() const
{
    return QStringList() << tr("Славянские круговые часы") << tr("Славянские числовые часы");
}

QGraphicsWidget *ChasyPlugin::sozdatGraphicsChasy(QString chasy, QState *group,
                                                  QStateMachine *machine)
{
    if (chasy == tr("Славяно-Арийские часы")) {
        SAChasyOkno *c = new SAChasyOkno(group, machine);
        return c;
    }
    return 0;
}

Q_EXPORT_PLUGIN2(chasyplugin, ChasyPlugin)
