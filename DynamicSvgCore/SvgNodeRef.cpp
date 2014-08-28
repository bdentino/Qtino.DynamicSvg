#include "SvgNodeRef.h"

#include "DynamicSvg.h"
#include <qsvgnode_p.h>

SvgNodeRef::SvgNodeRef(DynamicSvg* parent)
    : QQuickItem(parent),
      m_nodeId(""),
      m_style(NULL),
      m_defaultStyle(NULL),
      m_svg(parent)
{
    setSvg(parent);
    connect(this, SIGNAL(nodeIdChanged()), this, SLOT(triggerUpdate()));
    connect(this, SIGNAL(styleChanged()), this, SLOT(triggerUpdate()));
}

void SvgNodeRef::setSvg(DynamicSvg* svg)
{
    if (!svg) return;

    if (m_svg) {
        disconnect(m_svg);
        m_svg->disconnect(this);
    }

    m_svg = svg;
    connect(m_svg, SIGNAL(sourceChanged()), this, SIGNAL(validChanged()));
    initDefaultStyle();
    triggerUpdate();
    emit validChanged();
}

QString SvgNodeRef::nodeId()
{
    return m_nodeId;
}

void SvgNodeRef::setNodeId(QString nodeId)
{
    if (m_nodeId == nodeId) return;
    m_nodeId = nodeId;
    initDefaultStyle();
    emit nodeIdChanged();
    emit validChanged();
}

SvgNodeStyle* SvgNodeRef::style()
{
    return m_style;
}

void SvgNodeRef::setStyle(SvgNodeStyle* style)
{
    if (m_style == style) return;
    if (m_style) {
        disconnect(m_style);
        m_style->disconnect(this);
    }

    m_style = style;
    connect(m_style, SIGNAL(styleUpdated()), this, SLOT(triggerUpdate()));
    triggerUpdate();
    emit styleChanged();
}

void SvgNodeRef::initDefaultStyle()
{
    if (m_defaultStyle)
    {
        delete m_defaultStyle;
        m_defaultStyle = NULL;
    }
    if (!valid())
    {
        emit defaultStyleChanged();
        return;
    }

    m_defaultStyle = new SvgNodeStyle(m_svg->node(m_nodeId));
    emit defaultStyleChanged();
}

SvgNodeStyle* SvgNodeRef::defaultStyle()
{
    return m_defaultStyle;
}

//TODO: This will only return valid for visible elements, so user won't be able
//      to modify defs like gradients/fills, etc
bool SvgNodeRef::valid()
{
    if (!m_svg) return false;
    return m_svg->containsNode(m_nodeId);
}


void SvgNodeRef::triggerUpdate()
{
    if (valid() && m_style) {
        m_style->applyToNode(m_svg->node(m_nodeId));
    }
    if (m_svg) {
        m_svg->renderSvg();
    }
}
