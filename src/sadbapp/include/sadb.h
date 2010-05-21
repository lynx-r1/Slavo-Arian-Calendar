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

#ifndef DB_H
#define DB_H

#include <QCoreApplication>
#include <QVector>
#include <stdlib.h>
//#include <vector>

static const int DBTIMERINTERVAL = 3;

typedef const char *DBDenNedeli;
typedef const char *DBMesyac;
typedef const char *DBKrugiLeta;

typedef std::pair<const char *, const char *> DBChasOpisanie;
typedef std::pair<const char *, const char *> DBChetvertSutokCvet;
typedef std::pair<const char *, QVector<int> > DBDenNedeliNovoletie;
typedef std::pair<const char *, const char *> DBCvetHTMLCvet;
typedef std::pair<const char *, const char *> DBStihiyaCvet;


static const int DBCOLDNEYINEDELI = 9;
static const int DBCOLDNEYIVPOLNOMMESYACE = 41;
static const int DBCOLDNEYIVNEPOLNOMMESYACE = 40;
static const int DBCOLMESYACEV = 9;

static const int DBKRUGLET = 16;
static const int DBKRUGZGIZNI = 144;

static const int DBCOLCHASOV = 16;
static const int DBCOLCHETVERTEISUTOK = 4;
static const int DBCOLSTIHIYI = 9;
static const int DBCOLCVETOV = 9;

static const DBDenNedeli DBDniNedeli[] = {
    QT_TRANSLATE_NOOP("Дни недели", "Понедельник"),
    QT_TRANSLATE_NOOP("Дни недели", "Вторник"),
    QT_TRANSLATE_NOOP("Дни недели", "Тритейник"),
    QT_TRANSLATE_NOOP("Дни недели", "Четверг"),
    QT_TRANSLATE_NOOP("Дни недели", "Пятница"),
    QT_TRANSLATE_NOOP("Дни недели", "Шестица"),
    QT_TRANSLATE_NOOP("Дни недели", "Седьмица"),
    QT_TRANSLATE_NOOP("Дни недели", "Осьмица"),
    QT_TRANSLATE_NOOP("Дни недели", "Неделя")
    };

static const DBMesyac DBMesyaci[] = {
    QT_TRANSLATE_NOOP("Месяцы", "Рамхатъ"),
    QT_TRANSLATE_NOOP("Месяцы", "Айлѣтъ"),
    QT_TRANSLATE_NOOP("Месяцы", "Бэйлѣтъ"),
    QT_TRANSLATE_NOOP("Месяцы", "Гэйлѣтъ"),
    QT_TRANSLATE_NOOP("Месяцы", "Дайлѣтъ"),
    QT_TRANSLATE_NOOP("Месяцы", "Элѣтъ"),
    QT_TRANSLATE_NOOP("Месяцы", "Вэйлѣтъ"),
    QT_TRANSLATE_NOOP("Месяцы", "Хэйлѣтъ"),
    QT_TRANSLATE_NOOP("Месяцы", "Тайлѣтъ"),
};

static const DBKrugiLeta DBKrugiLet[] = {
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

static const DBChasOpisanie DBChasy_Opisaniya[] = {
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Паобѣдъ"),
                   QT_TRANSLATE_NOOP("Описание часа", "Начало нового дня")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Вѣчиръ"),
                   QT_TRANSLATE_NOOP("Описание часа", "Появление звёздной росы на Небесах")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Ничь"),
                   QT_TRANSLATE_NOOP("Описание часа", "Нечётное время 3-х Лун")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Полничь"),
                   QT_TRANSLATE_NOOP("Описание часа", "Полный путь Лун")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Заутра"),
                   QT_TRANSLATE_NOOP("Описание часа", "Звёздное утешение росы")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Заура"),
                   QT_TRANSLATE_NOOP("Описание часа", "Звёздное сияние, заря")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Заурнице"),
                   QT_TRANSLATE_NOOP("Описание часа", "Окончание звёздного сияния")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Настя"),
                   QT_TRANSLATE_NOOP("Описание часа", "Утренняя роса")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Сваоръ"),
                   QT_TRANSLATE_NOOP("Описание часа", "Восход Солнца")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Утрось"),
                   QT_TRANSLATE_NOOP("Описание часа", "Успокоение росы")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Поутрось"),
                   QT_TRANSLATE_NOOP("Описание часа", "Путь собирания успокоенной росы")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Обестина"),
                   QT_TRANSLATE_NOOP("Описание часа", "Обедня, совместное собрание")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Обесть"),
                   QT_TRANSLATE_NOOP("Описание часа", "Трапеза, время принятия пищи")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Подани"),
                   QT_TRANSLATE_NOOP("Описание часа", "Отдых после трапезы")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Утдайни"),
                   QT_TRANSLATE_NOOP("Описание часа", "Время окончания деяний")),
    DBChasOpisanie(QT_TRANSLATE_NOOP("Часы", "Поудани"),
                   QT_TRANSLATE_NOOP("Описание часа", "Завершённый день"))
};

static const DBChetvertSutokCvet DBChetvertySutok_Cveta[] = {
    DBChetvertSutokCvet(QT_TRANSLATE_NOOP("Цвет суток", "Вечер"), "#00ffff"),
    DBChetvertSutokCvet(QT_TRANSLATE_NOOP("Цвет суток", "Ночь"), "#1e1eaa"),
    DBChetvertSutokCvet(QT_TRANSLATE_NOOP("Цвет суток", "Утро"), "#ffc800"),
    DBChetvertSutokCvet(QT_TRANSLATE_NOOP("Цвет суток", "День"), "#ee0000")
};

static const DBDenNedeliNovoletie DBDenNedeli_Novoletie[] = {
    DBDenNedeliNovoletie(DBDniNedeli[0], QVector<int>() << 1 << 10 << 20
                         << 29 << 39 << 48 << 49 << 58 << 68 << 77 << 87
                         << 96 << 97 << 106 << 116 << 125 << 135 << 144),
            DBDenNedeliNovoletie(DBDniNedeli[1], QVector<int>() << 3 << 12 << 22
                                 << 31 << 41 << 51 << 60 << 70 << 79 << 89 << 99
                                 << 108 << 118 << 127 << 137),
            DBDenNedeliNovoletie(DBDniNedeli[2], QVector<int>() << 5 << 14 << 24
                                 << 34 << 43 << 53 << 62 << 72 << 82 << 91 << 101
                                 << 110 << 120 << 130 << 139),
            DBDenNedeliNovoletie(DBDniNedeli[3], QVector<int>() << 7 << 16 << 17
                                 << 26 << 36 << 45 << 55 << 64 << 65 << 74 << 84
                                 << 93 << 103 << 112 << 113 << 122 << 132 << 141),
            DBDenNedeliNovoletie(DBDniNedeli[4], QVector<int>() << 9 << 19 << 28 << 38
                                 << 47 << 57 << 67 << 76 << 86 << 95 << 105 << 115
                                 << 124 << 134 << 143),
            DBDenNedeliNovoletie(DBDniNedeli[5], QVector<int>() << 2 << 11 << 21 << 30
                                 << 40 << 50 << 59 << 69 << 78 << 88 << 98 << 107
                                 << 117 << 126 << 136),
            DBDenNedeliNovoletie(DBDniNedeli[6], QVector<int>() << 4 << 13 << 23 << 32
                                 << 33 << 42 << 52 << 61 << 71 << 80 << 81 << 90
                                 << 100 << 109 << 119 << 128 << 129 << 138),
            DBDenNedeliNovoletie(DBDniNedeli[7], QVector<int>() << 6 << 15 << 25
                                 <<35 << 44 << 54 << 63 << 73 << 83 << 92 << 102
                                 << 111 << 121 << 131 << 140),
            DBDenNedeliNovoletie(DBDniNedeli[8], QVector<int>() << 8 << 18 << 27 << 37
                                 << 46 << 56 << 66 << 75 << 85 << 94 << 104 << 114
                                 << 123 << 133 << 142)
        };

static const DBCvetHTMLCvet DBCvetaHTMLCveta[] = {
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Черный"), "#000000"),
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Красный"), "#ee0000"),
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Алый"), "#f0a0a0"),
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Златый"), "#fafa07"),
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Зеленый"), "#96f096"),
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Небесный"), "#aadce6"),
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Синий"), "#8282fa"),
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Фиолѣт"), "#c891ff"),
    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Белый"), "#ffffff")
    //    DBCvetHTMLCvet(QT_TRANSLATE_NOOP("Цвета", "Фиолѣтовый"), "#c891ff"),
};

static const DBStihiyaCvet DBStihiya_Cvet[] = {
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Земля"), DBCvetaHTMLCveta[0].first),
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Звезда"), DBCvetaHTMLCveta[1].first),
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Огонь"), DBCvetaHTMLCveta[2].first),
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Солнце"), DBCvetaHTMLCveta[3].first),
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Дерево"), DBCvetaHTMLCveta[4].first),
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Свага"), DBCvetaHTMLCveta[5].first),
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Океан"), DBCvetaHTMLCveta[6].first),
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Луна"), DBCvetaHTMLCveta[7].first),
    DBStihiyaCvet(QT_TRANSLATE_NOOP("Стихии", "Бог"), DBCvetaHTMLCveta[8].first)
};

#endif // DB_H
