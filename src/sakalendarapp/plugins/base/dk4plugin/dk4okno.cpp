#include <QGraphicsLinearLayout>
#include <QPropertyAnimation>
#include <QStateMachine>
#include <QSettings>

#include "sagraphicswindow.h"

#include "sadate_cpp.h"

#include "dk4.h"
#include "dk4okno.h"

DK4Okno::DK4Okno(QState *groupState, QStateMachine *machine, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mFrame(0)
{
    setObjectName("theDK4Okno");

    QSettings s;
    bool effects = s.value("/SAKalendar/SAKalendarApp/PluginEffects").toBool();

    DK4Base *dk4 = new DK4;
    mFrame = new SAGraphicsWindow(tr("Даарийский Круголет Числобога"),
                                  effects, this);
    mFrame->setCentralWidget(dk4);
    mFrame->createAnimatedStateSelected(groupState, machine);
    connect(dk4, SIGNAL(showInfoRequest(bool,QString)), mFrame, SLOT(showInfo(bool,QString)));

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(mFrame);
}
