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

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>

class QGraphicsWidget;
class QState;
class QStateMachine;
class QStringList;
class QWidget;

class KDTimeTimer;

class DK4Interface {
public:
    virtual ~DK4Interface() {}

    virtual QString dk4Name() const = 0;
    virtual QGraphicsWidget *sozdatDK4(QState *group, QStateMachine *machine) = 0;
};

class SorokovnikInterface {
public:
    virtual ~SorokovnikInterface() {}

    virtual QString sorokovnikName() const = 0;
    virtual QGraphicsWidget *sozdatSorokovnik(QState *group, QStateMachine *machine) = 0;
};

class SAChasyInterface {
public:
    virtual ~SAChasyInterface() {}

    virtual QString sachasyName() const = 0;
    virtual QGraphicsWidget *sozdatSAChasy(QState *group, QStateMachine *machine) = 0;
};

Q_DECLARE_INTERFACE(DK4Interface, "SAKalendar.DK4Interface/0.1")
Q_DECLARE_INTERFACE(SorokovnikInterface, "SAKalendar.SorokovnikInterface/0.1")
Q_DECLARE_INTERFACE(SAChasyInterface, "SAKalendar.SAChasyInterface/0.1")

#endif // INTERFACES_H
