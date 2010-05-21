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

#ifndef KDGRAPHICSLABELITEM_H
#define KDGRAPHICSLABELITEM_H

#include <QGraphicsWidget>

class SAGraphicsLabelItem : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAGraphicsLabelItem(const QString &podpis, const QFont &shrift = QFont(),
                        int vyiravn = Qt::AlignCenter, const QString &cvet = "#000000",
                        bool fone = false);

    void setText(const QString &text) { mPodpis = text; }

    void setColor(const char *color) { mCvet = color; }
    void setColor(const QColor &color) { mCvet = color.name(); }

    void setFixedWidth(qreal width);
    void setFixedHeight(qreal height);
    void setFixedSize(const QSizeF &size);
    void setFixedSize(qreal width, qreal height);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void clicked();

private:
    QString mPodpis;
    QFont mShrift;
    int mVyiravn;
    QString mCvet;
    bool mFone;
};

#endif // KDGRAPHICSLABELITEM_H
