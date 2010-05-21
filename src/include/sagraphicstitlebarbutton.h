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

#ifndef KDGRAPHICSTITLEBARBUTTON_H
#define KDGRAPHICSTITLEBARBUTTON_H

#include <QGraphicsWidget>

class SAGraphicsTitleBarButton : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAGraphicsTitleBarButton(const QColor &color, QGraphicsItem *parent = 0);

    bool checked() const;
    void setChecked(bool checked);

    void setFixedWidth(qreal width);
    void setFixedHeight(qreal height);
    void setFixedSize(const QSizeF &size);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    QPainterPath shape() const;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void clicked(bool);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

private:
    QSize mSize;
    QColor mColor;
    QColor mBackgroundColor;

    bool mChecked;
};

#endif // KDGRAPHICSTITLEBARBUTTON_H
