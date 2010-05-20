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
