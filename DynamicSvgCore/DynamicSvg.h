#ifndef DYNAMICSVG_H
#define DYNAMICSVG_H

#include <QSGNode>
#include <QSGTextureProvider>
#include <QQmlListProperty>
#include <QUrl>

#include <qsvgrenderer.h>

#include "SvgNodeRef.h"

class DynamicSvgTextureProvider : public QSGTextureProvider
{
    Q_OBJECT
public:
    DynamicSvgTextureProvider()
        : m_texture(0)
        , m_smooth(false)
    {
    }

    void updateTexture(QSGTexture *texture) {
        if (m_texture == texture)
            return;
        m_texture = texture;
        emit textureChanged();
    }

    QSGTexture* texture() const {
        if (m_texture) {
            m_texture->setFiltering(m_smooth ? QSGTexture::Linear : QSGTexture::Nearest);
            m_texture->setMipmapFiltering(m_mipmap ? QSGTexture::Linear : QSGTexture::None);
            m_texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);
            m_texture->setVerticalWrapMode(QSGTexture::ClampToEdge);
        }
        return m_texture;
    }

    friend class DynamicSvg;

    QSGTexture *m_texture;
    bool m_smooth;
    bool m_mipmap;
};

class QSvgNode;
class DynamicSvg : public QQuickItem
{
    friend class SvgNodeRef;

    Q_OBJECT
    Q_DISABLE_COPY(DynamicSvg)

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QQmlListProperty<SvgNodeRef> nodeRefs READ nodeRefs NOTIFY nodeRefsChanged)
    Q_CLASSINFO("DefaultProperty", "nodeRefs")

public:
    DynamicSvg(QQuickItem* parent = 0);
    ~DynamicSvg();

    QUrl source() const;
    void setSource(QUrl source);

    QQmlListProperty<SvgNodeRef> nodeRefs();

    bool containsNode(const QString& id) const;

    bool isTextureProvider() const { return true; }
    QSGTextureProvider* textureProvider() const;

signals:
    void sourceChanged();
    void nodeRefsChanged();

protected slots:
    void componentComplete();
    void onWindowChanged(QQuickWindow* window);
    void onSceneGraphInitialized();
    void setupTexProvider();
    void renderSvg();
    QImage* svgImage();
    virtual QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);

    virtual void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

protected:
    QSvgNode* node(const QString& id) const;

    static void addNodeRef(QQmlListProperty<SvgNodeRef>* nodeRefs, SvgNodeRef* nodeRef);
    static SvgNodeRef* nodeRefAt(QQmlListProperty<SvgNodeRef>* nodeRefs, int index);
    static void clearNodeRefs(QQmlListProperty<SvgNodeRef>* nodeRefs);
    static int nodeRefCount(QQmlListProperty<SvgNodeRef>* nodeRefs);

private:
    QUrl m_source;
    QSvgRenderer m_renderer;
    QList<SvgNodeRef*> m_nodeRefs;

    QImage* m_cachedImage;
    DynamicSvgTextureProvider* m_texProvider;
    bool m_bSceneGraphInitialized;
    QList<QSGTexture*> m_ownedTextures;
};

#endif // DYNAMICSVG_H

