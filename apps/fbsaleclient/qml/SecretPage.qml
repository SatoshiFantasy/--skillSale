import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {

    id: secretpage

    TextEdit {
        id: cliphelper
        visible: false
    }

    property var secretWordsIn: {
        return CoinSale.secretShow.split(" ");
    }

    property bool isimport: false
    property bool isverify: false
    property bool isdisplay: !isimport && (!isverify || CoinSale.secretIsVerified)
    property var secretOut: []
    property bool didcopy: false

//    onIsverifyChanged: {
//        console.log("verify " + isverify)
////        grid.forceLayout()
//    }

//    onIsimportChanged: {
//        console.log("import " + isimport)

//        grid.update()
//    }

    onIsdisplayChanged: {
        console.log("onIsdisplayChanged " + isdisplay)

        if ( !isdisplay ) {
            secretWordsIn = ""
            grid.update()
        }
        else {
            if ( CoinSale.secretShow !== "" ) {
                secretWordsIn = CoinSale.secretShow.split(" ");
            }
        }
    }

    implicitHeight: rec1.height + button1.height
    implicitWidth: rec1.width
    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
//        anchors.top: parent.top

        Rectangle {
            id: rec1
            anchors.centerIn: parent
            width: 300
            height: 260

            GridView {
                id: grid
                anchors.fill: parent
                cellHeight: parent.height / 6
                cellWidth: parent.width / 2

                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: 12
                delegate: secretDelegate
            }

            MouseArea {
//                enabled:!(isimport || isverify )
                anchors.fill: parent
                onClicked: {
                    if ( isdisplay ) {
                        console.log("copying ")
                        cliphelper.text = CoinSale.secretShow
                        cliphelper.selectAll()
                        cliphelper.copy()
                        cliphelper.text = ""
                        rec1.ToolTip.visible = true
                        didcopy = true
                    }
                    else {
                        cliphelper.paste()
                        console.log("pste " + cliphelper.text)
                        secretWordsIn = cliphelper.text.trim().split(" ")
                        cliphelper.text = ""
                        rec1.ToolTip.visible = true
                        grid.update
                    }
                }

                acceptedButtons: !isdisplay ? Qt.RightButton : Qt.LeftButton
            }

            ToolTip.timeout: 5000
            ToolTip.text: isdisplay ? "Secret Copied to Clipboard" : "Secret Pasted from Clipboard"

        }

        RowLayout {
            anchors.top: rec1.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: button1
                text: isimport ? "Import" : ( isverify ? "Verify" : "OK")
                onClicked: {
                    if ( !isdisplay ) {
                        var words = secretOut.join(" ")  ;
                        console.log("clicked: " + words)
                        if ( words !== "" ) {
                            if ( isimport) {
                                CoinSale.doimport(words);
                            }
                            else {
                                CoinSale.verify(words);
                            }
                        }
                        else CoinSale.set_currStatus("bad import")
                    }
                    else {
                        if ( didcopy ) {
                            cliphelper.text = "secret cleared from clipboard"
                            cliphelper.selectAll()
                            cliphelper.copy()
                            cliphelper.text = ""
                        }

                        CoinSale.secretOk();
                    }
                }
//                visible: isimport
            }

            Button {
                id: forgotbut
                text: "Recover"
                enabled: isverify
                visible: isverify
                onClicked: {
                    CoinSale.forgot()
                }
            }
        }
    }


    Component {
        id: secretDelegate

        Item {
            width: grid.cellWidth
            height: grid.cellHeight

            Rectangle {
//            anchors.margins: 2
                height: parent.height * .80
                width: parent.width
                anchors.centerIn: parent
                Label {
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    height: parent.height
                    width: parent.width * .20
                    id: lb
                    text: (index < 10 ? " " : "") +(index+1) + ": "
                }
                TextField {
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: lb.right
                    width: parent.width - lb.width
                    text: secretWordsIn.length < index ? "" : secretWordsIn[index]//(secretpage.isimport || secretpage.isverify || secretWordsIn.length < 12) ? "" : secretWordsIn[index]
                    font.bold: true
                    readOnly: secretpage.isdisplay
//                    onEditingFinished: {
//                        console.log("editing finished" + text)
//                        secretOut[index] = text
//                    }

                    onTextChanged: {
                        console.log("text changed " + text)
                        secretOut[index] = text
                    }
                }
            }
        }
    }
}
