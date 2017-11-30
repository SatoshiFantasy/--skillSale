import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {

    id: secretpage

    TextEdit {
        id: cliphelper
        visible: false
    }

    TextEdit {
        id: pasthelper
        visible: false
    }


    property var secretWordsIn: {
        return !isdisplay ? [] : CoinSale.secretShow.split(" ");
    }

    property bool isimport: window.doimport
    property bool isverify: window.doverify
    property bool isdisplay: !isimport && (!isverify || CoinSale.secretIsVerified)
    property var secretOut: []
    property bool didcopy: false

    property bool didpaste: false

//    onIsverifyChanged: {
//        console.log("verify " + isverify)
////        grid.forceLayout()
//    }

//    onIsimportChanged: {
//        console.log("import " + isimport)

//        grid.update()
//    }
/*
    Component.onCompleted:  {
        console.log("secret onCompleted " + isdisplay)

        if ( !isdisplay ) {
            secretWordsIn = []
            grid.update()
        }
        else {
            if ( CoinSale.secretShow !== "" ) {
                secretWordsIn = CoinSale.secretShow.split(" ");
            }
        }
    }
*/
    onIsdisplayChanged: {
        console.log("onIsdisplayChanged " + isdisplay)

        grid.update()

//        if ( !isdisplay ) {
//            secretWordsIn = []
//            grid.update()
//        }
//        else {
//            if ( CoinSale.secretShow !== "" ) {
//                secretWordsIn = CoinSale.secretShow.split(" ");
//            }
//        }
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

            ToolTip {
                id: tt
                parent: rec1
            }

            MouseArea {
//                enabled:!(isimport || isverify )
                id: ma
                anchors.fill: parent
                onClicked: {
                    if ( isdisplay ) {
                        console.log("copying ")
                        cliphelper.text = CoinSale.secretShow
                        console.log("copying 2")

                        cliphelper.selectAll()
                        cliphelper.cut()
                        console.log("copying3 ")

//                        cliphelper.
//                        cliphelper.text = ""
//                        rec1.ToolTip.visible = true
                        console.log("copying4 ")
                        rec1.ToolTip.show("Secret Copied to Clipboard" ,5000)
//                        tt.visible = true;

                        didcopy = true
                    }
                    else {
                        cliphelper.text = ""
                        cliphelper.paste()
                        console.log("pste " + cliphelper.text)
                        secretWordsIn = Qt.binding(function() {
                            return cliphelper.text.trim().split(" ");
                        });

                        console.log("PASTING3 ")


//                        cliphelper.text = ""
//                        rec1.ToolTip.visible = true
//                        ToolTip.timeout = 5000
//                        ToolTip.text =  "Secret Pasted from Clipboard"
//                        ToolTip.visible = true
//                        didpaste = true
//                        grid.update
                          rec1.ToolTip.show("Secret Pasted from Clipboard" ,5000)
//                        tt.visible = true;
                        console.log("PASTING4 ")
//                        ToolTip.show("")
                    }
                }

                acceptedButtons: !isdisplay ? Qt.RightButton : Qt.LeftButton

//                ToolTip.timeout: 5000
//                ToolTip.text: isdisplay ? "Secret Copied to Clipboard" : "Secret Pasted from Clipboard"
//                ToolTip.visible: pressed

            }


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
                    text: (secretWordsIn.length <= index) ? "" : secretWordsIn[index]//(secretpage.isimport || secretpage.isverify || secretWordsIn.length < 12) ? "" : secretWordsIn[index]
                    font.bold: true
                    readOnly: secretpage.isdisplay
//                    onEditingFinished: {
//                        console.log("editing finished" + text)
//                        secretOut[index] = text
//                    }

                    onTextChanged: {
                        console.log(index + "text changed " + text)
                        secretOut[index] = text
                    }
                }
            }
        }
    }
}
