import QtQuick 2.2
import Qtino.DynamicSvg 0.1

Rectangle {
    width: 400
    height: 400

    MouseArea {
        id: clickArea
        anchors.fill: parent
    }

    DynamicSvg {
        id: svg
        anchors.fill: parent
        source: "icon.svg"

        NodeRef {
            id: circleNode
            nodeId: "circle"
            style: NodeStyle {
                fill: ColorFill {
                    //TODO: Make 'DefaultStyle' an attached property of NodeStyle
                    color: clickArea.pressed ? 'purple': circleNode.defaultStyle.fill.color
                    Behavior on color { ColorAnimation { duration : 300 } }
                }
            }
        }
    }
}

