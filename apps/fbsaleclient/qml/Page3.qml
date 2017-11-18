//import QtQuick 2.7

//Page1Form {
//    button1.onClicked: {
//        console.log("Button Pressed. Entered text: " + textField1.text);
//    }
//}


import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
        anchors.top: parent.top
        QRCode {
            anchors.top : parent.top
            anchors.topMargin : 30
            anchors.horizontalCenter : parent.horizontalCenter
            width : 320
            height : 320
            value : inputField.text
            level : "H"
        }
        TextInput {
            id : inputField
            anchors.bottom : parent.bottom
            anchors.bottomMargin : 30
            anchors.horizontalCenter : parent.horizontalCenter
            text : CoinSale.bitcoinAddress
        }
}
