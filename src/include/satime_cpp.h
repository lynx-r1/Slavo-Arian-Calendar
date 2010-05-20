#ifndef KDTIME_H
#define KDTIME_H

#include <QString>

class SATime
{
public:
    SATime() : mIsValid(false)
    {}
    SATime(int chas, int chast, int dolya = 0);

    inline int dolya() const { return mDolya; }
    inline int chast() const { return mChast; }
    inline int chas() const { return mChas; }

    bool isValid() const;

    bool setCCD(int chas, int chast, int dolya);

    /*!
      с - час без нуля впереди (0 - 16)
      сc - час с нулём впереди (00 - 16)
      С - часть без нулей впереди (0 - 144)
      ССС - часть с нулями впереди (000 - 144)
      D - доля без нулей впереди (0 - 1296)
      DDDD -доля с нулями впереди (0000 - 1296)
    */
    QString toString(const QString &format) const;

    static SATime currentTime(bool ignoreDST = false);
    static bool isValid(int chas, int chast, int dolya);

    static QString chasText(int chas);
    static QString chastText(int chast);
    static QString dolyaText(int dolya);

private:
    bool capLetters(const QString &str, const QChar &ch, int pos, int count) const;

private:
    int mChas;
    int mChast;
    int mDolya;

    bool mIsValid;
};

#endif // KDTIME_H
