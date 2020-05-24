import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtGraphicalEffects 1.12 as GE

Item {
    id: svg2qml
    property var scaleShape: Qt.size(1, 1)
    property bool thinkLines: false
    Item {
        width: svg2qml.width
        height: svg2qml.height
        Item {
            id: svg8
            width: svg2qml.width
            height: svg2qml.height
            Item {
                id: layer1
                width: svg2qml.width
                height: svg2qml.height
                Shape {
                    id: path817
                    width: svg2qml.width
                    height: svg2qml.height
                    ShapePath {
                        PathSvg {
                            path: "M 0,271.2 C 0,271.2 573.2,-164.3 455.5,68.5 C 337.8,301.3 215.2,378.9 485.5,406.4 C 755.8,433.9 755.8,669.2 430.4,701.8 C 105,734.4 0,271.2 0,271.2 Z"
                        }
                        strokeColor: (svg2qml.thinkLines)? "black" : "#ffff0000"
                        strokeWidth: (svg2qml.thinkLines)? 1 : 10
                        fillColor: "transparent"
                    }
                }
            }
        }
    }
}
