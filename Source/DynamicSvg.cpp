#include "DynamicSvg.h"

#include <qsvgtinydocument_p.h>
#include <qsvgnode_p.h>

DynamicSvg::DynamicSvg(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{
    setFlag(ItemHasContents, true);
}

DynamicSvg::~DynamicSvg()
{
}

QString DynamicSvg::source() const
{
    return m_source;
}

void DynamicSvg::setSource(QString source)
{
    if (source == m_source) return;
    m_source = source;
    m_renderer.load(source);
    emit sourceChanged();
}

bool DynamicSvg::containsNode(const QString &id) const
{
    return m_renderer.getDocument()->elementExists(id);
}

QQmlListProperty<SvgNodeRef> DynamicSvg::nodeRefs()
{
    return QQmlListProperty<SvgNodeRef>(this, 0,
                                        &DynamicSvg::addNodeRef,
                                        &DynamicSvg::nodeRefCount,
                                        &DynamicSvg::nodeRefAt,
                                        &DynamicSvg::clearNodeRefs);
}

void DynamicSvg::paint(QPainter* painter)
{
    qDebug() << "Rendering";
    m_renderer.render(painter);
}

void DynamicSvg::addNodeRef(QQmlListProperty<SvgNodeRef>* nodeRefs, SvgNodeRef* nodeRef)
{
    DynamicSvg* svg = qobject_cast<DynamicSvg*>(nodeRefs->object);
    if (!svg) return;
    nodeRef->setSvg(svg);
    svg->m_nodeRefs.append(nodeRef);
}

SvgNodeRef* DynamicSvg::nodeRefAt(QQmlListProperty<SvgNodeRef>* nodeRefs, int index)
{
    DynamicSvg* svg = qobject_cast<DynamicSvg*>(nodeRefs->object);
    if (!svg) return NULL;
    return svg->m_nodeRefs.at(index);
}

void DynamicSvg::clearNodeRefs(QQmlListProperty<SvgNodeRef>* nodeRefs)
{
    DynamicSvg* svg = qobject_cast<DynamicSvg*>(nodeRefs->object);
    if (!svg) return;
    svg->m_nodeRefs.clear();
}

int DynamicSvg::nodeRefCount(QQmlListProperty<SvgNodeRef>* nodeRefs)
{
    DynamicSvg* svg = qobject_cast<DynamicSvg*>(nodeRefs->object);
    if (!svg) return 0;
    return svg->m_nodeRefs.count();
}

QSvgNode* DynamicSvg::node(const QString &id) const
{
    return m_renderer.getDocument()->namedNode(id);
}
