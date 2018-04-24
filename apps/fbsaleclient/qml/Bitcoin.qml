import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
//import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

Pane {
        id: bitcoinpage

        readonly property int pwidth:  (bitcoinpage.availableWidth / 8) * 7

        TextEdit {
            id: cliphelper
            visible: false
        }

        Column {
            anchors.fill: parent
            spacing: 15
            anchors.horizontalCenter : parent.horizontalCenter

            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 6
                width: pwidth

                Label {
                    width: pwidth
                    wrapMode: Label.Wrap
                    horizontalAlignment: Qt.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    text:   CoinSale.secretIsVerified ?
                                   ("Fund " + CoinSale.currName + "- Send payment to Bitcoin Address below.") :
                                    "Final step before funding!" ;
                }

                Label {
                    width: pwidth
                    wrapMode: Label.Wrap
//                    horizontalAlignment: Qt.Al
                    anchors.horizontalCenter: parent.horizontalCenter

                    text:   CoinSale.secretIsVerified ?
                                   "\nYou may send any BTC amount, in any number of transactions, from any wallet, even third parties or exchanges. If you close this program, you will need to import your \"brain wallet\", on restart."
                              :
                                    "\nAn In-memory Bitcoin wallet has been generated for: " + CoinSale.currName + " , your Fantasy Name. " + "Your custom Bitcoin funding address will be displayed below once you verify the ability to recover with your 12 word secret - \"brain wallet\"";

                    font.pixelSize: 12

                }

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
                    value: CoinSale.bitcoinAddress
                    level: "H"
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
//                        Material.theme:  Material.Dark
                        highlighted: true
//                        width: 231;
//                        height: 231;
                        text: "Click Here to \n\n Verify Secret and \n\n Display Address"
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

            Column {
                anchors.horizontalCenter : parent.horizontalCenter
                spacing: 4
                Label {
                    anchors.horizontalCenter : parent.horizontalCenter
                    text : CoinSale.secretIsVerified ? "Waiting for payment..." : ""
                    font.pixelSize: 12
                }

                Label {
                    id : inputField
        //            anchors.bottom : parent.bottom
        //            anchors.bottomMargin : 30
    //                wrapMode: Label.Wrap
                    anchors.horizontalCenter : parent.horizontalCenter
    //                text : CoinSale.secretIsVerified ? ("Waiting for payment...\n\n" + CoinSale.bitcoinAddress) : "verify secret to reveal funding bitcoin address"
                    text : CoinSale.secretIsVerified ?  CoinSale.bitcoinAddress : "verify secret to reveal funding bitcoin address"
                    font.bold: true
                }
            }

            ProgressBar {
                width: bitcoinpage.width
                indeterminate: true
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: CoinSale.checkFunds
                visible: enabled
            }

            Label {
                anchors.horizontalCenter : parent.horizontalCenter
                wrapMode: Label.Wrap
                width: pwidth

                text: "Warning: bitcoin private key is  not stored on disk. If you forget your 12 word secret from Step 2, your coins are at risk!"
                font.pixelSize: 12
                Material.foreground: Material.Red
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
