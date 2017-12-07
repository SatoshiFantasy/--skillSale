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
//    property alias tooltip: textField1.ToolTip


    Column {
        id: lbl
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        width: (secretpage.width / 6) *5
        spacing: 10

        Label {
//            width: parent.width
            wrapMode: Label.Wrap
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Qt.AlignHCenter
            width: (secretpage.width / 6) *5

            text: CoinSale.currName !== "" ?
                      CoinSale.currName +
                                " is your Fantasy Name."  :
                      "Step 1: Select a Fantasy Name. " ;
        }

        Label {
            wrapMode: Label.Wrap
            anchors.horizontalCenter: parent.horizontalCenter
            width: (secretpage.width / 6) *5

            text: CoinSale.currName !== "" ?
                                "It is the user name assocaited to your Fantsybit wallet, and public key"
                    :
                      " This is your user name, and Fantsybit wallet address. " ;

            font.pixelSize: 12
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
                text: CoinSale.currName
                readOnly: CoinSale.currName !== ""
                Layout.fillWidth: true;
            }

            Button {
                id: claimButton
                text: qsTr("Claim")
                onClicked: {
                    nameCheckBlank(claimNametext.text);
                }
                enabled: CoinSale.currName === ""
            }
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            id: nameStatusL
            text: window.nameStatsText
            width: parent.width
            wrapMode: Text.WordWrap
            font.pixelSize: 10
            color: Material.Grey

        }
    }

//        RowLayout {
        Text {
                id: textField1
                anchors.topMargin: 10
                anchors.top: col1.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                textFormat: Text.RichText
//                placeholderText: qsTr("Enter 12 word Secret")
                text: "Returning users - <a href=\"import\">Import name using secret</a>"
                font.pixelSize: 10
                onLinkActivated: {
                    CoinSale.currDialog = "secretimport";
//                    claimButton.enabled = false
                }

                visible: CoinSale.currName === ""
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
