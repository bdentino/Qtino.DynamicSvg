#include "SvgNodeFillStyle.h"

#include <qsvgnode_p.h>
#include <qsvgstyle_p.h>

SvgNodeFillStyle::SvgNodeFillStyle(QQuickItem* parent)
    : QQuickItem(parent),
      m_fillRule(Qt::WindingFill),
      m_opacity(1.0)
{
    connect(this, SIGNAL(fillUpdated()), this, SLOT(update()));
}

Qt::FillRule SvgNodeFillStyle::fillRule() const
{
    return m_fillRule;
}

qreal SvgNodeFillStyle::opacity() const
{
    return m_opacity;
}

void SvgNodeFillStyle::setFillRule(Qt::FillRule fillRule)
{
    if (m_fillRule == fillRule) return;
    m_fillRule = fillRule;
    emit fillUpdated();
}

void SvgNodeFillStyle::setOpacity(qreal opacity)
{
    if (m_opacity == opacity) return;
    m_opacity = opacity;
    emit fillUpdated();
}

void SvgNodeFillStyle::applyToNode(QSvgNode* node)
{
    QSvgFillStyle* fillStyle = new QSvgFillStyle();
    fillStyle->setFillOpacity(opacity());
    fillStyle->setFillRule(fillRule());
    node->appendStyleProperty(fillStyle, "");
}

SvgNodeSolidColorStyle::SvgNodeSolidColorStyle(QQuickItem* parent)
    : SvgNodeFillStyle(parent),
      m_color(QColor(Qt::white))
{

}

const QColor& SvgNodeSolidColorStyle::color() const
{
    return m_color;
}

void SvgNodeSolidColorStyle::setColor(QColor color)
{
    if (m_color == color) return;
    m_color = color;
    emit fillUpdated();
}

void SvgNodeSolidColorStyle::applyToNode(QSvgNode* node)
{
    QSvgFillStyle* fillStyle = new QSvgFillStyle();
    fillStyle->setFillOpacity(opacity());
    fillStyle->setFillRule(fillRule());
    fillStyle->setBrush(QBrush(m_color));
    node->appendStyleProperty(fillStyle, "");
}

SvgNodeGradientStyle::SvgNodeGradientStyle(QQuickItem* parent)
    : SvgNodeFillStyle(parent)
{

}

const QGradient& SvgNodeGradientStyle::gradient() const
{
    return m_gradient;
}

void SvgNodeGradientStyle::setGradient(QGradient gradient)
{
    if (m_gradient == gradient) return;
    m_gradient = gradient;
    emit fillUpdated();
}

void SvgNodeGradientStyle::applyToNode(QSvgNode* node)
{
    QSvgFillStyle* fillStyle = new QSvgFillStyle();
    fillStyle->setFillOpacity(opacity());
    fillStyle->setFillRule(fillRule());
    fillStyle->setBrush(QBrush(m_gradient));
    node->appendStyleProperty(fillStyle, "");
}
