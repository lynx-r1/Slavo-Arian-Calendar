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

#ifndef NOVOLETIYAPLUGIN_H
#define NOVOLETIYAPLUGIN_H

#include "interfaces.h"

class NachalaSorokovnikovPlugin : public QObject, NachalaSorokovnikovInterface {
    Q_OBJECT
    Q_INTERFACES(NachalaSorokovnikovInterface)

public:
    NachalaSorokovnikovPlugin();

    QStringList nachalaSorokovnikov() const;
    QGraphicsWidget *sozdatNachalaSorokovnikov(QString nachSorok);
};

#endif // NOVOLETIYAPLUGIN_H
