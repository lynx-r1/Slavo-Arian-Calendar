#ifndef SACHASYAPP_H
#define SACHASYAPP_H

#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QLabel)

QT_FORWARD_DECLARE_CLASS(SAChasy)
QT_FORWARD_DECLARE_CLASS(SAChasyAppSettingsDialog)

class SAChasyApp : public QGraphicsView
{
    Q_OBJECT

public:
    SAChasyApp(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void showAbout();

private:
    void readSettings();
    void writeSettings();

    void createActions();
    void createSAChasy();

private:
    QAction *mSettingsAction;
    QAction *mAboutAction;
    QAction *mQuitAction;

    QLabel *mDateLabel;

    QPoint mDragPosition;

    QColor mBgColor;
    QColor mShadowColor;

    SAChasyAppSettingsDialog *mSettingsDialog;
    SAChasy *mSAChasy;
};

#endif // SACHASYAPP_H
