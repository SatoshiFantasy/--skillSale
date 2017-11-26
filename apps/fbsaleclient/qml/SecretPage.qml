import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {

    id: secretpage
    property var secretWordsIn: {
        return CoinSale.secretShow.split(" ");
    }

    property bool isimport: false
    property var secretOut: []

    implicitHeight: rec1.height + button1.height
    implicitWidth: rec1.width
    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
//        anchors.top: parent.top

        Rectangle {
            id: rec1
            anchors.centerIn: parent
            width: 300
            height: 260

            GridView {
                id: grid
                anchors.fill: parent
                cellHeight: parent.height / 6
                cellWidth: parent.width / 2

                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: 12
                delegate: secretDelegate
            }
        }

        Button {
            id: button1
            anchors.top: rec1.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Import")
            onClicked: {
                var words = secretOut.join(" ")  ;
                console.log(words)
                if ( words !== "" )
                    CoinSale.doimport(words);
            }
            visible: isimport
        }
    }
    Component {
        id: secretDelegate

        Item {
            width: grid.cellWidth
            height: grid.cellHeight

            Rectangle {
//            anchors.margins: 2
                height: parent.height * .80
                width: parent.width
                anchors.centerIn: parent
                Label {
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    height: parent.height
                    width: parent.width * .20
                    id: lb
                    text: (index < 10 ? " " : "") +(index+1) + ": "
                }
                TextField {
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: lb.right
                    width: parent.width - lb.width
                    text: (secretpage.isimport || secretWordsIn.length < 12) ? "" : secretWordsIn[index]
                    font.bold: true
                    readOnly: !secretpage.isimport
                    onEditingFinished: {
                        console.log("editing finished" + text)
                        secretOut[index] = text
                    }
                }
            }
        }
    }
}
