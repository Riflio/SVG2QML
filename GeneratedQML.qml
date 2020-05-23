import QtQuick 2.12

Item {
    id: generatedQML
    property bool loaded: typeof(appCore)!=="undefined"

    Loader {
        anchors.fill: parent
        source: (generatedQML.loaded && appCore.generateQMLPath!=="")? "file:/"+appCore.generateQMLPath : ""

        onSourceChanged: {
            console.log("Begin load", source)
        }

        onLoaded: {
            console.log("LOADED")
        }


    }
}
