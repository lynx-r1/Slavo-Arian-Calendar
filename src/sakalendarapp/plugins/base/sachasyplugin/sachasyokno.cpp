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
