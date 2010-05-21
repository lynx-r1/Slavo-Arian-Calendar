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

#ifndef SADEFINES_H
#define SADEFINES_H

#include <QCoreApplication>
#include <stdlib.h>

typedef const char *SAKrugiLeta;

typedef std::pair<const char *, const char *> SADenNedeliOpisanie;
typedef std::pair<const char *, const char *> SAMesyacOpisanie;
typedef std::pair<const char *, const char *> SAChasOpisanie;
typedef std::pair<const char *, const char *> SAChetvertSutokCvet;
typedef std::pair<const char *, const char *> SACvetHTMLCvet;
typedef std::pair<const char *, const char *> SAStihiyaCvet;

static const int SATIMERINTERVAL            = 3;

static const int SACOLDNEYINEDELI           = 9;
static const int SACOLDNEYIVPOLNOMMESYACE   = 41;
static const int SACOLDNEYIVNEPOLNOMMESYACE = 40;
static const int SACOLMESYACEV              = 9;

static const int SAKRUGLET                  = 16;
static const int SAKRUGZGIZNI               = 144;
static const int SUTKISWAROGA               = 180;

static const int SACOLCHASOV                = 16;
static const int SACOLCHASTEI               = 144;
static const int SACOLDOLI                  = 1296;

static const int SACOLCHETVERTEISUTOK       = 4;
static const int SACOLSTIHIYI               = 9;
static const int SACOLCVETOV                = 9;

static const QString SADBPRAZDNIKINAME = QString("prazdniki");
static const QString SADBOPISANIELETNAME = QString("opisanielet");
static const QString SAFONTFAMILYIZHITSAC = QLatin1String("IzhitsaC");

static const SADenNedeliOpisanie SADniNedeli_Opisanie[] = {
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Понедельник"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День труда")),
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Вторник"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День труда")),
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Тритейник"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День отдыха, пост")),
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Четверг"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День труда")),
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Пятница"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День труда")),
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Шестица"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День отдыха, пост")),
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Седьмица"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День отдыха, пост")),
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Осьмица"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День труда")),
    SADenNedeliOpisanie(QT_TRANSLATE_NOOP("Дни недели", "Неделя"),
                        QT_TRANSLATE_NOOP("Дни недели описание", "День гостей"))
    };

static const SAMesyacOpisanie SAMesyaci_Opisanie[] = {
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Рамхатъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Божественного Начала")),
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Айлhтъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Новых Даров")),
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Бэйлhтъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Белого Сияния и Покоя Мира")),
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Гэйлhтъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Вьюг и Стужи")),
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Дайлhтъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Пробуждения Природы")),
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Элhтъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Посева и Наречения")),
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Вэйлhтъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Ветров")),
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Хэйлhтъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Получения Даров Природы")),
    SAMesyacOpisanie(QT_TRANSLATE_NOOP("Месяцы", "Тайлhтъ"),
                     QT_TRANSLATE_NOOP("Месяцы описание", "Сороковник Завершения"))
};

static const SAKrugiLeta SAKrugiLet[] = {
    QT_TRANSLATE_NOOP("Круги лет", "Странник\n(Путь)"),
    QT_TRANSLATE_NOOP("Круги лет", "Жрец"),
    QT_TRANSLATE_NOOP("Круги лет", "Жрица\n(Дева)"),
    QT_TRANSLATE_NOOP("Круги лет", "Мир\n(Явь)"),
    QT_TRANSLATE_NOOP("Круги лет", "Свиток"),
    QT_TRANSLATE_NOOP("Круги лет", "Феникс"),
    QT_TRANSLATE_NOOP("Круги лет", "Лис\n(Навь)"),
    QT_TRANSLATE_NOOP("Круги лет", "Дракон"),
    QT_TRANSLATE_NOOP("Круги лет", "Змей"),
    QT_TRANSLATE_NOOP("Круги лет", "Орёл"),
    QT_TRANSLATE_NOOP("Круги лет", "Дельфин"),
    QT_TRANSLATE_NOOP("Круги лет", "Конь"),
    QT_TRANSLATE_NOOP("Круги лет", "Пёс"),
    QT_TRANSLATE_NOOP("Круги лет", "Тур\n(Корова)"),
    QT_TRANSLATE_NOOP("Круги лет", "Хоромы\n(Дом)"),
    QT_TRANSLATE_NOOP("Круги лет", "Капище\n(Храм)")
    };

static const SAChasOpisanie SAChasy_Opisaniya[] = {
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Паобhдъ"),
                   QT_TRANSLATE_NOOP("Описание часа", "Начало нового дня")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Вhчиръ"),
                   QT_TRANSLATE_NOOP("Описание часа", "Появление звёздной росы на Небесах")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Ничь"),
                   QT_TRANSLATE_NOOP("Описание часа", "Нечётное время 3-х Лун")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Полничь"),
                   QT_TRANSLATE_NOOP("Описание часа", "Полный путь Лун")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Заутра"),
                   QT_TRANSLATE_NOOP("Описание часа", "Звёздное утешение росы")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Заура"),
                   QT_TRANSLATE_NOOP("Описание часа", "Звёздное сияние, заря")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Заурнице"),
                   QT_TRANSLATE_NOOP("Описание часа", "Окончание звёздного сияния")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Настя"),
                   QT_TRANSLATE_NOOP("Описание часа", "Утренняя роса")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Сваоръ"),
                   QT_TRANSLATE_NOOP("Описание часа", "Восход Солнца")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Утрось"),
                   QT_TRANSLATE_NOOP("Описание часа", "Успокоение росы")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Поутрось"),
                   QT_TRANSLATE_NOOP("Описание часа", "Путь собирания успокоенной росы")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Обестина"),
                   QT_TRANSLATE_NOOP("Описание часа", "Обедня, совместное собрание")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Обесть"),
                   QT_TRANSLATE_NOOP("Описание часа", "Трапеза, время принятия пищи")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Подани"),
                   QT_TRANSLATE_NOOP("Описание часа", "Отдых после трапезы")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Утдайни"),
                   QT_TRANSLATE_NOOP("Описание часа", "Время окончания деяний")),
    SAChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Поудани"),
                   QT_TRANSLATE_NOOP("Описание часа", "Завершённый день"))
};

static const SAChetvertSutokCvet SAChetvertySutok_Cveta[] = {
    SAChetvertSutokCvet(QT_TRANSLATE_NOOP("Цвет суток", "Вечер"), "#00ffff"),
    SAChetvertSutokCvet(QT_TRANSLATE_NOOP("Цвет суток", "Ночь"), "#1e1eaa"),
    SAChetvertSutokCvet(QT_TRANSLATE_NOOP("Цвет суток", "Утро"), "#ffc800"),
    SAChetvertSutokCvet(QT_TRANSLATE_NOOP("Цвет суток", "День"), "#ee0000")
};

static const SACvetHTMLCvet SACvetaHTMLCveta[] = {
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Черный"), "#000000"),
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Красный"), "#dd0000"),
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Алый"), "#f0a0a0"),
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Златый"), "#fafa07"),
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Зеленый"), "#96f096"),
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Небесный"), "#aadce6"),
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Синий"), "#8282fa"),
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Фиолhт"), "#c891ff"),
    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Белый"), "#ffffff")
    //    SACvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Фиолѣтовый"), "#c891ff"),
};

static const SAStihiyaCvet SAStihiya_Cvet[] = {
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Земля"), SACvetaHTMLCveta[0].first),
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Звезда"), SACvetaHTMLCveta[1].first),
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Огонь"), SACvetaHTMLCveta[2].first),
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Солнце"), SACvetaHTMLCveta[3].first),
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Дерево"), SACvetaHTMLCveta[4].first),
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Свага"), SACvetaHTMLCveta[5].first),
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Океан"), SACvetaHTMLCveta[6].first),
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Луна"), SACvetaHTMLCveta[7].first),
    SAStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Бог"), SACvetaHTMLCveta[8].first)
};

#endif // SADEFINES_H
