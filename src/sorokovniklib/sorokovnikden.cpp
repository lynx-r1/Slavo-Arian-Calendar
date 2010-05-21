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

#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "sadefines.h"

#include "sorokovnikden.h"

const QColor VESHII_SON_COLOR = QColor(88, 79, 161);
const QColor RODITELSKII_DEN_COLOR = QColor(1, 169, 103);
const QColor DEN_POCHITANIYA_PREDKOV_COLOR = QColor(180, 30, 30);

SorokovnikDen::SorokovnikDen()
    : SAGraphicsPushButton()
    , mPodskazka("")
    , mPrazdnik("")
    , mOpisaniePrazdnika("")
    , mCvet("")
    , mSegodnya(false)
    , mVeshchiiSon(false)
    , mRoditelskiiDen(false)
    , mDenPochitaniyaPredkov(false)
    , mVazhnyiPrazdnik(false)
    , mSvyatyiPost(false)
    , mDazhdbozhyiPost(false)
    , mChistyiPost(false)
    , mVelikiiPost(false)
    , mSvetlyiPost(false)
{
    setCheckable(true);
    setJump(1.4);
}

SorokovnikDen::SorokovnikDen(const QString &nazvanie, const QString &podskazka,
                             const QColor &cvet)
                                 : SAGraphicsPushButton(nazvanie, cvet)
                                 , mPodskazka(podskazka)
                                 , mPrazdnik("")
                                 , mOpisaniePrazdnika("")
                                 , mCvet(cvet)
                                 , mSegodnya(false)
                                 , mVeshchiiSon(false)
                                 , mRoditelskiiDen(false)
                                 , mDenPochitaniyaPredkov(false)
                                 , mVazhnyiPrazdnik(false)
                                 , mSvyatyiPost(false)
                                 , mDazhdbozhyiPost(false)
                                 , mChistyiPost(false)
                                 , mVelikiiPost(false)
                                 , mSvetlyiPost(false)
{
    setCheckable(true);
    setJump(1.4);
}

SorokovnikDen *SorokovnikDen::clone(const QString &nazvanie, const QString &podskazka,
                                    const QColor &cvet)
{
    return new SorokovnikDen(nazvanie, podskazka, cvet);
}

void SorokovnikDen::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    SAGraphicsPushButton::paint(painter, option, widget);

    // рисуем обозначения дня: Вещие сны, Родительские дни, Дни памяти и почитания предков
    int x = 2;
    int fontSide = 12;
#if defined(Q_OS_UNIX)
    int textAlignFlags = Qt::AlignHCenter | Qt::AlignBottom;
#elif defined(Q_OS_WIN)
    int textAlignFlags = Qt::AlignCenter;
#endif

    QString text;
    QRectF r(option->rect);
    QFont f(SAFONTFAMILYIZHITSAC, 10);
    painter->setFont(f);

    if (isVazhnyiPrazdnik()) {
        QColor red(255, 80, 80, 127);
        painter->setPen(Qt::NoPen);
        painter->setBrush(red);
        painter->drawRoundedRect(r.adjusted(2, 2, -2, -2), 3, 3);
    }

    if (isVeshchiiSon()) {
        painter->setPen(VESHII_SON_COLOR);
        text = tr("с");
        QRectF rt(QRectF(x, r.height() / 2 + 1, fontSide, fontSide));
        painter->drawEllipse(rt);
        painter->drawText(rt, textAlignFlags, text);
        x += 14;
    }

    if (isRoditelskiiDen()) {
        painter->setPen(RODITELSKII_DEN_COLOR);
        text = tr("р");
        QRectF rt(QRectF(x, r.height() / 2 + 1, fontSide, fontSide));
        painter->drawEllipse(rt);
         painter->drawText(rt, textAlignFlags, text);
        x += 14;
    }

    if (isDenPochitaniyaPredkov()) {
        painter->setPen(DEN_POCHITANIYA_PREDKOV_COLOR);
        text = tr("п");
        QRectF rt(QRectF(x, r.height() / 2 + 1, fontSide, fontSide));
        painter->drawEllipse(rt);
        painter->drawText(rt, textAlignFlags, text);
    }
}

void SorokovnikDen::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    SAGraphicsPushButton::hoverEnterEvent(event);
}

void SorokovnikDen::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    SAGraphicsPushButton::hoverLeaveEvent(event);
    update();
}
