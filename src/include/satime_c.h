#ifndef SATIME_H
#define SATIME_H

#include <time.h>

typedef long double sa_timef_t;

struct sa_tm {
    int tm_dolya;           // доли                     [1 - 1296]
    int tm_chast;           // части                    [1 - 144]
    int tm_chas;            // часы                     [1 - 16]
    int tm_dens;            // дни сороковника          [1 - 41]
    int tm_denn;            // дни недели 9             [1 - 9]
    int tm_denl;            // день в году              [1 - 365]
    int tm_sorokovnik;      // сороковники 9            [1 - 9]
    int tm_leto;            // лета
    int tm_lvklet;          // лето в круге лет         [1 - 16]
    int tm_lvkzhizni;       // лето в круге жизни       [1 - 144]
    int tm_kzhizni_ot_smzh;  // круг жизни с С.М.З.Х.
    int tm_svya;            // лето священное?
    int tm_nachr;           // начало Рамхата           [1 - 9]
    int tm_zimvr;           // переход на зимнее время  [-1/0/1] (0 не учитывать зим. время,
                            // >0 учитывать зим. время, <0 информация не доступна)
};

extern int sa_ignore_dst;

#ifdef __cplusplus
extern "C" {
#endif
    char *sa_asctime(const struct sa_tm *timeptr);
    char *sa_ctime(const sa_timef_t *timer);

    double sa_difftime(sa_timef_t time1, sa_timef_t time2);

    struct sa_tm *sa_gmtime(const sa_timef_t *timer);
    struct sa_tm *sa_localtime(const sa_timef_t *timer);

    sa_timef_t sa_mktime(struct sa_tm *timeptr);

    sa_timef_t sa_strftime(char *str, sa_timef_t maxsize, const char *format, const struct sa_tm *timeptr);

    sa_timef_t sa_time(sa_timef_t *sa_timer);
    sa_timef_t time_to_satime(const time_t *time);
    time_t satime_to_gtime(const sa_timef_t *time);

    const char *sa_sorokovnik_name(int sorokovnik);
    const char *sa_day_name(int weekday);

    int sa_svyashch_leto(int leto);

    int sa_lvkzhizni(int leto);
    int sa_lvklet(int leto);

    int sa_kzhizni_ot_smzh(int leto);
    int sa_leto_ot_smzh(int kzhizni_ot_smzh, int lvkzhizni);

    int sa_stikhiya(int leto);

    int sa_dennedeli(int denvlete, int nachramkh);
#ifdef __cplusplus
}
#endif

#endif // SATIME_H
