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

#ifndef KDGRAPHICSBUTTONGROUP_H
#define KDGRAPHICSBUTTONGROUP_H

#include <QGraphicsObject>
#include <QGraphicsWidget>

#include "sagraphicspushbutton.h"

class SAGraphicsButtonGroup : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAGraphicsButtonGroup(QGraphicsItem *parent = 0);

    void addButton(SAGraphicsPushButton *button, int id = -1);
    void removeButton(SAGraphicsPushButton *button);

    SAGraphicsPushButton *checkedButton() const;
    int checkedId() const;

    bool exclusive() const;
    void setExclusive(bool);

    int id(SAGraphicsPushButton *button) const;
    void setId(SAGraphicsPushButton *button, int id);

    SAGraphicsPushButton *button(int id) const;
    QList<SAGraphicsPushButton *> buttons() const;

signals:
    void buttonClicked(SAGraphicsPushButton *button);
    void buttonClicked(int id);

private slots:
    void emitClicked(bool clicked);
    void buttonToggled(bool checked);

private:
    QMap<SAGraphicsPushButton *, int> mPushButtonMap;
    SAGraphicsPushButton *mCheckedButton;
    bool mExclusive;
};

#endif // KDGRAPHICSBUTTONGROUP_H
