import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

Pane {
        id: bitcoinpaNE
        Label {
            id: lbl
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
            width: (parent.width / 3) *2
            wrapMode: Text.WordWrap
    //            maximumLineCount: 20
            horizontalAlignment: Qt.AlignHCenter
            text: "FUNDING ADDRESS "
            Component.onCompleted: {
                console.log(parent.width)
            }
        }

        QRCode {
            anchors.top : lbl.bottom
            anchors.topMargin : 30
            anchors.horizontalCenter : parent.horizontalCenter
            width : 320
            height : 320
            value : inputField.text
            level : "H"
            visible: CoinSale.secretIsVerified
        }
        Button {
            anchors.top : lbl.bottom
            anchors.topMargin : 30
            anchors.horizontalCenter : parent.horizontalCenter
            width : 320
            height : 320
            text: inputField.text
            visible: !CoinSale.secretIsVerified
            enabled: !CoinSale.secretIsVerified
            onClicked: {
                CoinSale.showAddress();
            }
        }

        Label {
            id : inputField
            anchors.bottom : parent.bottom
            anchors.bottomMargin : 30
            anchors.horizontalCenter : parent.horizontalCenter
            text : CoinSale.secretIsVerified ? CoinSale.bitcoinAddress : "verify secret to reveal funding bitcoin address"
        }
}
