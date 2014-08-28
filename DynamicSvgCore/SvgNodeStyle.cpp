#include "SvgNodeStyle.h"
#include "DynamicSvg.h"
#include "SvgNodeFillStyle.h"

#include <qsvgnode_p.h>
#include <qsvgstyle_p.h>

SvgNodeStyle::SvgNodeStyle(QQuickItem* parent)
    : QQuickItem(parent),
      m_fill(NULL)
{
}

SvgNodeStyle::SvgNodeStyle(QSvgNode* node)
{
    QSvgFillStyle* fill = dynamic_cast<QSvgFillStyle*>(node->styleProperty(QSvgStyleProperty::FILL));
    if (fill) {
        QSvgFillStyleProperty* fillStyle = fill->style();
        SvgNodeFillStyle* defaultFill = NULL;
        if (fillStyle) {

        }
        else {
            QBrush brush = fill->qbrush();
            if (brush.style() == Qt::SolidPattern)
            {
                SvgNodeSolidColorStyle* solidFill = new SvgNodeSolidColorStyle(this);
                solidFill->setColor(brush.color());
                defaultFill = solidFill;
            }
            else if (brush.style() == Qt::LinearGradientPattern ||
                     brush.style() == Qt::RadialGradientPattern ||
                     brush.style() == Qt::ConicalGradientPattern)
            {
                SvgNodeGradientStyle* gradientFill = new SvgNodeGradientStyle(this);
                gradientFill->setGradient(QGradient(*(brush.gradient())));
                defaultFill = gradientFill;
            }
        }
        if (defaultFill)
        {
            defaultFill->setOpacity(fill->fillOpacity());
            defaultFill->setFillRule(fill->fillRule());
            setFill(defaultFill);
        }
    }
}

SvgNodeFillStyle* SvgNodeStyle::fill()
{
    return m_fill;
}

void SvgNodeStyle::setFill(SvgNodeFillStyle* fill)
{
    if (m_fill == fill) return; //TODO: Equality testing for fillStyle
    m_fill = fill;
    connect(m_fill, SIGNAL(fillUpdated()), this, SIGNAL(styleUpdated()));
    emit fillChanged();
}

void SvgNodeStyle::applyToNode(QSvgNode* node)
{
    if (m_fill)
        m_fill->applyToNode(node);
}
