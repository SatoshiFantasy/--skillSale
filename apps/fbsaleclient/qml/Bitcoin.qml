import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
//import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

Pane {
        id: bitcoinpage

        readonly property int pwidth:  (bitcoinpage.availableWidth / 3) * 2

        TextEdit {
            id: cliphelper
            visible: false
        }

        Column {
            anchors.fill: parent
            spacing: 40

            Label {
                width: parent.width
                wrapMode: Label.Wrap
                horizontalAlignment: Qt.AlignHCenter

//                id: lbl
//                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.top: parent.top
//                anchors.topMargin: 10
//                width: (parent.width / 3) *2
//                wrapMode: Text.WordWrap
//        //            maximumLineCount: 20
//                horizontalAlignment: Qt.AlignHCenter
                text: "FUNDING ADDRESS "
            }

            Frame {
                anchors.horizontalCenter: parent.horizontalCenter
                id: fr1


                Item {
                    implicitHeight: 231//qrc.width
                    implicitWidth: 231//qrc.height

                    width: 231
                    height: 231
                    anchors.horizontalCenter: parent.horizontalCenter

//                    implicitWidth: width

//                    implicitHeight: 320
//                    width: pwidth;
//                    implicitWidth: width

                    QRCode {
                        id: qrc
                        anchors.centerIn: parent
                        anchors.fill: parent
//                        width: 231;
//                        height: 231
//                        implicitHeight: 231
//                        implicitWidth: 231


//                        anchors.fill: parent
                        anchors.horizontalCenter: parent.horizontalCenter
//                        anchors.verticalCenter: parent.verticalCenter

    //                anchors.centerIn: parent
    //                anchors.fill: parent
    //                anchors.top : lbl.bottom
    //                anchors.topMargin : 30
    //                anchors.horizontalCenter : parent.horizontalCenter
    //                width : 320
    //                height : 320
                    value : inputField.text
                    level : "H"
                    visible: CoinSale.secretIsVerified
                }
//                    Button {
    //                anchors.centerIn: parent

    //                anchors.top : lbl.bottom
    //                anchors.topMargin : 30
    //                anchors.horizontalCenter : parent.horizontalCenter
    //                width : 320
    //                height : 320

                    /**/
                    Button {
                        anchors.centerIn: parent
                        anchors.fill: parent
//                        width: 231;
//                        height: 231;
                        text: inputField.text
                        visible: !CoinSale.secretIsVerified
                        enabled: !CoinSale.secretIsVerified
                        onClicked: {
                            if ( !CoinSale.secretIsVerified )
                                CoinSale.showAddress();
                            else
                                console.log(": clicked address: " + inputField.text )
                        }
                    }
                /**/
                }
          }
            Label {
                id : inputField
    //            anchors.bottom : parent.bottom
    //            anchors.bottomMargin : 30
                anchors.horizontalCenter : parent.horizontalCenter
                text : CoinSale.secretIsVerified ? CoinSale.bitcoinAddress : "verify secret to reveal funding bitcoin address"
            }

            ProgressBar {
                width: parent.width
                indeterminate: true
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: CoinSale.checkFunds
                visible: enabled
            }

        }

        MouseArea {
            enabled: CoinSale.secretIsVerified
            anchors.fill: parent
            onClicked: {
                console.log("clicked MouseArea" + inputField.text );
                cliphelper.text = inputField.text
                cliphelper.selectAll()
                cliphelper.copy()
                inputField.ToolTip.show("Funding Address Copied" ,5000)
            }
        }
}
