import QtQuick
import QtQuick.Dialogs

Item {
    id: dropArea
    property var fileExtensionFilters: ([]) //-- "Vector (*.svg)", "Text (*.txt)", ...
    property bool multiple: false
    property bool enableSelectDialog: true
    property bool enableSelectDialogDblClick: true
    property string helpText: "Drop files here"

    signal filesAccepted(var files);

    QtObject {
        id: _private
        property string fileExtensionFiltersStr: fileExtensionFilters.join(",")
    }

    DropArea {
        anchors.fill: parent
        onEntered: (drag) => {
            let acceptedCount = 0;
            for(let i=0; i<drag.urls.length; ++i) {
                if( validateFileExtension(drag.urls[i]) ) { acceptedCount++; }
            }
            drag.accepted = ( ((dropArea.multiple && drag.urls.length>0) || ( !dropArea.multiple && drag.urls.length===1)) && acceptedCount===drag.urls.length );
        }

        onDropped: (drag) => {
            dropArea.filesAccepted(drag.urls);
        }

        function validateFileExtension(filePath)
        {
            if ( !filePath ) { return false; }
            if ( dropArea.fileExtensionFilters.length===0 ) { return true; }
            return ( _private.fileExtensionFiltersStr.indexOf("(*."+((""+filePath).split('.').pop())+")")>-1 );
        }

        FileDialog {
            id: fileSelectDialog
            nameFilters: fileExtensionFilters
            fileMode: (dropArea.multiple)? FileDialog.OpenFiles : FileDialog.OpenFile
            title: qsTr("Select file")
            onAccepted: {
                dropArea.filesAccepted(fileSelectDialog.selectedFiles);
            }
        }

        Canvas{
            id: borderDashed
            anchors.fill: parent

            property color borderColor: (dropArea.containsDrag)? "#FF9200" : "#909090"
            onBorderColorChanged: { requestPaint(); }

            onPaint: {
                let ctx = getContext("2d");
                ctx.setLineDash([1, 3]);
                ctx.strokeStyle = borderColor;
                ctx.lineWidth = 4;
                ctx.lineCap = "round";

                ctx.beginPath();
                let lw2 = ctx.lineWidth/2.0;
                ctx.moveTo(0+lw2, 0+lw2);
                ctx.lineTo(borderDashed.width-lw2, 0+lw2);
                ctx.lineTo(borderDashed.width-lw2, borderDashed.height-lw2);
                ctx.lineTo(0+lw2, borderDashed.height-lw2);
                ctx.lineTo(0+lw2, 0+lw2);
                ctx.stroke();
            }
        }

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                if ( dropArea.enableSelectDialog && dropArea.enableSelectDialogDblClick ) {
                    fileSelectDialog.visible = true;
                }
            }
        }

        Column {
            id: helpWrapper
            visible: (dropArea.helpText!=="")
            anchors.centerIn: parent
            height: childrenRect.height

            Text {
                text: dropArea.helpText
                font.pixelSize: 20
                color: "#909090"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: qsTr("or")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 14
                color: "#dfdfdf"
            }

            Rectangle {
                id: btn
                width: btnTxt.contentWidth+20
                height: btnTxt.contentHeight+10
                visible: dropArea.enableSelectDialog
                anchors.horizontalCenter: parent.horizontalCenter

                property string text: "btn"
                signal clicked();

                radius: 20
                color: (btnMa.containsMouse)? "#dfdfdf" : "transparent"
                border.width: 2
                border.color: "#909090"
                MouseArea {
                    id: btnMa
                    anchors.fill: parent
                    hoverEnabled: true
                    Text {
                        id: btnTxt
                        anchors.centerIn: parent
                        text: qsTr("browse")
                        font.pixelSize: 14
                        color: "#909090"
                    }
                    onClicked: {
                       fileSelectDialog.visible = true;
                    }
                }
            }
        }
    }
}
