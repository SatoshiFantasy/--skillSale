import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

Pane {
    id: accountpane
//    property alias textField1: textField1
//    property alias button1: button1
    property alias nameStatsText: nameStatusL.text

    Label {
        id: lbl
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        width: (parent.width / 3) *2
        wrapMode: Text.WordWrap
//            maximumLineCount: 20
        horizontalAlignment: Qt.AlignHCenter
        text: "Fantasy name is your public id and Fantasybit coin address"
        Component.onCompleted: {
            console.log(parent.width)
        }
    }

    Column {
        spacing: 2
//        anchors.fill: parent
        id: col1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 40
        anchors.top: lbl.bottom

        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter

            TextField {
                id: claimNametext
                placeholderText: qsTr("Enter Fantasy Name")
            }

            Button {
                id: claimButton
                text: qsTr("Claim")
                onClicked: {
                    nameCheckBlank(claimNametext.text);
                }
            }
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            id: nameStatusL
            text: window.nameStatsText
        }
    }

//        RowLayout {
     Text {
//                id: textField1
                anchors.topMargin: 40
                anchors.top: col1.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                textFormat: Text.RichText
//                placeholderText: qsTr("Enter 12 word Secret")
                text: "Returning users - <a href=\"import\">Import name using secret</a>"
                font.pointSize: 10
                onLinkActivated: {
                    CoinSale.currDialog = "secretimport";
                }
        }
//            }

//            Button {
//                height: 50
//                id: button1
//                text: "Import name using secrert"
//                onClicked: {
//                    insec.visible = true;
////                    if ( textField1.text !== '')
////                        CoinSale.doimport(textField1.text)
//                }
//            }
//        }

//        Rectangle {
//            Layout.alignment: Qt.AlignCenter
//            border.color: "black"
//            width: parent.width
//            height: insec.implicitHeight
//            SecretPage {
//                anchors.fill: parent
//                id: insec
//                isimport: true
//                visible: false
//            }
//        }
//    }

    function nameCheckBlank(s) {
        if ( s.trim().length === 0 ) {
            window.nameStatsText = "please enter a fantasy name "
        }
        else if ( s.length > 45) {
            window.nameStatsText = "name to long "
        }
        else {
            window.nameStatsText = s + " checking name available.";
            CoinSale.checkname(s)
        }
    }

}
