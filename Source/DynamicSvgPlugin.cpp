#include "DynamicSvgPlugin.h"
#include "DynamicSvg.h"
#include "SvgNodeStyle.h"
#include "SvgNodeFillStyle.h"

#include <qqml.h>

void DynamicSvgPlugin::registerTypes(const char *uri)
{
    // @uri Qtino.DynamicSvg
    qmlRegisterType<DynamicSvg>(uri, 0, 1, "DynamicSvg");
    qmlRegisterType<SvgNodeRef>(uri, 0, 1, "NodeRef");
    qmlRegisterType<SvgNodeStyle>(uri, 0, 1, "NodeStyle");
    qmlRegisterUncreatableType<SvgNodeFillStyle>(
                uri, 0, 1, "FillStyle", "FillStyle is an abstract Fill Type");
    qmlRegisterType<SvgNodeSolidColorStyle>(uri, 0, 1, "ColorFill");
    qmlRegisterType<SvgNodeGradientStyle>(uri, 0, 1, "GradientFill");
}


