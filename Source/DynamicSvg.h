#ifndef DYNAMICSVG_H
#define DYNAMICSVG_H

#include <QQuickPaintedItem>
#include <qsvgrenderer.h>
#include <QQmlListProperty>

#include "SvgNodeRef.h"

class QSvgNode;
class DynamicSvg : public QQuickPaintedItem
{
    friend class SvgNodeRef;

    Q_OBJECT
    Q_DISABLE_COPY(DynamicSvg)

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QQmlListProperty<SvgNodeRef> nodeRefs READ nodeRefs NOTIFY nodeRefsChanged)
    Q_CLASSINFO("DefaultProperty", "nodeRefs")

public:
    DynamicSvg(QQuickItem* parent = 0);
    ~DynamicSvg();

    QString source() const;
    void setSource(QString source);

    QQmlListProperty<SvgNodeRef> nodeRefs();

    bool containsNode(const QString& id) const;

signals:
    void sourceChanged();
    void nodeRefsChanged();

protected slots:
    void paint(QPainter* painter);

protected:
    QSvgNode* node(const QString& id) const;

    static void addNodeRef(QQmlListProperty<SvgNodeRef>* nodeRefs, SvgNodeRef* nodeRef);
    static SvgNodeRef* nodeRefAt(QQmlListProperty<SvgNodeRef>* nodeRefs, int index);
    static void clearNodeRefs(QQmlListProperty<SvgNodeRef>* nodeRefs);
    static int nodeRefCount(QQmlListProperty<SvgNodeRef>* nodeRefs);

private:
    QString m_source;
    QSvgRenderer m_renderer;
    QList<SvgNodeRef*> m_nodeRefs;
};

#endif // DYNAMICSVG_H

