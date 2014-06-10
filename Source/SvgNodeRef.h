#ifndef SVGNODEREF_H
#define SVGNODEREF_H

#include <QQuickItem>

#include "SvgNodeStyle.h"

class SvgNodeRef : public QQuickItem
{
    friend class DynamicSvg;

    Q_OBJECT

    Q_PROPERTY(QString nodeId READ nodeId WRITE setNodeId NOTIFY nodeIdChanged)
    Q_PROPERTY(SvgNodeStyle* style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(SvgNodeStyle* defaultStyle READ defaultStyle NOTIFY defaultStyleChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged STORED false)

    Q_CLASSINFO("DefaultProperty", "style")

public:
    explicit SvgNodeRef(DynamicSvg* parent = 0);

    QString nodeId();
    void setNodeId(QString nodeId);

    SvgNodeStyle* style();
    void setStyle(SvgNodeStyle* style);

    SvgNodeStyle* defaultStyle();

    bool valid();

signals:
    void nodeIdChanged();
    void styleChanged();
    void defaultStyleChanged();
    void validChanged();

public slots:
    void triggerUpdate();

protected:
    void setSvg(DynamicSvg* svg);
    void initDefaultStyle();
    void applyStyle();

private:
    QString m_nodeId;
    SvgNodeStyle* m_style;
    SvgNodeStyle* m_defaultStyle;
    DynamicSvg* m_svg;
};

#endif // SVGNODEREF_H
