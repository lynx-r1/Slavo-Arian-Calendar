# Информация об этом календаре является подозрительной. Я не несу никакой ответсвенности за ее распространение и применение. Мне было интересно решить задачу.

Цель проекта "Славяно-Арийский Календарь":
  Разработка приложений, которые упростили бы применение в жизни знаний о жизнестрое
  Славян и Ариев.

Базовые возможности программы:
  * наглядное представление Славяно-Арийского календаря и часов;
  * описание Славяно-Арийских праздников и событий.

Дополнительные возможности (в стадии планирования):
  * организация времени в соответствии с 16-ти часовым днём, 9-ти дневной неделей
    и 40(41)-дневным месяцем;
  * напоминание о праздниках и других важных событиях;
  * дневник.

О календаре из Славяно-Арийских Вед:
  Месяцев в Лете (в году) у славян было 9, а не 12, как у христиан. Славянские
  месяца содержали по 40 или 41 дню, а не по 30, 31, или 28 (29) дней, как у других
  ближневосточных и восточных народов. Славянская Неделя состояла из 9 дней, а не из 7.

Славянская мера времени:
  * 1 Час = 144 Части (= 90 минут);
  * 1 Часть = 1296 Долей (= 37.56 секунд);
  * 1 Доля = 72 Мгновений (1 сек. = 34.5 долей);
  * 1 Мгновение = 760 Мигов (1 сек. = 2484.34 мгновений);
  * 1 Миг = 160 Сигов (1 сек. = 1888102.236 мигов);
  * 1 Сиг = 14000 Сантигов (1 сек. = 302096358 сигов).

--------------------------------------------------------------------------------

Главной частью проекта является программный комплекс (ПК) "Славяно-Арийский Календарь".

ПК "Славяно-Арийский Календарь" состоит из следующих программ:
  1. Даарийский Круголет Числобога (dk4) - программа наглядно представляет Даарийский
     Круголет Числобога и содержит описания лет Круга Жизни;
  2. Славяно-Арийские Часы (sachasy) - программа наглядно показывает Славяно-Арийское
     время и дату;
  3. Редактор баз данных праздников и описаний лет (sadb) - программа предназначена
     для создания и редактирования базы данных Славяно-Арийских праздников и базы
     данных описания лет Круга Жизни;
  4. Сороковник (sorokovnik) - программа наглядно представляет Славяно-Арийский
     сороковник (месяц), а так же содержит информацию о Славяно-Арийских праздниках и
     предоставляет возможность делать заметки по дням;
  5. Славяно-Арийский Календарь (sakalendar) - главная программа в ПК. Она содержит
     в себе программы: Даарийский Круголет Числобога, Славяно-Арийские Часы и
     Сороковник в виде модулей. Предназначена для целостного представления о том
     как выглядит Славяно-Арийский Календарь.

Перечисленные программы основаны на следующих библиотеках, которые являются
частью ПК "Славяно-Арийский Календарь":
  1. dk4lib - библиотека реализует Даарийский Круголет Числобога;
  2. sachasylib - библиотека реализует Славяно-Арийские Часы;
  3. satimelib_c - библиотека на языке C, которая реализует перевод григорианского
     времени в Славяно-Арийское;
  4. satimelib_cpp - библиотека на языке C++, которая реализует перевод григорианского
     времени в Славяно-Арийское. Является объектно-ориентированной обёрткой над
     библиотекой satimelib_c;
  5. sawidgetslib - библиотека содержит используемые в программах расширенные виджеты;
  6. sorokovniklib - библиотека реализует Сороковник.

                                                                Слава Числобогу!
                                                                   Рысь (lynx_r)