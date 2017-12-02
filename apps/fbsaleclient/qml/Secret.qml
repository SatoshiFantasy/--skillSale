import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

Pane {

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

    property bool isimport: window.doimport && ! CoinSale.secretIsVerified
    property bool isverify: window.doverify && !CoinSale.secretIsVerified
    property bool isdisplay: !isimport && !isverify
    property var secretOut: []
    property bool didcopy: false

    property bool didpaste: false

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

    readonly property int itemWidth:  (secretpage.availableWidth / 3) * 2


    Column {
        anchors.fill: parent
        spacing: 40
//        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "A tool tip is a short piece of text that informs the user of a control's function."
        }

//    ColumnLayout {
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.topMargin: 5
////        anchors.top: parent.top

        Frame {
            anchors.horizontalCenter: parent.horizontalCenter
            id: rec1
//            anchors.centerIn: parent
//            width: 300
//            height: 260

            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5
                width: itemWidth

                GridView {
                    width: parent.width
                    implicitHeight: 200
                    id: grid
    //                anchors.fill: parent
                    cellHeight: 200 / 6
                    cellWidth: itemWidth / 2

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: 12
                    delegate: secretDelegate
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
                                pasthelper.text = ""
                                pasthelper.paste()
                                console.log("pste " + pasthelper.text)
                                secretWordsIn = Qt.binding(function() {
                                    return pasthelper.text.trim().split(" ");
                                });

                                console.log("PASTING3 ")


        //                        cliphelper.text = ""
        //                        rec1.ToolTip.visible = true
        //                        ToolTip.timeout = 5000
        //                        ToolTip.text =  "Secret Pasted from Clipboard"
        //                        ToolTip.visible = true
        //                        didpaste = true
        //                        grid.update
//                                  ma.ToolTip.show("Secret Pasted from Clipboard" ,5000)
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

//                ToolTip {
//                    id: tt
//                    parent: rec1
//                }

//                ToolTip {
//                    id: tt2
//                    parent: ma
//                }


            }
        }

        RowLayout {
//            anchors.top: rec1.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: button1
//                enabled: CoinSale.currName === "" ||  isverify
//                visible: enabled
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

            RowLayout {
//            anchors.margins: 2
                height: parent.height
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter

//                anchors.centerIn: parent
                Label {
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -2
//                    anchors.left: parent.left
//                    height: parent.height
                    Layout.preferredWidth: parent.width * .20
                    id: lb
                    text: (index < 9 ? " " : "") +(index+1) + ": "
                    font.pointSize: 16

                }
                TextField {
                    height: parent.height
//                    anchors.bottom:
                    anchors.verticalCenter: parent.verticalCenter
//                    horizontalAlignment: Text.AlightLeft
                    verticalAlignment: Text.AlignBottom
                    font.pointSize: 16
//                    verticalAlignment: Qt.AlignBottom

//                    height: parent.height
//                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.left: lb.right
//                    width: parent.width - lb.width
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
