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

#include "sagraphicsbuttongroup.h"

SAGraphicsButtonGroup::SAGraphicsButtonGroup(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mCheckedButton(0)
    , mExclusive(false)
{
}

void SAGraphicsButtonGroup::addButton(SAGraphicsPushButton *button, int id)
{
    if (!button)
        return;
    if (SAGraphicsButtonGroup *previous = button->graphicsGroup())
        previous->removeButton(button);
    button->setGraphicsGroup(this);
    if (id == -1) {
        QList<int> ids = mPushButtonMap.values();
        if (ids.isEmpty())
            mPushButtonMap[button] = -2;
        else {
            qSort(ids);
            mPushButtonMap[button] = ids.first() - 1;
        }
    } else {
        mPushButtonMap[button] = id;
    }

    connect(button, SIGNAL(clicked(bool)), this, SLOT(emitClicked(bool)));
    connect(button, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
}

void SAGraphicsButtonGroup::removeButton(SAGraphicsPushButton *button)
{
    if (button->graphicsGroup() == this) {
        disconnect(button, SIGNAL(clicked(bool)), this, SLOT(emitClicked(bool)));
        disconnect(button, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));

        button->setGraphicsGroup(0);
        mPushButtonMap.remove(button);
    }
}

SAGraphicsPushButton *SAGraphicsButtonGroup::checkedButton() const
{
    return mCheckedButton;
}

int SAGraphicsButtonGroup::checkedId() const
{
    return mPushButtonMap.value(mCheckedButton, -1);
}

bool SAGraphicsButtonGroup::exclusive() const
{
    return mExclusive;
}

void SAGraphicsButtonGroup::setExclusive(bool exclusive)
{
    mExclusive = exclusive;
}

int SAGraphicsButtonGroup::id(SAGraphicsPushButton *button) const
{
    return mPushButtonMap.value(button, -1);
}

void SAGraphicsButtonGroup::setId(SAGraphicsPushButton *button, int id)
{
    if (button && id != -1)
        mPushButtonMap[button] = id;
}

SAGraphicsPushButton *SAGraphicsButtonGroup::button(int id) const
{
    return mPushButtonMap.key(id);
}

QList<SAGraphicsPushButton *> SAGraphicsButtonGroup::buttons() const
{
    return mPushButtonMap.keys();
}

void SAGraphicsButtonGroup::emitClicked(bool clicked)
{
    SAGraphicsPushButton *previous = mCheckedButton;
    mCheckedButton = clicked ? qobject_cast<SAGraphicsPushButton *>(sender()) : 0;

    if (mExclusive && previous && previous != mCheckedButton) {
        previous->setChecked(false);
        previous->update(previous->boundingRect());
    }

    emit buttonClicked(mCheckedButton);
    emit buttonClicked(id(mCheckedButton));
}

void SAGraphicsButtonGroup::buttonToggled(bool checked)
{
    if (checked) {
        SAGraphicsPushButton *previous = mCheckedButton;
        mCheckedButton = qobject_cast<SAGraphicsPushButton *>(sender());

        if (mExclusive && previous && previous != mCheckedButton) {
            previous->setChecked(false);
            previous->update(previous->boundingRect());
        }
    }
}
