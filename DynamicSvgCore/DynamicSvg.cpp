#include "DynamicSvg.h"

#include <qsvgtinydocument_p.h>
#include <qsvgnode_p.h>

#include <QQuickWindow>
#include <QSGSimpleTextureNode>
#include <QDir>

DynamicSvg::DynamicSvg(QQuickItem* parent)
    : QQuickItem(parent),
      m_cachedImage(0),
      m_texProvider(0),
      m_bSceneGraphInitialized(false)
{
    QQuickItem::setFlag(QQuickItem::ItemHasContents, true);
    setAntialiasing(true);

    connect(this, SIGNAL(windowChanged(QQuickWindow*)),
            this, SLOT(onWindowChanged(QQuickWindow*)));
    connect(this, SIGNAL(sourceChanged()),
            this, SLOT(renderSvg()));
    connect(this, SIGNAL(nodeRefsChanged()),
            this, SLOT(renderSvg()));

    if (window()) {
        connect(window(), SIGNAL(sceneGraphInitialized()), this, SLOT(onSceneGraphInitialized()));
    }
}

DynamicSvg::~DynamicSvg()
{
//    qDebug() << "Destroying DynamicSvg";
//    QTime timer; timer.start();
//    if (m_cachedImage) delete m_cachedImage;
//    if (m_texProvider) delete m_texProvider;
//    qDeleteAll(m_ownedTextures);
//    m_ownedTextures.clear();
//    qDebug() << "Finished Destroying DynamicSvg (took" << timer.elapsed() << "ms)";
}

QUrl DynamicSvg::source() const
{
    return m_source;
}

void DynamicSvg::setSource(QUrl source)
{
    if (source == m_source) return;

    m_source = source;
    if (source.isLocalFile())
        m_renderer.load(source.path());
    else
        m_renderer.load(source.toString());
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

void DynamicSvg::renderSvg()
{
    if (!m_renderer.isValid()) return;
    if (!window()) return;
    if (!m_bSceneGraphInitialized) return;

    int width = qMax(1.0, this->width());
    int height = qMax(1.0, this->height());

    QRectF bounds = QRectF(QPointF(0,0), QSizeF(width, height));

    qreal dpr = window()->devicePixelRatio();
    QSizeF scale(dpr, dpr);
    QSize finalSize = QSize(width * dpr, height * dpr);

    QTransform transform;
    transform.scale(scale.width(), scale.height());
    bounds = transform.mapRect(bounds);

    if (!m_cachedImage) {
        m_cachedImage = new QImage(finalSize, QImage::Format_ARGB32_Premultiplied);
    }
    else if (m_cachedImage->size() != finalSize) {
        delete m_cachedImage;
        m_cachedImage = new QImage(finalSize, QImage::Format_ARGB32_Premultiplied);
    }
    m_cachedImage->fill(Qt::transparent);
    QPainter imgPainter(m_cachedImage);
    m_renderer.render(&imgPainter, bounds);

    update();
}

QImage* DynamicSvg::svgImage()
{
    if (!m_cachedImage) renderSvg();
    return m_cachedImage;
}

void DynamicSvg::setupTexProvider()
{
    //This provider is actually not used anymore...need to take out
    if (!m_texProvider) {
        m_texProvider = new DynamicSvgTextureProvider();
        m_texProvider->m_smooth = true;
        m_texProvider->m_mipmap = false;
        renderSvg();
    }
}

QSGTextureProvider* DynamicSvg::textureProvider() const
{
    return m_texProvider;
}

void DynamicSvg::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    if ((newGeometry.size() != oldGeometry.size()) && isVisible()) {
        renderSvg();
    }
}

void DynamicSvg::componentComplete()
{
    QQuickItem::componentComplete();
    if (window() && window()->openglContext())
        m_bSceneGraphInitialized = true;
    renderSvg();
}

void DynamicSvg::onWindowChanged(QQuickWindow* window)
{
    if (!window) { this->deleteLater(); return; }
    if (window->openglContext())
        onSceneGraphInitialized();
    else
        connect(window, SIGNAL(sceneGraphInitialized()), this, SLOT(onSceneGraphInitialized()));
    setupTexProvider();
}

void DynamicSvg::onSceneGraphInitialized()
{
    m_bSceneGraphInitialized = true;
    renderSvg();
}

QSGNode* DynamicSvg::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    if (!m_bSceneGraphInitialized) return node;

    if (!node) {
        node = new QSGSimpleTextureNode();
        ((QSGSimpleTextureNode*)node)->setRect(this->boundingRect());
    }
    QSGSimpleTextureNode* textureNode = ((QSGSimpleTextureNode*)node);

    QSGTexture* texture = window()->createTextureFromImage(*m_cachedImage,
          QQuickWindow::TextureOwnsGLTexture);
    m_ownedTextures.append(texture);
    textureNode->setTexture(texture);

    qreal dpr = window() ? window()->devicePixelRatio() : 1.0;
    int width = textureNode->texture()->textureSize().width() / dpr;
    int height = textureNode->texture()->textureSize().height() / dpr;
    textureNode->setRect(0, 0, width, height);

    //clear old textures
    foreach (QSGTexture* tex, m_ownedTextures) {
        if (tex != texture) {
            m_ownedTextures.removeAll(tex);
            tex->deleteLater();
        }
    }

    return node;
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

