#ifndef KDDATE_H
#define KDDATE_H

#include <QString>

class SADate
{
public:
    SADate() : mIsValid(false) {}
    SADate(int leto, int sorok, int den);

    inline int day() const { return mDay; }
    inline int sorokovnik() const { return mSorokovnik; }
    inline int leto() const { return mLeto; }
    inline int lvklet() const { return mKLet; }
    inline int lvkzhizni() const { return mKZhizni; }
    inline int kzhizniFromSMZH() const { return mKZhizniFromSMZH; }
    inline int dayOfWeek() const { return mDayOfWeek; }
    inline int dayOfLeta() const { return mDayOfLeta; }
    inline int nachRamkhata() const { return mNachRamkhata; }

    inline int isDST() const { return mIsDST; }
    inline bool isSvyashchLeto() const { return mIsSvyashchLeto; }

    int daysInSorok() const;
    int daysInLete() const;

    void getDate(int *leto, int *sorok, int *den);
    bool setDate(int leto, int sorok, int den);

    bool isValid() const;

    QString toString(const QString &format) const;

    static SADate currentDate();

    static int nachRamkhata(int leto);
    static int lvkzhizni(int leto);
    static int lvklet(int leto);
    static int kzhizniFromSMZH(int leto);
    static int letoFromSMZH(int kzhizniFromSMZH, int lvkzhizni);
    static int stikhiya(int leto);

    static bool isValid(int sorok, int den);
    static bool isSvyashchLeto(int leto);

    static QString longDayName(int weekday);
    static QString longSorokName(int sorok);
    static QString shortDayName(int weekday);
    static QString shortSorokName(int sorok);

private:
    bool capLetters(const QString &str, const QChar &ch, int pos, int count) const;

    static struct sa_tm *saTime();

private:
    int mLeto;
    int mSorokovnik;
    int mDay;
    int mDayOfWeek;
    int mDayOfLeta;
    int mKLet;
    int mKZhizni;
    int mKZhizniFromSMZH;
    int mNachRamkhata;
    int mIsDST;

    bool mIsSvyashchLeto;
    bool mIsValid;
};

#endif // KDDATE_H
