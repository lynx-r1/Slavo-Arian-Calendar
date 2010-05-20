#ifndef DK4_H
#define DK4_H

#include <QGraphicsWidget>
#include <QSqlDatabase>

QT_FORWARD_DECLARE_CLASS(QButtonGroup)
QT_FORWARD_DECLARE_CLASS(QGraphicsGridLayout)
QT_FORWARD_DECLARE_CLASS(QGraphicsLinearLayout)

QT_FORWARD_DECLARE_CLASS(SAGraphicsPushButton)
QT_FORWARD_DECLARE_CLASS(SAGraphicsButtonGroup)
QT_FORWARD_DECLARE_CLASS(SAGraphicsLabelItem)

QT_FORWARD_DECLARE_CLASS(DK4KZhizniLineEdit)
QT_FORWARD_DECLARE_CLASS(DK4Leto)

class DK4Base : public QGraphicsWidget
{
    Q_OBJECT

public:
    DK4Base(QGraphicsItem *parent = 0, bool deflt = true);

    virtual void init();

    inline int lvkzhizni() const { return mKZhizni; }
    void setKZhizni(int kzhizni);

    inline bool useAltColor() const { return mAltColor; }
    inline void setAltColor(bool useAltColor, bool recreate = false) {
        mAltColor = useAltColor;
        if (recreate) {
            deleteDK4();
            createDK4();
        }
    }

    inline int kzhizniOtSMZH() const { return mKZhizniOtSMZH; }
    void setKZhizniOtSMZH(int kzhizniOtSMZH) {
        if (kzhizniOtSMZH != mKZhizniOtSMZH) {
            mKZhizniOtSMZH = kzhizniOtSMZH;
            emit kzhizniOtSMZHChanged(kzhizniOtSMZH);
        }
    }

    int checkedKZhizniId() const;
    DK4Leto *checkedKZhizni() const;
    DK4Leto *lvkzhizni(int lvkzhizni) const;

    virtual void createDK4();
    virtual void deleteDK4();

    virtual DK4Leto *letoPrototype() const { return mLetoPrototype; }
    virtual void setLetoPrototype(DK4Leto *letoPrototype) { mLetoPrototype = letoPrototype; }

public slots:
    void sledKZhizniOtSMZH();
    void predKZhizniOtSMZH();

signals:
    void selected(int kzhizni);
    void kzhizniOtSMZHChanged(int kzhizniOtSMZH);

private slots:
    void kzhizniClicked(int kzhizni);

protected:
    inline SAGraphicsButtonGroup * dk4ButtonGroup() const { return mDK4ButtonGroup; }

    DK4Leto *createLeto(int nomerLeta, int denNedeliNovoletiya, int krugLet,
                        int stikhiyaICvet, int nachalo);

    virtual QGraphicsLayout *shemaKrugoleta();
    virtual QGraphicsWidget *dobavitLeto(int nomer, int krugLet, int cvet, int nachalo);

private:
    SAGraphicsButtonGroup * mDK4ButtonGroup;
    DK4Leto *mLetoPrototype;

    int mKZhizni;
    int mKZhizniOtSMZH;

    bool mAltColor;
};

class DK4DB : public DK4Base
{
    Q_OBJECT

public:
    DK4DB(QGraphicsItem *parent = 0, bool deflt = true);

    QSqlDatabase openOpisanieLetDb(int kzhizniFromSMZH);
    QSqlDatabase openOpisanieLetDb(const QString &name, const QString &connectionName);

    inline QString dbOpisanieLetConnectionName() const { return mOpisanieLetDbConnectionName; }
    inline void setDbOpisanieLetConnectionName(const QString &conName) { mOpisanieLetDbConnectionName = conName; }

protected:
    bool getInfoFromDB(int kzhizni, QString &name, QString &descr, QString &symbol,
                       QString &power, QString &color);

private slots:
    void reopenOpisanieLetDb(int kzhizniFromSMZH);

private:
    QString mOpisanieLetDbConnectionName;
    QString mOpisanieLetDbName;
};

class DK4 : public DK4DB
{
    Q_OBJECT

public:
    DK4(QGraphicsItem *parent = 0, bool deflt = true);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void createDK4();
    void deleteDK4();

    inline QColor conturColor() const { return mConturColor; }
    inline void setConturColor(const QColor &color) {
        mConturColor = color;
        update();
    }

    inline QColor diagonalColor() const { return mDiagonalColor; }
    inline void setDiagonalColor(const QColor &color) {
        mDiagonalColor = color;
        update();
    }

signals:
    void showInfoRequest(bool, const QString &);

protected:
    QGraphicsWidget *dobavitLeto(int nomer, int krugLet, int cvet, int nachalo);

private slots:
    void showInfoHtml(bool show);
    void updateDate();
    void updateInfoLabel();
    void updateInfoHtml();
    void updateKZhizniOtSMZH();
    void updateSeichasKZhizni();

private:
    void setupConnections();
    QGraphicsLayout *shemaNavig();

private:
    QColor mConturColor;
    QColor mDiagonalColor;

    SAGraphicsLabelItem *mKZhizniLabelOtSMZH;
    DK4KZhizniLineEdit *mKZhizniLineEdit;

    int mSeichasKZhizniOtSMZH;
    int mSeichasLetoVKZhizni;
};

#endif // DK4_H

