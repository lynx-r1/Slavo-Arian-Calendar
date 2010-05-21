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

#ifndef SOROKOVNIKPLUGIN_H
#define SOROKOVNIKPLUGIN_H

#include "interfaces.h"

QT_FORWARD_DECLARE_CLASS(QState)
QT_FORWARD_DECLARE_CLASS(QStateMachine)

class SorokovnikPlugin : public QObject, public SorokovnikInterface {
    Q_OBJECT
    Q_INTERFACES(SorokovnikInterface)

public:
    SorokovnikPlugin();

    QStringList sorokovnik() const;
    QGraphicsWidget *sozdatSorokovnik(QString sorokovnik, QState *group,
                                      QStateMachine *machine);
};

#endif // SOROKOVNIKPLUGIN_H
