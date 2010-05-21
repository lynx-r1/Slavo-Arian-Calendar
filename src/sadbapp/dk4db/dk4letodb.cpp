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

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "sadefines.h"
#include "dk4letodb.h"

DK4LetoDB::DK4LetoDB()
    : DK4Leto()
{
    setJump(1);
}

DK4LetoDB::DK4LetoDB(int nomerLeta, int denNedeliNovoletiya, int krugLet, int stikhiyaICvet, int nachalo)
    : DK4Leto(nomerLeta, denNedeliNovoletiya, krugLet, stikhiyaICvet, nachalo)
{
    setJump(1);
}

void DK4LetoDB::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(widget);

    QFont saFont(SAFONTFAMILYIZHITSAC, 10);

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setClipRect(option->exposedRect);
    painter->setFont(saFont);

    QColor bgColor(backgroundColor());
    bool black = bgColor == Qt::black;
    if (black)
        bgColor = QColor(30, 30, 30);

    QColor lighter1 = bgColor.lighter(black ? 400 : 130);
    QColor lighter2 = bgColor.lighter(black ? 320 : 120);
    QColor darker1 = bgColor.darker(115);
    QColor darker2 = bgColor.darker(125);

    QRectF r = option->rect;
    painter->setPen(bgColor);

    bool mouseOver = option->state & QStyle::State_MouseOver;

    QLinearGradient lgr(r.topLeft(), r.bottomLeft());
    lgr.setColorAt(0, checked() ? mouseOver ? darker1 : darker2 :
                   mouseOver ? lighter1 : lighter2);
    lgr.setColorAt(0.4, checked() ? mouseOver ? lighter1 : lighter2 :
                   mouseOver ? darker1 : darker2);
    lgr.setColorAt(0.6, checked() ? mouseOver ? lighter1 : lighter2 :
                   mouseOver ? darker1 : darker2);
    lgr.setColorAt(1, checked() ? mouseOver ? darker1 : darker2 :
                   mouseOver ? lighter1 : lighter2);

    painter->setBrush(lgr);
    painter->drawRoundedRect(r, 3, 3);

    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::white, 0.01));
    painter->setOpacity(0.2);
    if (!checked())
        painter->drawRoundedRect(r.x(), r.y(), r.width(), r.height() / 2 - 2, 3, 3);

    painter->setOpacity(1);
    painter->setPen(color());
    painter->setBrush(lgr);
    painter->drawText(r.adjusted(3, 5, -2, -2), text());
}

DK4LetoDB *DK4LetoDB::clone(int nomerLeta, int denNedeliNovoletiya, int krugLet,
                            int stikhiyaICvet, int nachalo)
{
    return new DK4LetoDB(nomerLeta, denNedeliNovoletiya, krugLet, stikhiyaICvet, nachalo);
}
