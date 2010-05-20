#ifndef KRUGOVYIECHASYI_H
#define KRUGOVYIECHASYI_H

#include <QWidget>

class KDTimeTimer;

class KrugovyeChasy : public QWidget
{
    Q_OBJECT

public:
    KrugovyeChasy(bool app = false, QWidget *parent = 0);
    QSize sizeHint() const;

    bool hasEffects() { return mEffects; }
    bool isPaused() { return mPausa; }

public slots:
    void effects(bool effects);
    void pause(bool pause);

protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void resizeEvent(QResizeEvent *);
    bool event(QEvent *);

private slots:
    void quit();
    void setDolyu(int);
    void setChast(int);
    void setChas(int);

private:
    void readSettings();
    void writeSettings();

    void setTime(QString time);

    QList<QPixmap> mChasyKartinki;

    QPoint mDragPos;
    QPoint mPrevPos;

    KDTimeTimer *mTimer;

    int mChas;
    int mChast;
    int mDolya;

    // бегает по кругу и вращает стрелки
    int mVolchok;
    int mInterval;

    bool mPausa;
    bool mEffects;
    bool mApp;
};

#endif // KRUGOVYIECHASYI_H
