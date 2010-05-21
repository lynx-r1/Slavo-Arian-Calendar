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
#include <QPainter>
#include <QToolTip>

#include "sadefines.h"

#include "dk4leto.h"

DK4Leto::DK4Leto()
    : SAGraphicsPushButton()
    , mNomerLeta(0)
    , mDenNedeliNovoletiya(0)
    , mKrugLet(0)
    , mStikhiyaICvet(0)
    , mNachalo(0)
{
    initToolTips();
    setCheckable(true);
    setJump(1.4);
}

DK4Leto::DK4Leto(int nomerLeta, int denNedeliNovoletiya, int krugLet, int stikhiyaICvet, int nachalo)
    : SAGraphicsPushButton(QString::number(nomerLeta), QColor(SACvetaHTMLCveta[stikhiyaICvet].second))
    , mColorLeta(SACvetaHTMLCveta[stikhiyaICvet].second)
    , mNomerLeta(nomerLeta)
    , mDenNedeliNovoletiya(denNedeliNovoletiya)
    , mKrugLet(krugLet)
    , mStikhiyaICvet(stikhiyaICvet)
    , mNachalo(nachalo)
{
    initToolTips();
    setCheckable(true);
    setJump(1.4);
}

DK4Leto *DK4Leto::clone(int nomerLeta, int denNedeliNovoletiya, int krugLet, int stikhiyaICvet, int nachalo)
{
    return new DK4Leto(nomerLeta, denNedeliNovoletiya, krugLet, stikhiyaICvet, nachalo);
}

void DK4Leto::initToolTips()
{
    mStihiyaCvet << tr("Земного (Черного)")
            << tr("Звёздного (Красного)")
            << tr("Огненного (Алого)")
            << tr("Солнечного (Златого)")
            << tr("Древесного (Зелёного)")
            << tr("Свага (Небесного)")
            << tr("Морского (Синего)")
            << tr("Лунного (Фиолетового)")
            << tr("Божественного (Белого)")
            ;

    mKrugiLet << tr("Странника (Пути)")
            << tr("Жреца")
            << tr("Жрицы (Девы)")
            << tr("Мира (Яви)")
            << tr("Свитка")
            << tr("Феникса")
            << tr("Лиса (Нави)")
            << tr("Дракона")
            << tr("Змеи")
            << tr("Орла")
            << tr("Дельфина")
            << tr("Коня")
            << tr("Пса")
            << tr("Тура (Коровы)")
            << tr("Хором (Дома)")
            << tr("Капища (Храма)");

    mDniNedeli << tr("в Понедельникъ")
            << tr("во Вторникъ")
            << tr("в Тритейникъ")
            << tr("в Четвергъ")
            << tr("в Пятницу")
            << tr("в Шестицу")
            << tr("в Седьмицу")
            << tr("в Осьмицу")
            << tr("в Неделю");

    QString cvetHtml = SACvetaHTMLCveta[mStikhiyaICvet].second;
    QString cvetHtmlText = QColor(cvetHtml).darker(200).name();

    QString nachalo = mNachalo ? tr("Мужское") : tr("Женское");
    QString stihiyaCvet;

    switch (mKrugLet) {
    case 2: case 8:
        stihiyaCvet = mStihiyaCvet[mStikhiyaICvet];
        stihiyaCvet.replace(tr("ого"), tr("ой"));
        stihiyaCvet.replace(tr("его"), tr("ей"));
        break;
    case 3: case 6: case 13: {
        stihiyaCvet = mStihiyaCvet[mStikhiyaICvet];
        int okonch = stihiyaCvet.lastIndexOf(tr("его"));
        stihiyaCvet.replace(okonch, 3, tr("ей"));
        if (okonch == -1) {
            okonch = stihiyaCvet.lastIndexOf(tr("ого"));
            stihiyaCvet.replace(okonch, 3, tr("ой"));
        }
        break;
    }
    case 14: {
        stihiyaCvet = mStihiyaCvet[mStikhiyaICvet];
        int okonch = stihiyaCvet.indexOf(tr("ого"));
        stihiyaCvet.replace(okonch, 3, tr("ых"));
        break;
    }
    default:
        stihiyaCvet = mStihiyaCvet[mStikhiyaICvet];
    }

    QString toolTip(tr("<p style='white-space:pre; color: %1'>"
                       "Лето <b>%2 %3</b>\n"
                       "Новолетие <b>%4</b>\n"
                       "Начало <b>%5</b>")
                    .arg(cvetHtmlText).arg(stihiyaCvet)
                    .arg(mKrugiLet[mKrugLet])
                    .arg(mDniNedeli[mDenNedeliNovoletiya])
                    .arg(nachalo)
                    );
    setToolTip(toolTip);
}
