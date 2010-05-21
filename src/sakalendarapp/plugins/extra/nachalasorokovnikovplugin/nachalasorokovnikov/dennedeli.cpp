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
#include <QToolTip>

#include "kd.h"

#include "dennedeli.h"

DenNedeli::DenNedeli()
    : KDGraphicsPushButton()
{
}

DenNedeli::DenNedeli(QString nazvanie, QString podskazka, QColor cvet, int mesyac, bool svyashch)
    : KDGraphicsPushButton(nazvanie, cvet)
    , mPodskazka(podskazka)
    , mCvet(cvet)
    , mMesyac(mesyac)
    , mSvyashch(svyashch)
{
}

void DenNedeli::vzyatInfo(int &mesyac, bool &svyashch)
{
    mesyac = mMesyac;
    svyashch = mSvyashch;
}

void DenNedeli::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QString letoTip = mSvyashch ? tr("Священное Лето") : tr("Простое Лето");
    QString toolTip(tr("<p style='white-space:pre; color :%1'>%2\n"
                       "%3\n%4")
                    .arg(mCvet.darker(200).name())
                    .arg(mPodskazka).arg(KDMesyaci[mMesyac]).arg(letoTip));
    QToolTip::showText(event->screenPos(), toolTip);
    QGraphicsProxyWidget::hoverEnterEvent(event);
}
