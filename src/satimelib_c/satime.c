#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "satime_c.h"

/**
славяно-арийское время
1 сутки = 16 часов
1 час = 144 части
1 часть = 1296 долей

перевод с вавилонского
1 час = 5400 секунд = 90 минут = 1.5 григ. часа
1 часть = 37,5 секунд = 37500 миллисекунд
1 доля = 28.9351851851 миллисекунд

00:00 часов, 1 января, 1970 года - начало эпохи Unix
Понедельник, 21 число месяца Бейлетъ 7478 Лета от С.М.З.Х.
Лето в круге Лет: 6 Лето в круге Жизни: 102

За основу этого модуля взят алгоритм из pivotail_gmtime_r.c написанный Paul Sheer
**/

#ifdef STATIC

// 7376 Лето - начала лета в прошлом цикле
// до 2012 года
#define PREVCIRCLE          7376

// после 2012 года
//#define PREVCIRCLE          7520

// Определяем Священный Лета обычным способом
#define SVYASHCH_(n)        ( ((n) + PREVCIRCLE) % 16 == 0 )

// поправка по Дегтярёву Сергею (каждое 512 лето простое)
#define SVYASHCH__(n)       ( (((n) + PREVCIRCLE) % 16 == 0) && (((n) + PREVCIRCLE + SMZH) % 512 != 0) )

// используемое в программе определение Священного Лета
#define SVYASHCH(n)         SVYASHCH_(n)

#endif

// точка отсчёта: Понедельник, 21 Бейлета, 7478 от СМЗХ
// разница во времени: 4(по Гринвичу) * 144 * 1296 + (41 + 40 + 21) * 16 * 144 * 1296
// DST = 1 / 1.5 * 144 * 1296
#define SA_GR_DIFF          305316864.0
#define SA_DST              124415.9999875584
#define SEC_V_CHASTE        37.5
#define SEC_V_DOLE          (37.5 / 1296.0)
#define DOL_V_SEC           34.56

enum {
    DOL_V_DNE           = 2985984,
    DOL_V_CHASE         = 186624,
    DOL_V_CHASTE        = 1296,
    DNEI_V_SVYA         = 369,
    DNEI_V_PROST        = 365,

    DOLI                = 1296,
    CHASTI              = 144,
    CHASY               = 16,

    NEDLEYA             = 9,
    SOROKOVNIKI         = 9,
    STIKHII             = 9,

    SOROKOVNIK_POLN     = 41,
    SOROKOVNIK_NEPOLN   = 40,

    KZHIZNI             = 144,
    KLET                = 16,
    SMZH                = 112,
    LETO_UNIX           = 7478
};

int sa_ignore_dst = 0;

static const char *sorokovniki[] = {
    "Рамхатъ", "Айлетъ", "Бэйлетъ", "Гэйлетъ", "Дайлетъ", "Эйлетъ", "Вэйлетъ", "Хэйлетъ", "Тайлетъ"
        };

static const char *dninedeli[] = {
    "Понедельникъ", "Вторникъ", "Тритейникъ", "Четвергъ", "Пятница", "Шестица", "Седьмица", "Осьмица", "Неделя"
        };

sa_timef_t _gmtoff();
struct sa_tm *_timef_to_tm(const sa_timef_t *time, int gmtoffchas);
int _is_valid(const struct sa_tm *timeptr);
void _reset_tm(struct sa_tm *t);
char *_prepend_zerro(int chislo, int zerro);

/* День недели Сороковник День сороковника Час:Часть:Доля Круг Лет Круг Жизни Лето от СМЗХ */
char *sa_asctime(const struct sa_tm *timeptr)
{
    char bf[256] = { '\0' };
    sa_strftime(bf, 255, "%a %b %d %X %k %K %L", timeptr);

    char *buf = (char *)malloc(strlen(bf) * sizeof(char));
    strcpy(buf, bf);
    return buf;
}

char *sa_ctime(const sa_timef_t *timer)
{
    return sa_asctime(sa_localtime(timer));
}

double sa_difftime(sa_timef_t time1, sa_timef_t time0)
{
    if (time1 > 0 && time0 > 0)
        return time0 - time1;
    return -1;
}

struct sa_tm *sa_gmtime(const sa_timef_t *timer)
{
    return _timef_to_tm(timer, 0);
}

struct sa_tm *sa_localtime(const sa_timef_t *timer)
{
    return _timef_to_tm(timer, _gmtoff());
}

sa_timef_t sa_mktime(struct sa_tm *timeptr)
{
    if (!_is_valid(timeptr)) {
        fprintf(stderr, "Invalid time.\n");
        return -1;
    }

    sa_timef_t t = 0.0;
    t += (sa_timef_t)timeptr->tm_dolya;
    t += (sa_timef_t)timeptr->tm_chast * DOL_V_CHASTE;
    t += (sa_timef_t)timeptr->tm_chas * DOL_V_CHASE;

    if (sa_svyashch_leto(timeptr->tm_leto)) {
        t += (sa_timef_t)(timeptr->tm_sorokovnik - 1) * SOROKOVNIK_POLN * DOL_V_DNE;
    } else {
        int s = timeptr->tm_sorokovnik - 1;
        while (s > 0) {
            if (s % 2) {
                t += SOROKOVNIK_POLN * DOL_V_DNE;
            } else {
                t += SOROKOVNIK_NEPOLN * DOL_V_DNE;
            }
            --s;
        }
    }

    t += (sa_timef_t)(timeptr->tm_dens) * DOL_V_DNE;

    int l = timeptr->tm_leto;
    int svya = sa_svyashch_leto(timeptr->tm_leto);
    int dni = svya ? DNEI_V_SVYA : DNEI_V_PROST;
    if (l > LETO_UNIX) {
        while (l > LETO_UNIX) {
            t += (sa_timef_t)dni * DOL_V_DNE;
            --l;
            svya = sa_svyashch_leto(l);
            dni = svya ? DNEI_V_SVYA : DNEI_V_PROST;
        }
    } else {
        while (l < LETO_UNIX) {
            t += (sa_timef_t)dni * DOL_V_DNE;
            ++l;
            svya = sa_svyashch_leto(l);
            dni = svya ? DNEI_V_SVYA : DNEI_V_PROST;
        }
        t *= -1;
    }

    *timeptr = *_timef_to_tm(&t, 0);
    return t;
}

sa_timef_t sa_strftime(char *str, sa_timef_t maxsize, const char *format, const struct sa_tm *timeptr)
{
    if (!_is_valid(timeptr)) {
        fprintf(stderr, "Invalid time.\n");
        return -1;
    }

    char buf[256];
    int ch = 0;
    unsigned int i = 0;

    while ((ch = *(format++)) && i < maxsize) {
        if (ch == '%') {
            ch = *(format++);
            switch (ch) {
            case 'a': // день недели сокр.
                strncat(str, dninedeli[timeptr->tm_denn - 1], 6);
                break;
            case 'A': // день недели
                strcat(str, dninedeli[timeptr->tm_denn - 1]);
                break;
            case 'r': // начала Рамхата
                strcat(str, dninedeli[timeptr->tm_nachr - 1]);
                break;
            case 'b': // сороковник сокр.
                strncat(str, sorokovniki[timeptr->tm_sorokovnik - 1], 6);
                break;
            case 'B': // сороковник
                strcat(str, sorokovniki[timeptr->tm_sorokovnik - 1]);
                break;
            case 'V': // время и дата
                strcat(str, sa_asctime(timeptr));
                break;
            case 'd': // день сороковника
                sprintf(buf, "%d", timeptr->tm_dens);
                strcat(str, buf);
                break;
            case 'j': // день в году
                sprintf(buf, "%d", timeptr->tm_denl);
                strcat(str, buf);
                break;
            case 's': // номер сороковника
                sprintf(buf, "%d", timeptr->tm_sorokovnik);
                strcat(str, buf);
                break;
            case 'S': // священное лето
                sprintf(buf, "%s", timeptr->tm_svya ? "Священное" : "Простое");
                strcat(str, buf);
                break;
            case 'c': // час
                sprintf(buf, "%d", timeptr->tm_chas);
                strcat(str, buf);
                break;
            case 'D': // доля
                sprintf(buf, "%d", timeptr->tm_dolya);
                strcat(str, buf);
                break;
            case 'C': // часть
                sprintf(buf, "%d", timeptr->tm_chast);
                strcat(str, buf);
                break;
            case 'w': // номер дня недели
                sprintf(buf, "%d", timeptr->tm_denn);
                strcat(str, buf);
                break;
            case 'x': // дата
                sprintf(buf, "%s/%s/%s", _prepend_zerro(timeptr->tm_dens, 2),
                        _prepend_zerro(timeptr->tm_sorokovnik, 2),
                        _prepend_zerro(timeptr->tm_leto % 100, 2));
                strcat(str, buf);
                break;
            case 'X': // время
                sprintf(buf, "%s:%s:%s", _prepend_zerro(timeptr->tm_chas, 2),
                        _prepend_zerro(timeptr->tm_chast, 3),
                        _prepend_zerro(timeptr->tm_dolya, 4));
                strcat(str, buf);
                break;
            case 'l': // лето (последние две цифры)
                sprintf(buf, "%d", timeptr->tm_leto % 100);
                strcat(str, buf);
                break;
            case 'L': // лето
                sprintf(buf, "%d", timeptr->tm_leto);
                strcat(str, buf);
                break;
            case 'k': // лето
                sprintf(buf, "%d", timeptr->tm_lvklet);
                strcat(str, buf);
                break;
            case 'K': // лето
                sprintf(buf, "%d", timeptr->tm_lvkzhizni);
                strcat(str, buf);
                break;
            case '%':
                strcat(str, "%");
                break;
            default:
                break;
            }

        } else {
            buf[0] = ch;
            strncat(str, buf, 1);
        }

        i++;
    }
    return 0;
}

sa_timef_t sa_time(sa_timef_t *sa_timer)
{
    time_t now = time(NULL);
    sa_timef_t sa_t = time_to_satime(&now);
    if (sa_timer)
        *sa_timer = sa_t;
    return sa_t;
}

// долей от начала эпохи Unix 00:00 1 января 1970 год
sa_timef_t time_to_satime(const time_t *timer)
{
    time_t now = timer ? *timer : time(NULL);
    struct tm *t = localtime(&now);

    int sa_dst = 0;
    if (t->tm_isdst && !sa_ignore_dst)
        sa_dst = SA_DST;

    sa_timef_t sa_now = -1;
    if (sizeof (now) == 4) {
        unsigned long long int v;
        v = (unsigned long long int) now;
        sa_now = (sa_timef_t)v * DOL_V_SEC + SA_GR_DIFF + sa_dst;
    } else {
        // 4(часа) * 144(доли) * 1296(части)
        // 41 Рамхатъ + 40 Айлетъ + 20 Бейлетъ (+20 потому что 1 прибавляем позднее)
        sa_now = now * DOL_V_SEC + SA_GR_DIFF + sa_dst;
    }
    return sa_now;
}

time_t satime_to_gtime(const sa_timef_t *time)
{
    return (time_t)(*time * (sa_timef_t)(SEC_V_DOLE) - SA_GR_DIFF);
}

const char *sa_sorokovnik_name(int sorokovnik)
{
    if (sorokovnik > 0 && sorokovnik <= SOROKOVNIKI)
        return sorokovniki[sorokovnik - 1];
    return "";
}

const char *sa_day_name(int weekday)
{
    if (weekday > 0 && weekday <= NEDLEYA)
        return dninedeli[weekday - 1];
    return "";
}

int sa_svyashch_leto(int leto)
{
    if (leto > 0) {
        return ((leto + SMZH) % KLET == 0);

        // поправка по Дегтярёву Сергею (каждое 512 лето простое)
    //    return ( (leto % KLET == 0) && ((leto + prevcircl + SMZH) % 512 != 0) );
    }
    return 0;
}

int sa_lvkzhizni(int leto)
{
    int kzh = (leto + SMZH) % KZHIZNI;
    return (kzh ? kzh : KZHIZNI);
}

int sa_lvklet(int leto)
{
    int klet = leto % KLET;
    return (klet ? klet : KLET);
}

int sa_kzhizni_ot_smzh(int leto)
{
    return ((leto + SMZH + KZHIZNI) / KZHIZNI);
}

int sa_leto_ot_smzh(int kzhizni_ot_smzh, int lvkzhizni)
{
    return (kzhizni_ot_smzh * KZHIZNI + lvkzhizni - SMZH - KZHIZNI);
}

int sa_stikhiya(int leto)
{
    int stikhiya = ((leto + SMZH + 1) / 2) % STIKHII;
    return (stikhiya ? stikhiya : STIKHII);
}

int sa_dennedeli(int denvlete, int nachramkh)
{
    int denn = (denvlete + nachramkh - 1) % NEDLEYA;
    return (denn ? denn : NEDLEYA);
}

sa_timef_t _gmtoff()
{
    time_t t = time(NULL);
    struct tm *tptr = localtime(&t);
    time_t local_secs = mktime(tptr);
    tptr = gmtime(&t);
    time_t gmt_secs = mktime(tptr);
    return difftime(local_secs, gmt_secs) * DOL_V_SEC;
}

int _is_valid(const struct sa_tm *t)
{
    return (t->tm_chas > 0 && t->tm_chas <= KLET) && (t->tm_chast >= 0 && t->tm_chast < CHASTI)
            && (t->tm_dolya >= 0 && t->tm_dolya < DOLI) && (t->tm_denn > 0 && t->tm_denn <= DOLI)
            && (t->tm_sorokovnik > 0 && t->tm_sorokovnik <= SOROKOVNIKI) && t->tm_dens > 0
            && (t->tm_sorokovnik % 2 == 0 ? t->tm_dens <= SOROKOVNIK_NEPOLN : t->tm_dens <= SOROKOVNIK_POLN)
            && t->tm_denl > 0 && (t->tm_svya ? t->tm_denl <= DNEI_V_SVYA : t->tm_denl <= DNEI_V_PROST)
            && (t->tm_nachr > 0 && t->tm_nachr <= DOLI) && (t->tm_lvklet > 0 && t->tm_lvklet <= KLET)
            && (t->tm_lvkzhizni > 0 && t->tm_lvkzhizni <= KZHIZNI);
}

void _reset_tm(struct sa_tm *t)
{
    t->tm_chas = 4;
    t->tm_chast = 0;
    t->tm_dolya = 0;
    t->tm_denl = 101;
    t->tm_denn = 2;
    t->tm_dens = 21;
    t->tm_lvklet = 6;
    t->tm_lvkzhizni = 102;
    t->tm_nachr = 8;
    t->tm_sorokovnik = 3;
    t->tm_svya = 0;
    t->tm_zimvr = 0;
}

char *_prepend_zerro(int number, int zerro)
{
    int c = number;
    int i = 0;
    while (c > 0) {
        c /= 10;
        i++;
    }

    char *res = (char *)malloc(100 * sizeof(char));
    *res = '\0';
    int j = zerro - i;
    while (j > 0) {
        strcat(res, "0");
        j--;
    }

    sprintf(res, "%s%d", res, number);
    return res;
}

struct sa_tm *_timef_to_tm(const sa_timef_t *sa_time, int gmtoffchas)
{
    int dolya, chast, chas, kzhizni, do7478;
    sa_timef_t t;

    if (*sa_time <= 0) {
        do7478 = 1;
        t = *sa_time * -1 + gmtoffchas;
    } else {
        do7478 = 0;
        t = *sa_time + gmtoffchas;
    }

    // доли
    dolya = fmod(t, DOLI);
    t /= DOLI;

    // части
    chast = fmod(t, CHASTI);
    t /= CHASTI;

    // часы
    chas = fmod(t, CHASY);
    chas = chas ? chas : CHASY;
    t /= CHASY;

    // лета
    int svya = 0;
    int leta = LETO_UNIX; // 1970 год
    int dvlete = DNEI_V_PROST;
    int t1 = floor(t);
    while (t1 >= dvlete) {
        t1 -= dvlete;
        leta += do7478 ? -1 : 1;
        svya = sa_svyashch_leto(leta);
        dvlete = svya ? DNEI_V_SVYA : DNEI_V_PROST;
    }
    dvlete = t = t1;
    kzhizni = sa_lvkzhizni(leta);

    // сороковники
    int sorokovnik = 1;
    int srdni = SOROKOVNIK_POLN;
    while (t > srdni) {
        t -= srdni;
        ++sorokovnik;
        srdni = svya ? SOROKOVNIK_POLN : (sorokovnik % 2 ? SOROKOVNIK_POLN : SOROKOVNIK_NEPOLN);
    }

    int i, k;
    for (i = 1, k = 0; i < kzhizni; ++i) {
        if (i % KLET != 0)
            ++k;
        if (k > 8)
            k = 0;
    }

    const int nachramh[NEDLEYA] = { 1, 6, 2, 7, 3, 8, 4, 9, 5 };
    int nr = nachramh[k];

    struct sa_tm *vm = (struct sa_tm *)malloc(sizeof(struct sa_tm));
    vm->tm_denn = sa_dennedeli(dvlete, nr);

    vm->tm_dolya = dolya;
    vm->tm_chast = chast;
    vm->tm_chas = chas;

    vm->tm_dens = t;
    vm->tm_sorokovnik = sorokovnik;
    vm->tm_denl = dvlete;

    vm->tm_lvklet = sa_lvklet(leta);
    vm->tm_lvkzhizni = kzhizni;
    vm->tm_leto = leta;

    vm->tm_svya = sa_svyashch_leto(vm->tm_leto);

    vm->tm_nachr = nr;

    time_t now = time(NULL);
    struct tm *g_t = localtime(&now);
    vm->tm_zimvr = g_t->tm_isdst;

    vm->tm_kzhizni_ot_smzh = sa_kzhizni_ot_smzh(leta);

    return vm;
}
