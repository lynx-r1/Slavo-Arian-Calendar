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

class KrugoletChislobogaInterface {
public:
    virtual ~KrugoletChislobogaInterface() {}

    virtual QStringList krugolet() const = 0;
    virtual QGraphicsWidget *sozdatKrugolet(QString krugolet, QState *group, QStateMachine *machine) = 0;
};
#if 0
class NachalaSorokovnikovInterface {
public:
    virtual ~NachalaSorokovnikovInterface() {}

    virtual QStringList nachalaSorokovnikov() const = 0;
    virtual QGraphicsWidget *sozdatNachalaSorokovnikov(QString nachSorok) = 0;
};
#endif
class SorokovnikInterface {
public:
    virtual ~SorokovnikInterface() {}

    virtual QStringList sorokovnik() const = 0;
    virtual QGraphicsWidget *sozdatSorokovnik(QString sorokovnik, QState *group,
                                              QStateMachine *machine) = 0;
};

class ChasyInterface {
public:
    virtual ~ChasyInterface() {}

    virtual QStringList chasy() const = 0;
    virtual QGraphicsWidget *sozdatGraphicsChasy(QString chasy, QState *group,
                                                 QStateMachine *machine) = 0;
};

class RaschetyInterface {
public:
    virtual ~RaschetyInterface() {}

    virtual QStringList raschety() const = 0;
    virtual QWidget *sozdatRaschet(QString raschet) = 0;
};

Q_DECLARE_INTERFACE(KrugoletChislobogaInterface, "KalyadyDar.KrugoletChislobogaInterface/0.1")
//Q_DECLARE_INTERFACE(NachalaSorokovnikovInterface, "KalyadyDar.NachalaSorokovnikovInterface/0.1")
Q_DECLARE_INTERFACE(SorokovnikInterface, "KalyadyDar.SorokovnikInterface/0.1")
Q_DECLARE_INTERFACE(ChasyInterface, "KalyadyDar.ChasyInterface/0.1")
Q_DECLARE_INTERFACE(RaschetyInterface, "KalyadyDar.RaschetyInterface/0.1")

#endif // INTERFACES_H
