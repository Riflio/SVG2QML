import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs

Window {
    visible: true
    width: 640
    height: 480
    title: "SVG to QML converter. By pavelk.ru." + " V" + appCore.version;    
    //color: "#505050"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"

            WDropArea {
                anchors.fill: parent
                fileExtensionFilters: ["Vector (*.svg)"]
                helpText: ( generatedQmlLoader.status===Loader.Null )? qsTr("Drop SVG file to here") : "";

                onFilesAccepted: (files) => {
                    appCore.svg2qml(files[0], "file:./tmp.qml");
                }

                Item {
                    anchors.fill: parent
                    anchors.margins: 10
                    clip: true

                    Connections {
                        target: appCore
                        function onGenerateQMLPathChanged() {
                            generatedQmlLoader.source = appCore.generateQMLPath + "?t=" + Date.now(); //-- Force update when file name not changed
                        }
                    }

                    Loader {
                        id: generatedQmlLoader
                        anchors.fill: parent

                        onLoaded: {
                            console.log("Generated qml loaded")
                        }
                    }

                    FileDialog {
                        id: saveFileDialog
                        fileMode: FileDialog.SaveFile
                        onAccepted: {
                            appCore.saveAs(saveFileDialog.selectedFile);
                        }

                    }

                    Text {
                        text: qsTr("Saved as tmp.qml")
                        visible: ( generatedQmlLoader.status===Loader.Ready )
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        color: "#dfdfdf"
                    }
                }
            }
        }
    }
}
