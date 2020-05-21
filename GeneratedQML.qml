import QtQuick 2.14

Item {
    id: generatedQML
    property bool loaded: typeof(appCore)!=="undefined"

    Loader {
        anchors.fill: parent
        source: (generatedQML.loaded)? "file:/"+appCore.generateQMLPath : ""
    }
}
