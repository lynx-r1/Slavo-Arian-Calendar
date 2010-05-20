#include <QProgressDialog>
#include <QStringList>
#include <QSqlQuery>
#include <QVariant>

#include "prazdniki_db.h"

void insertHoliday(QSqlQuery &q, int id, const QString &name, const QString &descr, int rang)
{
    q.addBindValue(id);
    q.addBindValue(name);
    q.addBindValue(descr);
    q.addBindValue(rang);
    q.exec();
}

void insertRecord(QSqlQuery &q, int id, int count)
{
    q.addBindValue(id);
    q.addBindValue(count);
    q.exec();
}

void updateHoliday(QSqlQuery &q, int id, const QString &name, const QString &descr, int rang)
{
    q.addBindValue(name);
    q.addBindValue(descr);
    q.addBindValue(rang);
    q.addBindValue(id);
    q.exec();
}

namespace DBPrazdniki
{
    QSqlError openDB(const QString &fileName, const QString &connectionName)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(fileName);

        if (!db.open())
            return db.lastError();
        return QSqlError();
    }

    QSqlError createDB(QSqlDatabase db, bool svyasch)
    {
        QProgressDialog progress(QT_TR_NOOP("Создание базы данных праздников"), "", 0, 740);
        progress.setWindowTitle(QT_TR_NOOP("База данных праздников"));
        progress.setCancelButton(0);
        progress.show();

        QSqlQuery q(db);
        if (!q.exec(QString("CREATE TABLE ramkhato (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(0);

        if (!q.exec(QString("CREATE TABLE aileto (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(1);

        if (!q.exec(QString("CREATE TABLE beileto (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(2);

        if (!q.exec(QString("CREATE TABLE geileto (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(3);

        if (!q.exec(QString("CREATE TABLE daileto (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(4);

        if (!q.exec(QString("CREATE TABLE eileto (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(5);

        if (!q.exec(QString("CREATE TABLE veileto (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(6);

        if (!q.exec(QString("CREATE TABLE heileto (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(7);

        if (!q.exec(QString("CREATE TABLE taileto (id integer primary key, count integer)")))
            return q.lastError();
        progress.setValue(8);

        if (!q.exec(QString("CREATE TABLE slavo_arian_holid (id integer primary key, name text, descr text, rang integer)")))
            return q.lastError();
        progress.setValue(9);

        if (!q.prepare(QString("INSERT INTO slavo_arian_holid (id, name, descr, rang) VALUES(?, ?, ?, ?)")))
            return q.lastError();
        for (int i = 1; i <= 365; ++i) {
            insertHoliday(q, i, QString(""), QString(""), 0);
            progress.setValue(9 + i);
        }

        int k = 1;
        QStringList list = QStringList() << "ramkhato" << "aileto" << "beileto" << "geileto"
                           << "daileto" << "eileto" << "veileto" << "heileto" << "taileto";
        for (int i = 1; i <= 9; ++i) {
            int s = svyasch ? 41 : i % 2 ? 41 : 40;
            for (int j = 1; j <= s; ++j) {
                if (!q.prepare(QString("INSERT INTO %1 (id, count) VALUES(?, ?)").arg(list.at(i - 1))))
                    return q.lastError();
                insertRecord(q, j, k);
                progress.setValue(374 + k);
                ++k;
            }
        }

        return QSqlError();
    }

    QSqlError populateDB(QSqlDatabase db, bool svyashch)
    {
        Q_UNUSED(svyashch);

        QProgressDialog progress(QT_TR_NOOP("Заполнение базы данных праздников"), QT_TR_NOOP("Отмена"), 1, 365);
        progress.setWindowTitle(QT_TR_NOOP("База данных праздников"));
        progress.setCancelButton(0);
        progress.show();

        QSqlQuery q(db);
        if (!q.prepare(QString("UPDATE slavo_arian_holid SET name=?, descr=?, rang=? WHERE id=?")))
            return q.lastError();
        updateHoliday(q, 1, QT_TR_NOOP("Новолетие"), QT_TR_NOOP("Рамха-Ита (Новолетие).\nОвсень. Осеннее равноденствие. В праздник Новолетия "
                                                                "с каждым ударом Большого колокола человеку даётся возможность избавиться от "
                                                                "одного греха (Большой колокол бъёт в этот праздник ровно 108 раз), и в течении "
                                                                "первого месяца Новго Лета человек, совершивший неблаговидные поступки, должен "
                                                                "исправить сложившееся положение. Устраивается пир-братчина, пьётся медовая "
                                                                "Сурья. Некоторые роды отмечали новолетие на Коляду или Красногор."), 8);
        progress.setValue(1);

        updateHoliday(q, 2,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(2);

        updateHoliday(q, 3,  QT_TR_NOOP(""), QT_TR_NOOP(""), 1);
        progress.setValue(3);

        updateHoliday(q, 4,  QT_TR_NOOP("День Ивана Омийского"), QT_TR_NOOP(""), 0);
        progress.setValue(4);

        updateHoliday(q, 5,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(5);

        updateHoliday(q, 6,  QT_TR_NOOP("Праздник Урожая"), QT_TR_NOOP(""), 9);
        progress.setValue(6);

        updateHoliday(q, 7,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(7);

        updateHoliday(q, 8,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(8);

        updateHoliday(q, 9,  QT_TR_NOOP("День Сергия Странника"), QT_TR_NOOP(""), 4);
        progress.setValue(9);

        updateHoliday(q, 10,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(10);

        updateHoliday(q, 11,  QT_TR_NOOP("День Ольги Премудрой"), QT_TR_NOOP(""), 0);
        progress.setValue(11);

        updateHoliday(q, 12,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(12);

        updateHoliday(q, 13,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(13);

        updateHoliday(q, 14,  QT_TR_NOOP("День Великой Рассении"), QT_TR_NOOP("Рассения - территория, по которой расселялись роды Свята Расы."), 10);
        progress.setValue(14);

        updateHoliday(q, 15,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(15);

        updateHoliday(q, 16,  QT_TR_NOOP("День Василия Буслаева"), QT_TR_NOOP(""), 4);
        progress.setValue(16);

        updateHoliday(q, 17,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(17);

        updateHoliday(q, 18,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(18);

        updateHoliday(q, 19,  QT_TR_NOOP("День Хария Мудрого"), QT_TR_NOOP(""), 0);
        progress.setValue(19);

        updateHoliday(q, 20,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(20);

        updateHoliday(q, 21,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(21);

        updateHoliday(q, 22,  QT_TR_NOOP("День Великого Знания"), QT_TR_NOOP(""), 8);
        progress.setValue(22);

        updateHoliday(q, 23, QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"
                                                        "СВЯТЫЙ ПОСТЪ - девять дней. В этот период непозволительно употреблять - мясную, "
                                                        "рыбную, жирную белковую (яйца, масло, молоко) и острую пищу. В пищу в эти дни нужно "
                                                        "употреблять: утром и вечером - овощи, фрукты, зерновые и зернобобовые, а днем - воду, "
                                                        "соки, травяные чаи. Это связано с подготовкой к переходу на ранне-консервируемую пищу."), 16);
        progress.setValue(23);

        updateHoliday(q, 24, QT_TR_NOOP("День Трояна"), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"
                                                                   "Данный Бог - Покровитель жизненных путей."), 20);
        progress.setValue(24);

        updateHoliday(q, 25,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"), 16);
        progress.setValue(25);

        updateHoliday(q, 26,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"), 16);
        progress.setValue(26);

        updateHoliday(q, 27,  QT_TR_NOOP("День Святибора"), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"), 18);
        progress.setValue(27);

        updateHoliday(q, 28,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"), 16);
        progress.setValue(28);

        updateHoliday(q, 29,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"), 16);
        progress.setValue(29);

        updateHoliday(q, 30,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"), 16);
        progress.setValue(30);

        updateHoliday(q, 31,  QT_TR_NOOP("День Моисия Волхва"), QT_TR_NOOP("<p style='color:#fef25e' align='right'>СВЯТЫЙ ПОСТЪ</p>"), 16);
        progress.setValue(31);

        updateHoliday(q, 32,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(32);

        updateHoliday(q, 33,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(33);

        updateHoliday(q, 34,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(34);

        updateHoliday(q, 35,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(35);

        updateHoliday(q, 36,  QT_TR_NOOP("День Сканда Ирийского"), QT_TR_NOOP(""), 4);
        progress.setValue(36);

        updateHoliday(q, 37,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(37);

        updateHoliday(q, 38,  QT_TR_NOOP("День Богини Рожаны"), QT_TR_NOOP("Данная Богиня покровительствует роженицам."), 10);
        progress.setValue(38);

        updateHoliday(q, 39,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(39);

        updateHoliday(q, 40,  QT_TR_NOOP("День Мудрых Странников"), QT_TR_NOOP(""), 4);
        progress.setValue(40);

        updateHoliday(q, 41,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(41);

        updateHoliday(q, 42,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(42);

        updateHoliday(q, 43,  QT_TR_NOOP("День Мирослава Тарийского"), QT_TR_NOOP(""), 0);
        progress.setValue(43);

        updateHoliday(q, 44,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(44);

        updateHoliday(q, 45,  QT_TR_NOOP("День Хария Предвечного"), QT_TR_NOOP(""), 4);
        progress.setValue(45);

        updateHoliday(q, 46,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(46);

        updateHoliday(q, 47,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(47);

        updateHoliday(q, 48,  QT_TR_NOOP("День Родамира Тарийского"), QT_TR_NOOP(""), 1);
        progress.setValue(48);

        updateHoliday(q, 49,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(49);

        updateHoliday(q, 50,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(50);

        updateHoliday(q, 51,  QT_TR_NOOP(""), QT_TR_NOOP(""), 1);
        progress.setValue(51);

        updateHoliday(q, 52,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(52);

        updateHoliday(q, 53, QT_TR_NOOP("День Богини Макоши"), QT_TR_NOOP("Богиня наших судеб, вместе с дочерьми Доолей и Недолей прядёт нить "
                                                                          "наших судеб. Даже Боги подчиняются её нитям."), 10);
        progress.setValue(53);

        updateHoliday(q, 54,  QT_TR_NOOP("День Олега Воителя"), QT_TR_NOOP(""), 0);
        progress.setValue(54);

        updateHoliday(q, 55,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(55);

        updateHoliday(q, 56,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(56);

        updateHoliday(q, 57,  QT_TR_NOOP("День Посвящённых Инглян"), QT_TR_NOOP(""), 4);
        progress.setValue(57);

        updateHoliday(q, 58,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(58);

        updateHoliday(q, 59,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(59);

        updateHoliday(q, 60,  QT_TR_NOOP("День Аристарха Мудрого"), QT_TR_NOOP(""), 0);
        progress.setValue(60);

        updateHoliday(q, 61,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(61);

        updateHoliday(q, 62,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(62);

        updateHoliday(q, 63,  QT_TR_NOOP("День Откровения Богов"), QT_TR_NOOP(""), 8);
        progress.setValue(63);

        updateHoliday(q, 64,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(64);

        updateHoliday(q, 65,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(65);

        updateHoliday(q, 66, QT_TR_NOOP("День Сварога и Семаргла"), QT_TR_NOOP("Сварог - Верховный Небесный Бог, управляющий течением Жизни "
                                                                               "нашей и всем Мироустройством Вселенной в Явном Мире. Бог Сварог "
                                                                               "и его сын Семаргл победили Чёрного змея и провели рубеж по "
                                                                               "землям Мира Яви. Тёмная Навь была совлечена ниже Яви, а Славь "
                                                                               "осталась выше."), 10);
        progress.setValue(66);

        updateHoliday(q, 67,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(67);

        updateHoliday(q, 68,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(68);

        updateHoliday(q, 69,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(69);

        updateHoliday(q, 70,  QT_TR_NOOP("День Ярослава Мироустроителя"), QT_TR_NOOP(""), 4);
        progress.setValue(70);

        updateHoliday(q, 71,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(71);

        updateHoliday(q, 72,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(72);

        updateHoliday(q, 73,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(73);

        updateHoliday(q, 74,  QT_TR_NOOP("День Хорива Мудрого"), QT_TR_NOOP(""), 0);
        progress.setValue(74);

        updateHoliday(q, 75,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(75);

        updateHoliday(q, 76,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(76);

        updateHoliday(q, 77,  QT_TR_NOOP("День Олега Праведного"), QT_TR_NOOP(""), 4);
        progress.setValue(77);

        updateHoliday(q, 78,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(78);

        updateHoliday(q, 79,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(79);

        updateHoliday(q, 80,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(80);

        updateHoliday(q, 81,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(81);

        updateHoliday(q, 82, QT_TR_NOOP("День Даждьбога и Марены"), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"
                                                                               "Свадьба Даждьбога с Богиней Мареной.\nДаждьбог - Бог Тарх "
                                                                               "Перунович, Бог-Хранитель древней Великой Мудрости. Был назван "
                                                                               "Даждьбогом за то, что дал людям Великой Расы и потомкам Рода "
                                                                               "Небесного Девять Саньтий (книг).[n][t]Марена (Мара) - Великая "
                                                                               "Богиня Зимы, Ночи и Вечного Сна и Вечной Жизни.<br><br>"
                                                                               "ДАЖДЬБОЖИЙ ПОСТЪ - две недели. В этот период "
                                                                               "мы готовим себя к переходу от ранне-консервируемой пищи на "
                                                                               "полностью консервируемую пищу (соления, копчения и т.д.). В этот "
                                                                               "период непозволительно употреблять - мясо, жирную рыбу, яйца, "
                                                                               "белый хлеб, жирные молочные продукты и масло (растительное масло "
                                                                               "- можно употреблять). Из хлебных изделий - можно вкушать "
                                                                               "'серый' и ржаной хлеб, особенно испеченного их полуночной "
                                                                               "опары. Остальные продукты вкушаются также, как и в Святый ПОСТЪ. "
                                                                               "Для улучшения обмена веществ, днем рекомендуется употреблять "
                                                                               "кислую капусту или гречневую кашу."), 40);
        progress.setValue(82);

        updateHoliday(q, 83,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(83);

        updateHoliday(q, 84,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 33);
        progress.setValue(84);

        updateHoliday(q, 85,  QT_TR_NOOP("День Марьяны и Марены"), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(85);

        updateHoliday(q, 86,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(86);

        updateHoliday(q, 87,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 33);
        progress.setValue(87);

        updateHoliday(q, 88,  QT_TR_NOOP("День Успокоения"), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 36);
        progress.setValue(88);

        updateHoliday(q, 89,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(89);

        updateHoliday(q, 90,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(90);

        updateHoliday(q, 91,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(91);

        updateHoliday(q, 92,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(92);

        updateHoliday(q, 93,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(93);

        updateHoliday(q, 94, QT_TR_NOOP("День Перемен, Коляда"), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"
                                                                            "Рождение Бога Коляды. Также в этот день Перун победил зверя "
                                                                            "Скипетра и освободил из Пекла наших Предков, завлечённых туда "
                                                                            "обманом."), 44);
        progress.setValue(94);

        updateHoliday(q, 95,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(95);

        updateHoliday(q, 96,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(96);

        updateHoliday(q, 97,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(97);

        updateHoliday(q, 98,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 32);
        progress.setValue(98);

        updateHoliday(q, 99,  QT_TR_NOOP("День Спасителя Расы"), QT_TR_NOOP("<p style='color:#8ec96d' align='right'>ДАЖДЬБОЖИЙ ПОСТЪ</p>"), 44);
        progress.setValue(99);

        updateHoliday(q, 100,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(100);

        updateHoliday(q, 101,  QT_TR_NOOP("День Всеволода Вечного"), QT_TR_NOOP(""), 0);
        progress.setValue(101);

        updateHoliday(q, 102,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(102);

        updateHoliday(q, 103, QT_TR_NOOP("День Великого Покоя"), QT_TR_NOOP("В этот день Бог Перун завалил Врата Междумирья ведущие в Пекло "
                                                                            "Кавказскими горами."), 0);
        progress.setValue(103);

        updateHoliday(q, 104,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(104);

        updateHoliday(q, 105,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(105);

        updateHoliday(q, 106,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(106);

        updateHoliday(q, 107,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(107);

        updateHoliday(q, 108,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(108);

        updateHoliday(q, 109,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(109);

        updateHoliday(q, 110,  QT_TR_NOOP("Родогощь"), QT_TR_NOOP("День победы Бога Велеса над Кощеем."), 10);
        progress.setValue(110);

        updateHoliday(q, 111,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(111);

        updateHoliday(q, 112,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(112);

        updateHoliday(q, 113,  QT_TR_NOOP("Неделя Памяти Праотцев"), QT_TR_NOOP("Неделя чествования Праотцев, освободжённых из Пекла."), 10);
        progress.setValue(113);

        updateHoliday(q, 114, QT_TR_NOOP("День Бога Перуна"), QT_TR_NOOP("В этот день Бог Перун, после того как очистил землю от бесов и "
                                                                         "нечисти, воткнул сияющий меч в землю.[n]Перун - Бог-Покровитель "
                                                                         "всех воинов и многих Родов из Расы Великой, защитник Земель и "
                                                                         "Рода Святорусов (русичей, Белорусов, эстов, литов, латов, латгалов, "
                                                                         "полян, сербов и др.) от Тёмных сил, Бог-Громовержец, управляющий "
                                                                         "Молниями, сын Бога Сварога и Лады-Богородицы, внук Бога Вышеня."), 8);
        progress.setValue(114);

        updateHoliday(q, 115,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(115);

        updateHoliday(q, 116,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(116);

        updateHoliday(q, 117,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(117);

        updateHoliday(q, 118,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(118);

        updateHoliday(q, 119,  QT_TR_NOOP("День Александра Основателя"), QT_TR_NOOP(""), 0);
        progress.setValue(119);

        updateHoliday(q, 120,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(120);

        updateHoliday(q, 121,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(121);

        updateHoliday(q, 122,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(122);

        updateHoliday(q, 123,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(123);

        updateHoliday(q, 124,  QT_TR_NOOP("День Винитара Воителя"), QT_TR_NOOP(""), 0);
        progress.setValue(124);

        updateHoliday(q, 125,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(125);

        updateHoliday(q, 126,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(126);

        updateHoliday(q, 127,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(127);

        updateHoliday(q, 128,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(128);

        updateHoliday(q, 129,  QT_TR_NOOP("День Горислава Мученика"), QT_TR_NOOP(""), 05);
        progress.setValue(129);

        updateHoliday(q, 130,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(130);

        updateHoliday(q, 131,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(131);

        updateHoliday(q, 132,  QT_TR_NOOP(""), QT_TR_NOOP(""), 1);
        progress.setValue(132);

        updateHoliday(q, 133,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(133);

        updateHoliday(q, 134,  QT_TR_NOOP("День Светозара Вечнаго"), QT_TR_NOOP(""), 4);
        progress.setValue(134);

        updateHoliday(q, 135,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(135);

        updateHoliday(q, 136,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(136);

        updateHoliday(q, 137,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(137);

        updateHoliday(q, 138,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(138);

        updateHoliday(q, 139,  QT_TR_NOOP("День Хария Праведного"), QT_TR_NOOP(""), 4);
        progress.setValue(139);

        updateHoliday(q, 140,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(140);

        updateHoliday(q, 141,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(141);

        updateHoliday(q, 142,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(142);

        updateHoliday(q, 143,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(143);

        updateHoliday(q, 144,  QT_TR_NOOP("День Рамхата Странника"), QT_TR_NOOP(""), 2);
        progress.setValue(144);

        updateHoliday(q, 145, QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"
                                                         "ЧИСТЫЙ ПОСТЪ - две недели. В этот период непозволительно употреблять пищу - мясную, "
                                                         "жирную, кислую (квашеную), яйца. Масло животное и растительное - запрещено! "
                                                         "Можно - нежирную рыбу (кроме жареной), крупяные изделия, 'серый' и ржаной хлеб, "
                                                         "орехи, соки, травяные чаи и в малых количествах - зернобобовые. Во вторую неделю "
                                                         "ПОСТа постепенно вводится свежая растительная пища (зеленый лук, редис и т.д.)."), 64);
        progress.setValue(145);

        updateHoliday(q, 146,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(146);

        updateHoliday(q, 147,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(147);

        updateHoliday(q, 148,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(148);

        updateHoliday(q, 149,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(149);

        updateHoliday(q, 150,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(150);

        updateHoliday(q, 151,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(151);

        updateHoliday(q, 152,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(152);

        updateHoliday(q, 153,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(153);

        updateHoliday(q, 154,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(154);

        updateHoliday(q, 155, QT_TR_NOOP("День Дида"), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"
                                                                  "День поминовения всех дедов по Родовой (отцовской) Линии с посещением "
                                                                  "кладбищ, погостов, отдельных могил и курганов."), 74);
        progress.setValue(155);

        updateHoliday(q, 156,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(156);

        updateHoliday(q, 157,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(157);

        updateHoliday(q, 158,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(158);

        updateHoliday(q, 159, QT_TR_NOOP("День Бога Велеса"), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"
                                                                         "Бог-Покровитель скотоводов и животноводов, а также родовой покровитель "
                                                                         "Западных Славян - Скоттов (шотландев).[n][t]Бог Велес, охраняя "
                                                                         "Небесные Врата Междумирья, пропускает во Сваргу Пречистую Души "
                                                                         "только тех усопших, кто не щадил своей жизни во защиту Родов своих, "
                                                                         "во защиту земель Отцов и Дедов, во зашиту древней Веры; которые "
                                                                         "усердно и созидательно трудились для процветания своих Родов и "
                                                                         "которые от чистого сердца выполняли Два Великих Принципа: Свято "
                                                                         "чтить Богов и Предков своих и живших по Совести в гармонии с "
                                                                         "Матерью-Природой."), 74);
        progress.setValue(159);

        updateHoliday(q, 160,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(160);

        updateHoliday(q, 161,  QT_TR_NOOP(""), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"), 64);
        progress.setValue(161);

        updateHoliday(q, 162, QT_TR_NOOP("День Храма Души"), QT_TR_NOOP("<p style='color:#6fc390' align='right'>ЧИСТЫЙ ПОСТЪ</p>"
                                                                        "День, когда человек должен навести порядок в своей Душе. Он должен "
                                                                        "посетить службу в Храме, принести дары Богам и Предкам, принять "
                                                                        "Чистый Свет в свою Душу от Храмовой Огневицы. Этот день завершает "
                                                                        "Чистый Пост."), 74);
        progress.setValue(162);

        updateHoliday(q, 163,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(163);

        updateHoliday(q, 164,  QT_TR_NOOP("День Вечезара Мученика"), QT_TR_NOOP(""), 0);
        progress.setValue(164);

        updateHoliday(q, 165,  QT_TR_NOOP(""), QT_TR_NOOP(""), 1);
        progress.setValue(165);

        updateHoliday(q, 166,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(166);

        updateHoliday(q, 167,  QT_TR_NOOP("День Аристарха Воителя"), QT_TR_NOOP(""), 4);
        progress.setValue(167);

        updateHoliday(q, 168,  QT_TR_NOOP("День Боруслава Асгардского"), QT_TR_NOOP(""), 1);
        progress.setValue(168);

        updateHoliday(q, 169,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(169);

        updateHoliday(q, 170,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(170);

        updateHoliday(q, 171,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(171);

        updateHoliday(q, 172,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(172);

        updateHoliday(q, 173,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(173);

        updateHoliday(q, 174,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(174);

        updateHoliday(q, 175,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(175);

        updateHoliday(q, 176,  QT_TR_NOOP("День Ратибора Странника"), QT_TR_NOOP(""), 4);
        progress.setValue(176);

        updateHoliday(q, 177,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(177);

        updateHoliday(q, 178,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(178);

        updateHoliday(q, 179,  QT_TR_NOOP("День Димитрия Капена"), QT_TR_NOOP(""), 2);
        progress.setValue(179);

        updateHoliday(q, 180,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(180);

        updateHoliday(q, 181,  QT_TR_NOOP("День Игоря Воителя"), QT_TR_NOOP(""), 4);
        progress.setValue(181);

        updateHoliday(q, 182,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(182);

        updateHoliday(q, 183,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(183);

        updateHoliday(q, 184, QT_TR_NOOP("День Весты, Богини Весны"), QT_TR_NOOP("День Весеннего Равноденствия. В этот день празднуется приход "
                                                                                 "на землю Весны, прилёт птиц. Во всех родах пекут из теста "
                                                                                 "фигурки жаворонков, оладьи и печенье с Солнечной Символикой."), 8);
        progress.setValue(184);

        updateHoliday(q, 185, QT_TR_NOOP("Красногор. Масленица"), QT_TR_NOOP("Второй день Весеннего Равноденствия. В этот день празднуются "
                                                                             "проводы Богини Зимы Марены в её Ледяные Чертоги, находящиеся "
                                                                             "на Севере. В этот день зажигается огромное огневище (костёр), "
                                                                             "в которое каждый присутствующий на празднике кладёт маленькую "
                                                                             "куколку, сделанную из соломы, на которую наговариваются пожелания "
                                                                             "счастья, радости, хорошего урожая и т.д. Вместе с куклой в огонь "
                                                                             "кладутся блины и зёрна (просо, овёс для произрастания обильной "
                                                                             "пищи для домашней скотины), чтобы Боги даровали обильный урожай. "
                                                                             "Проводятся народные гуляния и прыжки черег отдельное (не "
                                                                             "жертвенное) огневище, чтобы очистить свою Душу и телеса. "
                                                                             "Водятся хороводы, игрища и т.д. В эти два дня Весеннего "
                                                                             "равноденствия запрещается употребление спиртный и хмельных "
                                                                             "напитков: 'Не пейте питья хмельного, не скажите бранного "
                                                                             "слова'. Не упортебление соблюдается всеми, кроме мужей достигших "
                                                                             "возраста три круга лет (16 х 3 = 48 лет), выполнивших долг перед "
                                                                             "Родом (9 детей) - 1 чарка или имеющих Потомстенный Круг (16 и "
                                                                             "более детей) - 2 чарки Сурицы (27 грамм). Чарки им наливает "
                                                                             "Родан (предводитель Рода)."), 8);
        progress.setValue(185);

        updateHoliday(q, 186, QT_TR_NOOP("День Хария Мученика"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"
                                                                            "ВЕЛИКИЙ ПОСТЪ - тринадесять дней. В этот период непозволительно "
                                                                            "употреблять: жирную мясную пищу (кроме мяса курицы и кролика), "
                                                                            "жирную рыбу, масло (кроме растительного), молочные продукты. "
                                                                            "Все остальное нежирное можно вкушать."), 128);
        progress.setValue(186);

        updateHoliday(q, 187,  QT_TR_NOOP("День Богуслава Мученика"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(187);

        updateHoliday(q, 188,  QT_TR_NOOP("День Трислава Воителя"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(188);

        updateHoliday(q, 189,  QT_TR_NOOP("День Леды Воительницы"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(189);

        updateHoliday(q, 190, QT_TR_NOOP("День Богини Тары"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"
                                                                         "Тара - младшая сестра Тарха Даждьбога. В этот день чествуется Богиня "
                                                                         "Тара - Покровительница Священных Лесов и Деревьев. Ей преподносятся "
                                                                         "Дары и Требы. В огненный жертвенник ложат семена и зёрна для обильного "
                                                                         "урожая, необходимого для питания людей. В этот день её честь проводится "
                                                                         "служба и Великая Братчина - совместная трапеза, чаепитие, пир, "
                                                                         "учавствующих в празднике. Яства приносят участники, которые они "
                                                                         "создали собственными руками для общего стола.  Перед началом Великой "
                                                                         "Трапезы от каждого блюда берётся понемногу, чтобы принести Жертву "
                                                                         "Богине Таре и остальным Богам и Предкам."), 136);
        progress.setValue(190);

        updateHoliday(q, 191,  QT_TR_NOOP("День Святозара Спасителя"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(191);

        updateHoliday(q, 192,  QT_TR_NOOP("День Сергия Лучезарного"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(192);

        updateHoliday(q, 193,  QT_TR_NOOP("День Ратислава Асгардского"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(193);

        updateHoliday(q, 194,  QT_TR_NOOP("День Руслана Воителя"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(194);

        updateHoliday(q, 195,  QT_TR_NOOP("День Одина Асгардского"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(195);

        updateHoliday(q, 196,  QT_TR_NOOP("День Тора Свентоградского"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"), 128);
        progress.setValue(196);

        updateHoliday(q, 197, QT_TR_NOOP("День Памяти Предков"), QT_TR_NOOP("<p style='color:#f58769' align='right'>ВЕЛИКИЙ ПОСТЪ</p>"
                                                                            "День, в который почитаются все Предки всех Родов. В этот день "
                                                                            "совершаются службы на всех кладбищах и погостах, на могилах и "
                                                                            "курганах наводится чистота и порядок. Помимо Даров и Треб умершим "
                                                                            "родичам, на их последнем пристанище зажигаются священные огни "
                                                                            "(свечи, лампады, огневицы)."), 138);
        progress.setValue(197);

        updateHoliday(q, 198, QT_TR_NOOP("ПАСХЕТ"), QT_TR_NOOP("Путь Асами Ходяше Есть Твердо Сотворение - день памяти о 15 летнем переселении "
                                                               "наших Великих Предков из Даарии в Рассению и Беловодье. Память о том, как "
                                                               "Предки наши в 16-ое лето славили всех Богов и жреца Спаса за спасение от "
                                                               "Потопа Великого."), 10);
        progress.setValue(198);

        updateHoliday(q, 199, QT_TR_NOOP("День Митры Спасителя"), QT_TR_NOOP("День почитания Бога Митры, который спас Роды Рассенов и "
                                                                             "Святорусов от засухи, даровал им воду и пищу и указал каким "
                                                                             "Родам в какие земли цветущие переселиться. За то, что Митра "
                                                                             "спас Роды, тёмные силы его приковали цепями к горам Кавказским "
                                                                             "и посылали диких прит клевать его плоть. Митра три дня находлся "
                                                                             "между Жизнью и Смертью. Из Родов Святорусов была избрана дружина "
                                                                             "из тридевяти наилучших воинов, которую возглавила Жрица - "
                                                                             "Воительница Ас'Тара. Она привела воинов к месту мучения Митры, "
                                                                             "где были разбиты тёмные силы, освободила Митру от оков, оживила "
                                                                             "его Силой своей Любви, после чего на Огненной Божественной "
                                                                             "Колеснице поднялась с Митрой в Небеса. С той поры многие Роды "
                                                                             "Южной Скифии почитали Ас'Тару как Богиню Высшей Спасительной Любви "
                                                                             "и Возрождения. Отсюда появились легенды о том, что распятые Боги "
                                                                             "в третий день воскресают, т.к. на Кавказских горах был распят и "
                                                                             "Тарх Даждьбог, спасённый Лебедью Дживой."), 10);
        progress.setValue(199);

        updateHoliday(q, 200, QT_TR_NOOP("День Возрождения Тарха"), QT_TR_NOOP("Тарх Даждьбог сражался с тёмными силами и они запросили пощады, "
                                                                               "перемирия и устроили пир. На пире в питьё Тарху подсыпали "
                                                                               "Сонного Зелья, а когда он уснул, приковали его цепями к горам "
                                                                               "Кавказским, чтобы хищные прицы и звери терзали его плоть. Богиня "
                                                                               "Джива освободила его от оков, перенесла на своих Лебединых "
                                                                               "Крыльях в урочище Тары на слияние Раты и Ирия, где вместе с "
                                                                               "сестрой Тарха - Богиней Тарой они залечили его телесные раны, "
                                                                               "а Богиня Джива силой Высшей Спасительной Любви и Возрождения "
                                                                               "вдохнула в него новые силы и Жизнь, после чего состоялась "
                                                                               "свадьба Тарха Даждьбога и Богини Дживы. Даждьбог, так же, как "
                                                                               "и Митра был три дня прикован к Кавказским горам и находился "
                                                                               "между Жизнью и Смертью."), 10);
        progress.setValue(200);

        updateHoliday(q, 201,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(201);

        updateHoliday(q, 202,  QT_TR_NOOP("День Айвана Вечного"), QT_TR_NOOP(""), 0);
        progress.setValue(202);

        updateHoliday(q, 203,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(203);

        updateHoliday(q, 204, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(204);

        updateHoliday(q, 205, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(205);

        updateHoliday(q, 206, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(206);

        updateHoliday(q, 207,  QT_TR_NOOP("День Вечеслава Ирийского"), QT_TR_NOOP(""), 4);
        progress.setValue(207);

        updateHoliday(q, 208, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(208);

        updateHoliday(q, 209, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(209);

        updateHoliday(q, 210, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 1);
        progress.setValue(210);

        updateHoliday(q, 211, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(211);

        updateHoliday(q, 212, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(212);

        updateHoliday(q, 213, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 1);
        progress.setValue(213);

        updateHoliday(q, 214,  QT_TR_NOOP("День Аристарха Праведника"), QT_TR_NOOP(""), 4);
        progress.setValue(214);

        updateHoliday(q, 215, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(215);

        updateHoliday(q, 216, QT_TR_NOOP("День Лели Небесной"), QT_TR_NOOP("В этот день празднуется День Победы Даждьбога над тёмными силами, "
                                                                           "которых Кощеи собрали на Леле (ближайшей к Мидгарду Луне, а дило в "
                                                                           "то время три Луны). Даждьбог разрушил малую Луну Лелю и уничтожил "
                                                                           "тёмные силы, которые готовились к захвату Мидгарда (нашей Земли). "
                                                                           "С той поры существует обычай на день Лели Небесной красить птичьи "
                                                                           "яйца и стукать их друг об друга. Разбившееся яйцо называют Яйцом "
                                                                           "Кощея, а целое - Силой Даждьбога. Разбитое яйцо всегда отдают недругам "
                                                                           "или животным, а целое съедают сами. Также, в этот день прославляется "
                                                                           "Богиня Леля, дочь Лады Богородицы."), 10);
        progress.setValue(216);

        updateHoliday(q, 217, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все"
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и"
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и"
                                                                       "т.д.)."), 0);
        progress.setValue(217);

        updateHoliday(q, 218, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(218);

        updateHoliday(q, 219,  QT_TR_NOOP("День Борислава Спасителя"), QT_TR_NOOP(""), 0);
        progress.setValue(219);

        updateHoliday(q, 220, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(220);

        updateHoliday(q, 221, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(221);

        updateHoliday(q, 222,  QT_TR_NOOP("День Родомира Ведуна"), QT_TR_NOOP(""), 2);
        progress.setValue(222);

        updateHoliday(q, 223, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(223);

        updateHoliday(q, 224, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(224);

        updateHoliday(q, 225,  QT_TR_NOOP("День Мудрости Крышеня"), QT_TR_NOOP(""), 8);
        progress.setValue(225);

        updateHoliday(q, 226, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(226);

        updateHoliday(q, 227, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(227);

        updateHoliday(q, 228, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(228);

        updateHoliday(q, 229,  QT_TR_NOOP("Святочь"), QT_TR_NOOP(""), 12);
        progress.setValue(229);

        updateHoliday(q, 230, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(230);

        updateHoliday(q, 231, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(231);

        updateHoliday(q, 232,  QT_TR_NOOP("День Ведамира Асгардского"), QT_TR_NOOP(""), 2);
        progress.setValue(232);

        updateHoliday(q, 233, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(233);

        updateHoliday(q, 234, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(234);

        updateHoliday(q, 235, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(235);

        updateHoliday(q, 236, QT_TR_NOOP("ВЕДА-ИНТА"), QT_TR_NOOP("ИНТА (Истина Наши Твердо Асы) - день когда почитается Мудрость, которую оставили "
                                                                  "нам все Светлые Боги и Многомудрые Предки."), 12);
        progress.setValue(236);

        updateHoliday(q, 237, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(237);

        updateHoliday(q, 238, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(238);

        updateHoliday(q, 239, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(239);

        updateHoliday(q, 240, QT_TR_NOOP("День Матери Сырой Земли"), QT_TR_NOOP("День чествования Богини Матери Сырой Земли. В этот день Ей приносят "
                                                                                "в жертву лучшие семена, фрукты, овощи и просят, чтобы всё, что идёт "
                                                                                "в огонь, было в изобилии в Родах. В этот день многие овощи и фрукты "
                                                                                "высаживаются в открытый грунт, а также высеваются различные "
                                                                                "злаковые культуры."), 10);
        progress.setValue(240);

        updateHoliday(q, 241, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(241);

        updateHoliday(q, 242, QT_TR_NOOP("День Наречения"), QT_TR_NOOP("В месяц Элетъ, когда Природа расцветает, и на земле просыпаются все "
                                                                       "Природные Силы, происходят обряды наречения (новым именем, женихом и "
                                                                       "невестой, Жрецами, Волхвами, Князьями Ханами, Витязями, Дружинниками и "
                                                                       "т.д.)."), 0);
        progress.setValue(242);

        updateHoliday(q, 243, QT_TR_NOOP("День Лады Богородицы"), QT_TR_NOOP("Лада-Матушка (Матерь Сва) - Великая Небесная Мать, Богородица. "
                                                                             "Любящая и нежная Мать большинства светлых Богов Расы Великой, "
                                                                             "Богородица-Покровительница всех Народов Великой Рассении "
                                                                             "(територии Великой Расы) и Чертога Лося во Сварожьем Круге. "
                                                                             "Покровительница семьи, гармонии, красоты и любви."), 10);
        progress.setValue(243);

        updateHoliday(q, 244,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(244);

        updateHoliday(q, 245,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(245);

        updateHoliday(q, 246,  QT_TR_NOOP(""), QT_TR_NOOP(""), 1);
        progress.setValue(246);

        updateHoliday(q, 247,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(247);

        updateHoliday(q, 248,  QT_TR_NOOP("День Всеволода Мудрого"), QT_TR_NOOP(""), 4);
        progress.setValue(248);

        updateHoliday(q, 249,  QT_TR_NOOP(""), QT_TR_NOOP(""), 1);
        progress.setValue(249);

        updateHoliday(q, 250,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(250);

        updateHoliday(q, 251,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(251);

        updateHoliday(q, 252, QT_TR_NOOP("День Бога Вышеня"), QT_TR_NOOP("Вышень - Бог-Покровитель нашей Вселенной в Светлых Мирах Нави, "
                                                                         "т.е. в Мирах Слави. Заботливый и могучий Отец Бога Сварога. "
                                                                         "Справедливый судья, разрешающий любые споры, которые возникают между "
                                                                         "Богами различных Миров или между людьми."), 10);
        progress.setValue(252);

        updateHoliday(q, 253,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(253);

        updateHoliday(q, 254,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(254);

        updateHoliday(q, 255,  QT_TR_NOOP("День Бога Яровита"), QT_TR_NOOP(""), 8);
        progress.setValue(255);

        updateHoliday(q, 256,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(256);

        updateHoliday(q, 257,  QT_TR_NOOP("День Ратибора Воителя"), QT_TR_NOOP(""), 4);
        progress.setValue(257);

        updateHoliday(q, 258,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(258);

        updateHoliday(q, 259,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(259);

        updateHoliday(q, 260,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(260);

        updateHoliday(q, 261,  QT_TR_NOOP("День Мудрости Будха"), QT_TR_NOOP(""), 4);
        progress.setValue(261);

        updateHoliday(q, 262,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(262);

        updateHoliday(q, 263,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(263);

        updateHoliday(q, 264,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(264);

        updateHoliday(q, 265, QT_TR_NOOP("День Ра-М-Ха - ИНТА"), QT_TR_NOOP("Ра-М-Ха ИНТА (Истина Наши Твердо Асы) - Мудрость Ра-М-Хи.<br>"
                                                                            "Ра-М-Ха (РАМХА) - Единый Творец-Созидатель, Вышняя Непознаваемая "
                                                                            "Сущность, излучающая Изначальный Жизньродящий Свет Радости и "
                                                                            "Первичный Огонь Мироздания (Жизньнесущую Инглию), из которого "
                                                                            "появились все ныне существующие, а также существовавшие в прошлом "
                                                                            "видимые и невидимые Вселенные и всевозможные обитаемые Миры."), 10);
        progress.setValue(265);

        updateHoliday(q, 266,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(266);

        updateHoliday(q, 267,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(267);

        updateHoliday(q, 268,  QT_TR_NOOP("День Ведаслава Праведника"), QT_TR_NOOP(""), 4);
        progress.setValue(268);

        updateHoliday(q, 269,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(269);

        updateHoliday(q, 270,  QT_TR_NOOP("Триглав"), QT_TR_NOOP("День когда почитаются Великие Триглавы."), 10);
        progress.setValue(270);

        updateHoliday(q, 271,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(271);

        updateHoliday(q, 272,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(272);

        updateHoliday(q, 273,  QT_TR_NOOP("Родоница"), QT_TR_NOOP(""), 10);
        progress.setValue(273);

        updateHoliday(q, 274, QT_TR_NOOP("День Числобога"), QT_TR_NOOP("Числобог - Мудрый, Вышний Бог, управляющий течением Реки Времени, "
                                                                       "а также Бог-Хранитель Даарийского Круголёта и различных жреческих "
                                                                       "систем Славяно-Арийского летоисчисления."), 8);
        progress.setValue(274);

        updateHoliday(q, 275, QT_TR_NOOP("День Ладаада"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"
                                                                     "СВЕТЛЫЙ ПОСТЪ - две недели. В этот период позволительно употреблять "
                                                                     "только утром и вечером - одну лишь растительную пищу, а в день Бога Купала, "
                                                                     "который совпадает с ПОСТом - можно есть мясо, но оно должно быть "
                                                                     "приготовлено на Живом Огне (костер, запаленный от добытого Живого Огня)."), 264);
        progress.setValue(275);

        updateHoliday(q, 276, QT_TR_NOOP("День Бога Хорса"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"
                                                                        "Праздник начинается с правьславления Бога Хорса и принесения жертв "
                                                                        "безкровных (круглые кулички) на жертвенник. После чего устраиваются "
                                                                        "хороводы, хороводные пляски - медленные движения с подражаньем словам "
                                                                        "хороводной, протяжной песни. В летний праздник Бога Хорса с горы к "
                                                                        "реке обязательно скатывают большое горящее колесо, символизирующее "
                                                                        "солярный знак солнца."), 264);
        progress.setValue(276);

        updateHoliday(q, 277,  QT_TR_NOOP("День Ясуни"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 256);
        progress.setValue(277);

        updateHoliday(q, 278,  QT_TR_NOOP("День Родовита"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 256);
        progress.setValue(278);

        updateHoliday(q, 279,  QT_TR_NOOP("День Подзвезда"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 256);
        progress.setValue(279);

        updateHoliday(q, 280,  QT_TR_NOOP("День Родарита"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 256);
        progress.setValue(280);

        updateHoliday(q, 281,  QT_TR_NOOP("День Родагоста"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 256);
        progress.setValue(281);

        updateHoliday(q, 282,  QT_TR_NOOP("День Крода"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 258);
        progress.setValue(282);

        updateHoliday(q, 283,  QT_TR_NOOP("Русалии"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 264);
        progress.setValue(283);

        updateHoliday(q, 284,  QT_TR_NOOP("Русалии"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 264);
        progress.setValue(284);

        updateHoliday(q, 285,  QT_TR_NOOP("Русалии"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 264);
        progress.setValue(285);

        updateHoliday(q, 286, QT_TR_NOOP("День Ярилы-Солнца"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"
                                                                          "Почитается наше Солнце, которое дарует нам тепло и свет."), 264);
        progress.setValue(286);

        updateHoliday(q, 287,  QT_TR_NOOP("День Омовения"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 256);
        progress.setValue(287);

        updateHoliday(q, 288,  QT_TR_NOOP("Русалии"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 264);
        progress.setValue(288);

        updateHoliday(q, 289,  QT_TR_NOOP("Русалии"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 264);
        progress.setValue(289);

        updateHoliday(q, 290,  QT_TR_NOOP("Русалии"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 264);
        progress.setValue(290);

        updateHoliday(q, 291, QT_TR_NOOP("День Бога Купала"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"
                                                                         "Почитается Бог Купала. Люди проходят очищение в пяти стихиях."), 265);
        progress.setValue(291);

        updateHoliday(q, 292,  QT_TR_NOOP("День Зари-Мерцаны"), QT_TR_NOOP("<p style='color:#7ccef2' align='right'>СВЕТЛЫЙ ПОСТЪ</p>"), 268);
        progress.setValue(292);

        updateHoliday(q, 293,  QT_TR_NOOP("День Великой Инглии"), QT_TR_NOOP(""), 10);
        progress.setValue(293);

        updateHoliday(q, 294,  QT_TR_NOOP(""), QT_TR_NOOP(""), 1);
        progress.setValue(294);

        updateHoliday(q, 295,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(295);

        updateHoliday(q, 296,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(296);

        updateHoliday(q, 297,  QT_TR_NOOP("День Александра Странника"), QT_TR_NOOP(""), 4);
        progress.setValue(297);

        updateHoliday(q, 298,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(298);

        updateHoliday(q, 299,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(299);

        updateHoliday(q, 300,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(300);

        updateHoliday(q, 301,  QT_TR_NOOP("День Земли Свята Расы"), QT_TR_NOOP(""), 10);
        progress.setValue(301);

        updateHoliday(q, 302,  QT_TR_NOOP("День Орея Вендагардского"), QT_TR_NOOP(""), 0);
        progress.setValue(302);

        updateHoliday(q, 303,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(303);

        updateHoliday(q, 304,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(304);

        updateHoliday(q, 305,  QT_TR_NOOP("Перуница"), QT_TR_NOOP("День почитания Жены Бога Перуна - Дивы."), 8);
        progress.setValue(305);

        updateHoliday(q, 306,  QT_TR_NOOP("День Сбора Плодов"), QT_TR_NOOP(""), 8);
        progress.setValue(306);

        updateHoliday(q, 307,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(307);

        updateHoliday(q, 308,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(308);

        updateHoliday(q, 309,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(309);

        updateHoliday(q, 310,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(310);

        updateHoliday(q, 311,  QT_TR_NOOP("День Бога Удрзеца"), QT_TR_NOOP(""), 10);
        progress.setValue(311);

        updateHoliday(q, 312,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(312);

        updateHoliday(q, 313,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(313);

        updateHoliday(q, 314,  QT_TR_NOOP("День Игоря Святомудрого"), QT_TR_NOOP(""), 4);
        progress.setValue(314);

        updateHoliday(q, 315,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(315);

        updateHoliday(q, 316,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(316);

        updateHoliday(q, 317,  QT_TR_NOOP("Вышний День Перуна"), QT_TR_NOOP(""), 8);
        progress.setValue(317);

        updateHoliday(q, 318,  QT_TR_NOOP("День Летиницы"), QT_TR_NOOP(""), 10);
        progress.setValue(318);

        updateHoliday(q, 319,  QT_TR_NOOP("РИТА"), QT_TR_NOOP(""), 10);
        progress.setValue(319);

        updateHoliday(q, 320,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(320);

        updateHoliday(q, 321,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(321);

        updateHoliday(q, 322,  QT_TR_NOOP("День Александра Ярославича"), QT_TR_NOOP(""), 4);
        progress.setValue(322);

        updateHoliday(q, 323,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(323);

        updateHoliday(q, 324,  QT_TR_NOOP("Житничь"), QT_TR_NOOP("Начало Жатвы."), 8);
        progress.setValue(324);

        updateHoliday(q, 325,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(325);

        updateHoliday(q, 326,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(326);

        updateHoliday(q, 327,  QT_TR_NOOP("День Ворона Варуны"), QT_TR_NOOP(""), 11);
        progress.setValue(327);

        updateHoliday(q, 328,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(328);

        updateHoliday(q, 329,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(329);

        updateHoliday(q, 330,  QT_TR_NOOP("День Александра Мудрого"), QT_TR_NOOP(""), 5);
        progress.setValue(330);

        updateHoliday(q, 331,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(331);

        updateHoliday(q, 332,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(332);

        updateHoliday(q, 333, QT_TR_NOOP("День Трёх Лун"), QT_TR_NOOP("В этот день, когда в древние времена вокруг Земли вращались три луны, "
                                                                      "они сошлись на небосводе в одном месте. В этот день было начато "
                                                                      "строительство Асгарда Ирийского и Великого Капища Инглии (Великого "
                                                                      "Храма Священного Первичного Огня). Этот день считается днём основания "
                                                                      "Священного Города Богов, построенного на слиянии рек Ирий и Омь."), 8);
        progress.setValue(333);

        updateHoliday(q, 334,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(334);

        updateHoliday(q, 335,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(335);

        updateHoliday(q, 336,  QT_TR_NOOP("День Скифия Волхва"), QT_TR_NOOP(""), 4);
        progress.setValue(336);

        updateHoliday(q, 337,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(337);

        updateHoliday(q, 338,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(338);

        updateHoliday(q, 339,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(339);

        updateHoliday(q, 340,  QT_TR_NOOP("День Димитрия Основателя"), QT_TR_NOOP(""), 0);
        progress.setValue(340);

        updateHoliday(q, 341,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(341);

        updateHoliday(q, 342,  QT_TR_NOOP("День Стрибога"), QT_TR_NOOP("Почитание Бога управляющего ветрами."), 10);
        progress.setValue(342);

        updateHoliday(q, 343,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(343);

        updateHoliday(q, 344,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(344);

        updateHoliday(q, 345,  QT_TR_NOOP("Златогорка"), QT_TR_NOOP(""), 8);
        progress.setValue(345);

        updateHoliday(q, 346,  QT_TR_NOOP("Любомир"), QT_TR_NOOP("День Свадеб."), 8);
        progress.setValue(346);

        updateHoliday(q, 347,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(347);

        updateHoliday(q, 348,  QT_TR_NOOP("День Свентослава Асгардского"), QT_TR_NOOP(""), 4);
        progress.setValue(348);

        updateHoliday(q, 349,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(349);

        updateHoliday(q, 350,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(350);

        updateHoliday(q, 351,  QT_TR_NOOP("День Джива-ИНТА"), QT_TR_NOOP("Время очищения и преображения."), 10);
        progress.setValue(351);

        updateHoliday(q, 352,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(352);

        updateHoliday(q, 353,  QT_TR_NOOP("День Жели"), QT_TR_NOOP(""), 10);
        progress.setValue(353);

        updateHoliday(q, 354,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(354);

        updateHoliday(q, 355,  QT_TR_NOOP("День Святослава Праведного"), QT_TR_NOOP(""), 4);
        progress.setValue(355);

        updateHoliday(q, 356,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(356);

        updateHoliday(q, 357,  QT_TR_NOOP("Сеченник"), QT_TR_NOOP(""), 10);
        progress.setValue(357);

        updateHoliday(q, 358,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(358);

        updateHoliday(q, 359, QT_TR_NOOP("День Богини Карны"), QT_TR_NOOP("День почитанияя богини Карны (сестры Жели). Она отвечает за "
                                                                          "перерождение душ в Явном мире, которые не исполнили свой долг."), 10);
        progress.setValue(359);

        updateHoliday(q, 360,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(360);

        updateHoliday(q, 361,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(361);

        updateHoliday(q, 362,  QT_TR_NOOP("День Светозара Странника"), QT_TR_NOOP(""), 0);
        progress.setValue(362);

        updateHoliday(q, 363,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(363);

        updateHoliday(q, 364,  QT_TR_NOOP(""), QT_TR_NOOP(""), 0);
        progress.setValue(364);

        updateHoliday(q, 365,  QT_TR_NOOP("День Александра Вещего"), QT_TR_NOOP(""), 4);
        progress.setValue(365);


        {
            // TODO: в далёком будущем добавить государственные праздники
#if 0
            if (!q.prepare(QString("insert into gov_holid (id, name, descr) values(?, ?, ?)")))
                return q.lastError();
            addHoliday(q, 1, QString("Новый год"), QString(""), -1);
            addHoliday(q, 2, QString("Рождество Христово"), QString(""), -1);
            addHoliday(q, 3, QString("День работника прокуратуры РФ"), QString(""), -1);
            addHoliday(q, 4, QString("День российской печати"), QString(""), -1);
            addHoliday(q, 5, QString("Старый Новый год"), QString(""), -1);
            addHoliday(q, 6, QString("День инженерных войск РФ"), QString(""), -1);
            addHoliday(q, 7, QString("День снятия блокады города Ленинграда (1944)"), QString(""), -1);
            addHoliday(q, 8, QString("День памяти А. С. Пушкина (1799-1837)"), QString(""), -1);
            addHoliday(q, 9, QString("День памяти воинов-интернационалистов в России"), QString(""), -1);
            addHoliday(q, 10, QString("День защитника Отечества"), QString(""), -1);
            addHoliday(q, 11, QString("Международный женский день"), QString(""), -1);
            addHoliday(q, 12, QString("День работников геодезии и картографии"), QString(""), -1);
            addHoliday(q, 13, QString("День моряка-подводника; День работников торговли, бытового обслуживания населения и жилищно-коммунального хозяйства"), QString(""), -1);
            addHoliday(q, 14, QString("Международный день театра; День внутренних войск МВД РФ"), QString(""), -1);
            addHoliday(q, 15, QString("День смеха"), QString(""), -1);
            addHoliday(q, 16, QString("День единения народов России и Белоруссии; День геолога"), QString(""), -1);
            addHoliday(q, 17, QString("День памяти погибших подводников"), QString(""), -1);
            addHoliday(q, 18, QString("День войск противовоздушной обороны РФ"), QString(""), -1);
            addHoliday(q, 19, QString("Международный день освобождения узников фашистских концлагерей"), QString(""), -1);
            addHoliday(q, 20, QString("День авиации и космонавтики"), QString(""), -1);
            addHoliday(q, 21, QString("День науки"), QString(""), -1);
            addHoliday(q, 22, QString("Международный день солидарности молодежи"), QString(""), -1);
            addHoliday(q, 23, QString("День памяти погибших в радиационных авариях и катастрофах"), QString(""), -1);
            addHoliday(q, 24, QString("Международный день танца"), QString(""), -1);
            addHoliday(q, 25, QString("День пожарной охраны РФ"), QString(""), -1);
            addHoliday(q, 26, QString("Праздник Весны и Труда"), QString(""), -1);
            addHoliday(q, 27, QString("Всемирный день свободы печати"), QString(""), -1);
            addHoliday(q, 28, QString("День Европы"), QString(""), -1);
            addHoliday(q, 29, QString("День радио. Праздник работников всех отраслей связи"), QString(""), -1);
            addHoliday(q, 30, QString("День Победы советского народа в Великой Отечественной войне 1941-1945 гг."), QString(""), -1);
            addHoliday(q, 31, QString("Международный день электросвязи"), QString(""), -1);
            addHoliday(q, 32, QString("День славянской письменности и культуры"), QString(""), -1);
            addHoliday(q, 33, QString("Общероссийский день библиотек"), QString(""), -1);
            addHoliday(q, 34, QString("День химика; День пограничника"), QString(""), -1);
            addHoliday(q, 35, QString("Международный день защиты детей"), QString(""), -1);
            addHoliday(q, 36, QString("Пушкинский день России"), QString(""), -1);
            addHoliday(q, 37, QString("День работника текстильной и легкой промышленности"), QString(""), -1);
            addHoliday(q, 38, QString("День России"), QString(""), -1);
            addHoliday(q, 39, QString("День медицинского работника"), QString(""), -1);
            addHoliday(q, 40, QString("День памяти и скорби"), QString(""), -1);
            addHoliday(q, 41, QString("День изобретателя и рационализатора"), QString(""), -1);
            addHoliday(q, 42, QString("Международный день борьбы с наркоманией и незаконным оборотом наркотиков"), QString(""), -1);
            addHoliday(q, 43, QString("День молодежи в России"), QString(""), -1);
            addHoliday(q, 44, QString("День работника морского и речного флота"), QString(""), -1);
            addHoliday(q, 45, QString("День рыбака; День российской почты"), QString(""), -1);
            addHoliday(q, 46, QString("День металлурга"), QString(""), -1);
            addHoliday(q, 47, QString("День Военно-морского флота РФ"), QString(""), -1);
            addHoliday(q, 48, QString("День железнодорожника; День железнодорожных войск РФ"), QString(""), -1);
            addHoliday(q, 49, QString("День военно-воздушных сил РФ; Международный день молодежи"), QString(""), -1);
            addHoliday(q, 50, QString("День строителя"), QString(""), -1);
            addHoliday(q, 51, QString("День воздушного флота РФ"), QString(""), -1);
            addHoliday(q, 52, QString("День государственного флага РФ"), QString(""), -1);
            addHoliday(q, 53, QString("День шахтера; День российского кино"), QString(""), -1);
            addHoliday(q, 54, QString("День знаний"), QString(""), -1);
            addHoliday(q, 55, QString("День российской гвардии"), QString(""), -1);
            addHoliday(q, 56, QString("День работников нефтяной и газовой промышленности"), QString(""), -1);
            addHoliday(q, 57, QString("Международный день солидарности журналистов"), QString(""), -1);
            addHoliday(q, 58, QString("День танкистов"), QString(""), -1);
            addHoliday(q, 59, QString("День работников лесного хозяйства"), QString(""), -1);
            addHoliday(q, 60, QString("День машиностроителя"), QString(""), -1);
            addHoliday(q, 61, QString("Международный день учителя; Международный день пожилых людей"), QString(""), -1);
            addHoliday(q, 62, QString("Всемирный день архитектуры; Международный день жилья"), QString(""), -1);
            addHoliday(q, 63, QString("День военно-космических сил РФ"), QString(""), -1);
            addHoliday(q, 64, QString("День таможенника РФ"), QString(""), -1);
            addHoliday(q, 65, QString("День работников автомобильного транспорта и дорожного хозяйства"), QString(""), -1);
            addHoliday(q, 66, QString("День памяти жертв политических репрессий в России; День политзаключенного."), QString(""), -1);
            addHoliday(q, 67, QString("День народного единства"), QString(""), -1);
            addHoliday(q, 68, QString("День милиции"), QString(""), -1);
            addHoliday(q, 69, QString("Международный день слепых"), QString(""), -1);
            addHoliday(q, 70, QString("Всероссийский день призывника"), QString(""), -1);
            addHoliday(q, 71, QString("Международный день студентов"), QString(""), -1);
            addHoliday(q, 72, QString("День ракетных войск и артиллерии РФ"), QString(""), -1);
            addHoliday(q, 73, QString("День работников налоговых органов РФ"), QString(""), -1);
            addHoliday(q, 74, QString("Международный день инвалидов"), QString(""), -1);
            addHoliday(q, 75, QString("День ракетных войск стратегического назначения РФ"), QString(""), -1);
            addHoliday(q, 76, QString("День работника органов безопасности (ФСБ) РФ"), QString(""), -1);
            addHoliday(q, 77, QString("День энергетика"), QString(""), -1);
            addHoliday(q, 78, QString("День спасателя РФ"), QString(""), -1);
#endif
        }

        return QSqlError();
    }
};
