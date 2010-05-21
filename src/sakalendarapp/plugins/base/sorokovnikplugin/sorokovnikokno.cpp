/*****************************************************************************
 *   Copyright (C) 2009-2010  Jack R  <lynx21.12.12@yandex.ru>               *
 *                                                                           *
 *   This file is part of SAKalendar.                                        *
 *                                                                           *
 *   SAKalendar is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

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
