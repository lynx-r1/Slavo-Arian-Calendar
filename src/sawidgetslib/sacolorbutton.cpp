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

#include "sacolorbutton.h"

SAColorButton::SAColorButton(QWidget *parent)
    : QPushButton(parent)
{
}

SAColorButton::SAColorButton(QString text, QWidget *parent)
    : QPushButton(text, parent)
{
}

void SAColorButton::setColor(const QColor &color)
{
    mColor = color;

    QPixmap pixmap(iconSize());
    pixmap.fill(QColor(0, 0, 0, 0));

    QRect rect(1, 1, iconSize().width() - 2, iconSize().height() - 2);

    // draw a slightly rounded rectangle
    QPainter p(&pixmap);
    p.setPen(Qt::NoPen);
    p.setRenderHints(p.renderHints() | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p.setBrush(color);
    p.drawRoundedRect(rect, 4, 4);
    p.end();

    // set this pixmap as icon
    setIcon(QIcon(pixmap));
}
