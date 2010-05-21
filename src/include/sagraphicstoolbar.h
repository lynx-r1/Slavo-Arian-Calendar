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

#ifndef SAGRAPHICSTOOLBAR_H
#define SAGRAPHICSTOOLBAR_H

#include <QGraphicsWidget>

QT_FORWARD_DECLARE_CLASS(QGraphicsBlurEffect)
QT_FORWARD_DECLARE_CLASS(QGraphicsLinearLayout)

class SAGraphicsToolBar : public QGraphicsWidget
{
public:
    SAGraphicsToolBar(const QColor &color = Qt::transparent, QGraphicsWidget *parent = 0);

    void addWidget(QGraphicsWidget *widget);
    void addSeparator();

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    inline QColor color() const { return mColor; }
    inline void setColor(const QColor &color) {
        mColor = color;
        update();
    }

    inline bool hideOnHoverLeave() const { return mHideOnHoverLeave; }
    inline void setHideOnHoverLeave(bool hide) { mHideOnHoverLeave = hide; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    class Separator : public QGraphicsWidget
    {
    public:
        Separator(Qt::Orientation orientation, const QColor &color = QColor(64, 64, 64, 127),
                  SAGraphicsToolBar *parent = 0);

        inline QColor color() const { return mColor; }
        inline void setColor(const QColor &color) {
            mColor = color;
            update();
        }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget = 0);

    private:
        Qt::Orientation mOrientation;
        QColor mColor;
    };

    QGraphicsBlurEffect *mBlurEffect;
    QList<QGraphicsWidget *> mWidgets;
    QGraphicsLinearLayout *mLayout;
    QColor mColor;

    bool mHideOnHoverLeave;
};

#endif // SAGRAPHICSTOOLBAR_H
