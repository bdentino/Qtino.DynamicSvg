import QtQuick 2.2
import QtQuick.Window 2.1
import Qtino.DynamicSvg 0.1

Window {
    id: root
    width: 400
    height: 400

    visible: true

    MouseArea {
        id: clickArea
        anchors.fill: svg
    }

    Image {
        id: img
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -parent.height / 4
        source: 'icon.svg'

        height: parent.height * 0.4
        width: height

        sourceSize: Qt.size(width * 2, height * 2);
    }

    DynamicSvg {
        id: svg
        anchors.centerIn: parent
        anchors.verticalCenterOffset: parent.height / 4
        source: "icon.svg"

        height: parent.height * 0.4
        width: height

        NodeRef {
            id: circleNode
            nodeId: "circlePart"
            style: NodeStyle {
                fill: ColorFill {
                    //TODO: Make 'DefaultStyle' an attached property of NodeStyle
                    id: colorFill
                    color: clickArea.pressed ? 'purple': circleNode.defaultStyle.fill.color
                    Behavior on color { ColorAnimation { duration : 1000 } }
                }
            }
        }
    }
}

