#include <QGraphicsLinearLayout>
#include <QPropertyAnimation>
#include <QSettings>

#include "sagraphicswindow.h"

#include "dk4.h"
#include "sorokovnik.h"
#include "sorokovnikokno.h"

SorokovnikOkno::SorokovnikOkno(QState *groupState, QStateMachine *machine, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mFrame(0)
{
    setObjectName("theSorokovnikOkno");

    QSettings s;
    bool effects = s.value("/SAKalendar/SAKalendarApp/PluginEffects").toBool();

    SorokovnikBase *sorokovnik = new Sorokovnik;
    sorokovnik->setSwitchMesyacEasingCurve(QEasingCurve::OutExpo);
    sorokovnik->setSwitchSorokovnikNextDirection(SAGraphicsStackedLayout::Top2Bottom);
    sorokovnik->setSwitchSorokovnikPrevDirection(SAGraphicsStackedLayout::Top2Bottom);

    mFrame = new SAGraphicsWindow(tr("Сороковник (месяц)"), effects, this);
    mFrame->setCentralWidget(sorokovnik);
    mFrame->createAnimatedStateSelected(groupState, machine);
    connect(sorokovnik, SIGNAL(showInfoRequest(bool,QString)), mFrame, SLOT(showInfo(bool,QString)));

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(mFrame);
}
