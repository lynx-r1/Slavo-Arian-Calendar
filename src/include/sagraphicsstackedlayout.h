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

#ifndef KDGRAPHICSSTACKEDLAYOUT_H
#define KDGRAPHICSSTACKEDLAYOUT_H

#include <QEasingCurve>
#include <QGraphicsLayout>
#include <QList>
#include <QObject>

#include <QAnimationGroup>

class SAGraphicsStackedLayout : public QObject, public QGraphicsLayout
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsLayout);

public:
    enum SwitchDirection {
        Top2Bottom,
        Bottom2Top,
        Left2Right,
        Right2Left
    };

    explicit SAGraphicsStackedLayout(QGraphicsLayoutItem *parent = 0);
    ~SAGraphicsStackedLayout();

    void setGeometry(const QRectF &rect);
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint=QSizeF()) const;
    int count() const { return mItems.count(); }
    QGraphicsLayoutItem *itemAt(int i) const;

    void insertItem(QGraphicsLayoutItem *item, int pos);
    void addItem(QGraphicsLayoutItem *item);

    void removeAt(int index);

    inline int currentIndex() const { return mCurrentWidgetIndex; }
    QGraphicsItem *currentWidget() const {
        return itemAt(mCurrentWidgetIndex)->graphicsItem();
    }

    inline QEasingCurve switchEasingCurve() const { return mSwitchEasingCurve; }
    inline void setSwitchEasingCurve(const QEasingCurve &curve) { mSwitchEasingCurve = curve; }

    inline int switchDuration() const { return mSwitchDuration; }
    inline void setSwitchDuration(int duration) { mSwitchDuration = duration; }

    inline SwitchDirection switchDirection() const { return mSwitchDirection; }
    inline void setSwitchDirection(SwitchDirection direction) { mSwitchDirection = direction; }

    inline bool silentSwitch() const { return mSilentSwitch; }
    inline void setSilentSwitch(bool silent) { mSilentSwitch = silent; }

public slots:
    void setCurrentIndex(int index);

signals:
    void currentIndexChanged(int);

private slots:
    void groupAnimationFinished();

private:
    QList<QGraphicsLayoutItem *> mItems;
    QEasingCurve mSwitchEasingCurve;

    SwitchDirection mSwitchDirection;

    int mSwitchDuration;
    int mCurrentWidgetIndex;
    bool mSilentSwitch;
};

#endif // KDGRAPHICSSTACKEDLAYOUT_H
