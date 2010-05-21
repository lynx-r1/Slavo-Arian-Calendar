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

#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QToolTip>

#include "kd.h"
#include "kdgraphicspushbutton.h"
#include "kdgraphicslabelitem.h"

#include "dennedeli.h"
#include "nachalasorokovnikov.h"

NachalaSorokovnikov::NachalaSorokovnikov(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , mNachSorok(2)
{
    setAcceptHoverEvents(true);

    mDniNachMesButtonGroup.setExclusive(true);
    mDniNachMesButtonGroup.setExclusive(true);

    setLayout(shemaNachalaSorokovnikov());
    layout()->setContentsMargins(0, 0, 10, 10);

    connect(&mDniNachMesButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(vyibratNovoletie(int)));

    connect(this, SIGNAL(vyibranoNovoletie(int,int,bool)),
            this, SLOT(test(int,int,bool)));
}

void NachalaSorokovnikov::vzyatInfo(int &mesyac, int &nachDen, bool &svyashchLeto)
{
    nachDen = mDniNachMesButtonGroup.id(mDniNachMesButtonGroup.checkedButton());

    if (mDniNachMesButtonGroup.checkedButton()) {
        QVariant proxyParent = mDniNachMesButtonGroup.checkedButton()->property("proxyParent");
        KDGraphicsPushButton *gpb = proxyParent.value<KDGraphicsPushButton *>();
        DenNedeli *den = qobject_cast<DenNedeli *>(gpb);
        if (den)
            den->vzyatInfo(mesyac, svyashchLeto);
    }
}

void NachalaSorokovnikov::ustanovitNovoletie(int novoletie)
{
    mNachSorok = novoletie;
    update();
}

void NachalaSorokovnikov::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/,
                              QWidget */*widget*/)
{
#if 1
    bool sv;
    int col, m, nd;
    vzyatInfo(m, nd, sv);

    if (sv) {
        if (mNachSorok == 1) col = KDCOLDNEYINEDELI + 1;
        else if (mNachSorok == 4) col = KDCOLDNEYINEDELI + 2;
        else if (mNachSorok == 7) col = KDCOLDNEYINEDELI + 3;
        else return;
    } else
        col = mNachSorok < 6 ? 2 * mNachSorok - 1 : 2 * mNachSorok - KDCOLDNEYINEDELI - 1;

    QGraphicsGridLayout *shemaNachSorokov = static_cast<QGraphicsGridLayout *>(layout());
    QPointF p1 = shemaNachSorokov->itemAt(1, col)->geometry().topLeft() - QPoint(1, 1);
    QPointF p2 = shemaNachSorokov->itemAt(shemaNachSorokov->rowCount() - 1,
                                          col)->geometry().bottomRight() + QPoint(1, 1);
    QRectF nachSorokRect(p1, p2);

    // рисуем прямоугольник, выделяющий текущее новолетие
    QColor c(0, 240, 190);
//    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setPen(c);
    painter->setBrush(c);
    painter->drawRect(nachSorokRect);
#endif
}

// ----------------------------- public slots ------------------------------ //
void NachalaSorokovnikov::vyibratNovoletie(int /*denNom*/)
{
    int mesyac, nachDen;
    bool svyashchLeto;
    vzyatInfo(mesyac, nachDen, svyashchLeto);

    emit vyibranoNovoletie(mesyac, nachDen, svyashchLeto);
}
#include <QDebug>
void NachalaSorokovnikov::test(int mesyac, int nachDen, bool svyashchLeto)
{
    qDebug() << (KDMesyaci[mesyac]) << (KDDniNedeli[nachDen]) << svyashchLeto;
}

// -------------------------------- private -------------------------------- //
QGraphicsLayout *NachalaSorokovnikov::shemaNachalaSorokovnikov()
{
    QGraphicsGridLayout *shemaNachSorokov = new QGraphicsGridLayout;
    shemaNachSorokov->setColumnSpacing(0, 3.0);;
    shemaNachSorokov->setSpacing(3.0);


    QSizeF denItemSize = DenNedeli().size();
    denItemSize.setWidth(denItemSize.width() + 5);

//    label->setToolTip(tr("Дни недели, с которыйх начинаются месяцы Лет Круголета"));

    // подписываем Месяцы
    KDGraphicsLabelItem *label = new KDGraphicsLabelItem(tr("Месяц"), font().family(), Qt::AlignCenter,
                                    "#99ff99", true);
    shemaNachSorokov->addItem(label, 0, 0);
    label->setMaximumHeight(denItemSize.height());
    label->setMinimumHeight(denItemSize.height());

    // названия месяцев
    for (int i = 1; i < KDCOLMESYACEV + 1; ++i) {
        label = new KDGraphicsLabelItem(QString::number(i) + ". " + KDMesyaci[i - 1],
                                        font().family(), Qt::AlignLeft | Qt::AlignVCenter,
                                        KDCvetaHTMLCveta[i - 1].second, true);
        shemaNachSorokov->addItem(label, i, 0);
        label->setMaximumHeight(denItemSize.height());
    }

    // подписываем простые лета
    label = new KDGraphicsLabelItem(tr("Простые Лета"), font().family(),
                                    Qt::AlignCenter, "#9999ff", true);
    label->setMaximumHeight(denItemSize.height());
    label->setMinimumHeight(denItemSize.height());
    shemaNachSorokov->addItem(label, 0, 1, 1, KDCOLMESYACEV);

    // подписываем священные лета
    label = new KDGraphicsLabelItem(tr("Священные Лета"), font().family(),
                                    Qt::AlignCenter, "#ff9999", true);
    label->setMaximumHeight(denItemSize.height());
    label->setMinimumHeight(denItemSize.height());
    shemaNachSorokov->addItem(label, 0, KDCOLMESYACEV + 1, 1, 3);

    // рисуем таблицы простых лет
    QGraphicsWidget *den;
    for (int i = 1; i < KDCOLMESYACEV + 1; i += 2) {
        for (int j = 1, k = 0; j < KDCOLDNEYINEDELI + 1; j += 2, ++k) {
            den = dobavitDenNedeli(denItemSize, KDDniNedeli[k], k, i - 1);
            shemaNachSorokov->addItem(den, i, j);

            if (j != KDCOLDNEYINEDELI) {
                den = dobavitDenNedeli(denItemSize, KDDniNedeli[k + 5], k + 5, i - 1);
                shemaNachSorokov->addItem(den, i, j + 1);
            }
        }

        if (i != KDCOLMESYACEV) {
            for (int j = 1, k = 1; j < KDCOLDNEYINEDELI + 1; j += 2, ++k) {
                if (j != KDCOLDNEYINEDELI) {
                    den = dobavitDenNedeli(denItemSize, KDDniNedeli[k + 4], k + 4, i);
                    shemaNachSorokov->addItem(den, i + 1, j);
                } else {
                    den = dobavitDenNedeli(denItemSize, KDDniNedeli[0], 0, i);
                    shemaNachSorokov->addItem(den, i + 1, j);
                }

                if (j != KDCOLDNEYINEDELI) {
                    den = dobavitDenNedeli(denItemSize, KDDniNedeli[k], k, i);
                    shemaNachSorokov->addItem(den, i + 1, j + 1);
                }
            }
        }
    }

    // рисуем таблицу священных лет
    for (int j = KDCOLDNEYINEDELI + 1; j < KDCOLDNEYINEDELI + 4; ++j) {
        int k = 3 * (j - KDCOLDNEYINEDELI - 1) + 1;
        for (int i = 0; i < KDCOLDNEYINEDELI - 4; ++i) {
            den = dobavitDenNedeli(denItemSize, KDDniNedeli[i], i, k - 1, true);
            shemaNachSorokov->addItem(den, k, j);

            if (i + 5 < KDCOLDNEYINEDELI) {
                int n = k + 1 > KDCOLDNEYINEDELI ? 1 : k + 1;
                den = dobavitDenNedeli(denItemSize, KDDniNedeli[i + 5], i + 5, n - 1, true);
                shemaNachSorokov->addItem(den, n, j);
            }

            k += 2;
            if (k > KDCOLDNEYINEDELI)
                k -= KDCOLDNEYINEDELI;
        }
    }

    return shemaNachSorokov;
}

QGraphicsWidget *NachalaSorokovnikov::dobavitDenNedeli(QSizeF size, QString den, int nom,
                                                     int mesyac, bool svyashch)
{
    QRegExp rx("[аеёиоуыэюя]", Qt::CaseInsensitive);
    QString d(den[0]);
    d += rx.exactMatch(den.mid(1, 1)) ? den[2] : den[1];

    QColor cvet(KDCvetaHTMLCveta[nom].second);
    DenNedeli *dn = new DenNedeli(d + ".", den, cvet, mesyac, svyashch);
    dn->setMaximumSize(size);

    QAbstractButton *b = qobject_cast<QAbstractButton *>(dn->widget());
    mDniNachMesButtonGroup.addButton(b, nom);

    return dn;
}
