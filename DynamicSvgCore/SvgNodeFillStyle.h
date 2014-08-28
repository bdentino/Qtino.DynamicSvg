#ifndef SVGNODEFILLSTYLE_H
#define SVGNODEFILLSTYLE_H

#include <QQuickItem>
#include <QGradient>

/*
 * TODO: What about handling url(#...) references?
 *       - Should the user be able to specify a url in Qml? (initial thought, no)
 *       - Can the user access fill style nodes using nodeId? (should be able to...)
 */

class QSvgNode;
class SvgNodeFillStyle : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(Qt::FillRule fillRule READ fillRule WRITE setFillRule NOTIFY fillUpdated)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY fillUpdated)

public:
    explicit SvgNodeFillStyle(QQuickItem* parent = 0);

    Qt::FillRule fillRule() const;
    qreal opacity() const;

    void setFillRule(Qt::FillRule fillRule);
    void setOpacity(qreal opacity);

    virtual void applyToNode(QSvgNode* node);

signals:
    void fillUpdated();

public slots:

private:
    Qt::FillRule m_fillRule;
    qreal m_opacity;

};

class SvgNodeSolidColorStyle : public SvgNodeFillStyle
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorUpdated)

public:
    explicit SvgNodeSolidColorStyle(QQuickItem* parent = 0);

    const QColor& color() const;

    void setColor(QColor color);

    void applyToNode(QSvgNode* node);

signals:
    void colorUpdated();

private:
    QColor m_color;
};

class SvgNodeGradientStyle : public SvgNodeFillStyle
{
    Q_OBJECT

    Q_PROPERTY(QGradient gradient READ gradient WRITE setGradient NOTIFY gradientUpdated)
    //TODO: Transform Matrix Property (for rotated/translated gradients)
    //TODO: Will this support radial gradients as well?

public:
    explicit SvgNodeGradientStyle(QQuickItem* parent = 0);

    const QGradient& gradient() const;

    void setGradient(QGradient gradient);

    void applyToNode(QSvgNode* node);

signals:
    void gradientUpdated();

private:
    QGradient m_gradient;
};

#endif // SVGNODEFILLSTYLE_H
