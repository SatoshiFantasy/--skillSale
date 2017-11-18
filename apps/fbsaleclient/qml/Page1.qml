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
    property alias textField1: textField1
    property alias button1: button1

    RowLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        anchors.top: parent.top

        TextField {
            id: textField1
            placeholderText: qsTr("Enter 12 word Secret")
        }

        Button {
            id: button1
            text: qsTr("Import")
            onClicked: {
                if ( textField1.text !== '')
                    CoinSale.doimport(textField1.text)
            }
        }
    }
}
