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

#include <QGraphicsSceneResizeEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QPushButton>

#include "sagraphicstitlebarbutton.h"

SAGraphicsTitleBarButton::SAGraphicsTitleBarButton(const QColor &color, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mBackgroundColor(color)
{
    setObjectName("theSAGraphicsTitleBarButton");

    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);
}

bool SAGraphicsTitleBarButton::checked() const
{
    return mChecked;
}

void SAGraphicsTitleBarButton::setChecked(bool checked)
{
    mChecked = checked;
}

QPainterPath SAGraphicsTitleBarButton::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void SAGraphicsTitleBarButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsWidget::hoverLeaveEvent(event);
    update(boundingRect());
}

void SAGraphicsTitleBarButton::setFixedWidth(qreal width)
{
    setMinimumWidth(width);
    setMaximumWidth(width);
}

void SAGraphicsTitleBarButton::setFixedHeight(qreal height)
{
    setMinimumHeight(height);
    setMaximumHeight(height);
}

void SAGraphicsTitleBarButton::setFixedSize(const QSizeF &size)
{
    setMinimumSize(size);
    setMaximumSize(size);
}

void SAGraphicsTitleBarButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                     QWidget *widget)
{
    Q_UNUSED(widget);

    int darker = 120;
    int lighter = 140;

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    bool down = option->state & QStyle::State_Sunken;
    QRectF r = option->rect;
    QLinearGradient grad(r.topLeft(), r.bottomRight());
    grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ?
                    Qt::white: mBackgroundColor.lighter(lighter));
    grad.setColorAt(down ? 0 : 1, mBackgroundColor.darker(darker));
    painter->setPen(mBackgroundColor.darker(darker));
    painter->setBrush(grad);
    painter->drawEllipse(r);

    grad.setColorAt(down ? 1 : 0, mBackgroundColor.darker(darker));
    grad.setColorAt(down ? 0 : 1, mBackgroundColor.lighter(lighter));
    painter->setPen(Qt::NoPen);
    painter->setBrush(grad);
    if (down)
        painter->translate(1, 1);
    painter->drawEllipse(r.adjusted(1.5, 1.5, -1.5, -1.5));
}

void SAGraphicsTitleBarButton::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    update();
}

void SAGraphicsTitleBarButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    mChecked = !mChecked;
    emit clicked(mChecked);
    update();
}
