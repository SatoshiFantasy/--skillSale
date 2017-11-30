import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

Item {
    id: buybut
    RowLayout {
        id: rowLayout3
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        Image {
            id: image1
            fillMode: Image.PreserveAspectFit
            source: "FantsyBitLogoSmall.png"
        }

        ColumnLayout {
            id: columnLayout1
            width: 100
            height: 100
            Layout.rowSpan: 2
            Layout.columnSpan: 3

            RowLayout {
                id: rowLayout1
                width: 100
                height: 100
                Layout.columnSpan: 2

                Label {
                    id: price
                    text: qsTr(".00001")
                    font.bold: true
                    font.pointSize: 14
                }

                Label {
                    id: btc
                    text: qsTr("BTC")
                    font.bold: true
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                }
            }

            RowLayout {
                id: rowLayout2
                width: 100
                height: 100

                Label {
                    id: available
                    text: Number(CoinSale.totalAvailable).toLocaleString(Qt.locale("en-US"),'f',0)
                    font.pointSize: 12
                    verticalAlignment: Text.AlignVCenter
                }

                Label {
                    id: label2
                    text: qsTr("available")
                    font.pointSize: 12
                    Layout.columnSpan: 1
                    Layout.rowSpan: 1
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

         Button {
            id: button1
            text: enabled ? "BUY" : "..."
            enabled: CoinSale.totalAvailable > 0
            onClicked: {
                CoinSale.buy()
            }
        }
    }
}
