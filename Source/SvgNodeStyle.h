#ifndef SVGNODESTYLE_H
#define SVGNODESTYLE_H

#include <QQuickItem>

class DynamicSvg;
class SvgNodeFillStyle;
class QSvgNode;
class SvgNodeStyle : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(SvgNodeFillStyle* fill READ fill WRITE setFill NOTIFY fillChanged)

public:
    explicit SvgNodeStyle(QQuickItem* parent = 0);
    explicit SvgNodeStyle(QSvgNode* node);

    SvgNodeFillStyle* fill();

    void setFill(SvgNodeFillStyle* fill);

    void applyToNode(QSvgNode* node);

signals:
    void fillChanged();
    void styleUpdated();

public slots:

private:
    SvgNodeFillStyle* m_fill;
};

#endif // SVGNODESTYLE_H
