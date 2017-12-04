import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
//import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

Pane {
        id: bitcoinpage

        readonly property int pwidth:  (bitcoinpage.availableWidth / 5) * 4

        readonly property int itemHeight:  30

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.fill: parent
            spacing: 40

            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5
                width: parent.width

                Label {
                    width: parent.width
                    wrapMode: Label.Wrap
                    horizontalAlignment: Qt.AlignHCenter

                    textFormat: Text.RichText
                    text: "ƑɃ " + Number(CoinSale.pFBSaleTXModel.totalfb).toLocaleString(Qt.locale("en-US"),'f',0)

                    font.pixelSize: 24
                    font.family: fontfamFB

                }

                Label {
                    width: parent.width
                    wrapMode: Label.Wrap
                    horizontalAlignment: Qt.AlignHCenter

                    textFormat: Text.RichText
                    text:  "Spent BTC "  +  Number(CoinSale.pFBSaleTXModel.totalbtc).toLocaleString(Qt.locale("en-US"),'f',8)

                    font.pixelSize: 16
                }
            }

//                id: lbl
//                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.top: parent.top
//                anchors.topMargin: 10
//                width: (parent.width / 3) *2
//                wrapMode: Text.WordWrap
//        //            maximumLiner: 20
//                horizontalAlignment: Qt.AlignHCenter
//                text: "Transactions and Balance"
//            }

            ListView {
                anchors.horizontalCenter: parent.horizontalCenter

                width: pwidth

//                anchors.fill: parent
//                width: parent.width
                implicitHeight: itemHeight *  count * 2

//                anchors.fill: parent
                model: CoinSale.pFBSaleTXModel

                delegate: balDelegate
                spacing: 4
           }
        }

        Component {
            id: balDelegate

            Rectangle {
                id: content

                anchors { left: parent.left; right: parent.right }
                height: column.implicitHeight + 4

                border.width: 1
                border.color: "lightsteelblue"

                radius: 2

                Column {
                    id: column
                    anchors { fill: parent; margins: 2 }

                    Label {
                        textFormat: Text.RichText
                        text: "<a href=\"" + CoinSale.toUrl(model.txid) +  "\">" + "Txid: " + model.txid + "</a>"
                        onLinkActivated: Qt.openUrlExternally(link)
                        font.pixelSize: 10

                    }
                    Label {
                        textFormat: Text.RichText
                        text:  "Spent BTC "  +  Number(model.btc).toLocaleString(Qt.locale("en-US"),'f',8)
                        font.pixelSize: 12
                    }
                    Label {
                        textFormat: Text.RichText
                        text: "ƑɃ " + Number(model.fb).toLocaleString(Qt.locale("en-US"),'f',0)
                        font.pixelSize: 12
                        font.family: fontfamFB
                    }

                }
            }
        }

}
