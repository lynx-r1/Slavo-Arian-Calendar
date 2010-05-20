#include <QGraphicsLinearLayout>
#include <QPropertyAnimation>
#include <QSettings>

#include "sagraphicswindow.h"

#include "sachasy.h"
#include "sachasyokno.h"

SAChasyOkno::SAChasyOkno(QState *groupState, QStateMachine *machine, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mFrame(0)
{
    setObjectName("theSAChasyOkno");

    QSettings s;
    bool effects = s.value("/SAKalendar/SAKalendarApp/PluginEffects").toBool();

    SAChasy *sachasy = new SAChasy;
    sachasy->setMinimumSize(270, 270);

    mFrame = new SAGraphicsWindow(tr("Часы    "), effects, this);
    mFrame->setCentralWidget(sachasy);
    mFrame->createAnimatedStateSelected(groupState, machine);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(mFrame);

    setMinimumSize(250, 300);
}
