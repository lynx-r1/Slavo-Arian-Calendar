#ifndef KDGRAPHICSPUSHBUTTON_H
#define KDGRAPHICSPUSHBUTTON_H

#include <QGraphicsWidget>

QT_FORWARD_DECLARE_CLASS(QPropertyAnimation)
QT_FORWARD_DECLARE_CLASS(SAGraphicsButtonGroup)

class SAGraphicsPushButton : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal zValue READ zValue WRITE setZValue);

public:
    SAGraphicsPushButton();
    SAGraphicsPushButton(const QString &text, const QColor &color,
                         const QColor &textColor = QColor(),
                         QGraphicsItem *parent = 0);

    inline bool checked() const { return mChecked; }
    inline void setChecked(bool checked) {
        mChecked = checked;
        emit toggled(checked);
        update();
    }

    inline bool isCheckable() const { return mCheckable; }
    inline void setCheckable(bool checkable) { mCheckable = checkable; }

    inline bool isUnderline() const { return mUnderline; }
    inline void setUnderline(bool underline) {
        mUnderline = underline;
        update();
    }

    inline qreal jump() const { return mJump; }
    inline void setJump(qreal jump) { mJump = jump; }

    inline QPixmap pixmap() const { return mPixmap; }
    inline void setPixmap(const QPixmap &pixmap) {
        mPixmap = pixmap;
        update();
    }

    inline SAGraphicsButtonGroup *graphicsGroup() const { return mGroup; }
    inline void setGraphicsGroup(SAGraphicsButtonGroup *group) { mGroup = group; }

    inline QString text() const { return mText; }
    inline void setText(const QString &text) {
        mText = text;
        update();
    }

    inline QColor color() const { return mColor; }
    inline void setColor(const QColor &color) {
        mColor = color;
        update();
    }
    QColor backgroundColor() const { return mBackgroundColor; }
    void setBackgroundColor(const QColor &color) {
        mBackgroundColor = color;
        update();
    }

    inline void setFixedWidth(qreal width) {
        setMinimumWidth(width);
        setMaximumWidth(width);
    }
    inline void setFixedHeight(qreal height) {
        setMinimumHeight(height);
        setMaximumHeight(height);
    }
    inline void setFixedSize(const QSizeF &size) {
        setMinimumSize(size);
        setMaximumSize(size);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

signals:
    void clicked();
    void clicked(bool);
    void toggled(bool);

private:
    QPropertyAnimation *mScaleAnimation;
    QPropertyAnimation *mZValueAnimation;

    QPixmap mPixmap;
    QColor mBackgroundColor;
    QColor mColor;
    QString mText;
    QPointF mOrigPos;

    SAGraphicsButtonGroup *mGroup;

    qreal mJump;

    bool mCheckable;
    bool mChecked;
    bool mUnderline;
};

#endif // KDGRAPHICSPUSHBUTTON_H
