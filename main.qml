import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("SVG to QML converter. By pavelk.ru. V1.0.")

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        CompareQML {
            anchors.fill: parent
        }

        GeneratedQML {
            anchors.fill: parent
        }

    }

}
