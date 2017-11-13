import QtQuick 2.4
import QtQuick.Controls.Material 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: item1
    property alias image1: image1
    property alias price: price





    ColumnLayout {
        id: columnLayout2
        clip: false
        anchors.topMargin: 5
        spacing: 8
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            id: label1
            text: qsTr("Welcome to Fantasybit Skill Coin Sale ")
            Layout.columnSpan: 4
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 17
            font.bold: true
            textFormat: Text.RichText
            horizontalAlignment: Text.AlignHCenter
            enabled: true
        }

        RowLayout {
            id: rowLayout3
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Image {
                id: image1
                fillMode: Image.PreserveAspectFit
                source: "images/FantsyBitLogoSmall.png"
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
                        text: qsTr(".00001177")
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
                        text: qsTr("2,500,000")
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
                text: qsTr("BUY")
            }
        }
    }





}
