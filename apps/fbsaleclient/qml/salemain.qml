import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0

ApplicationWindow {
    id: window
    width: 500
    height: 700
    visible: true
    title: "Fantasybit"
    property  bool first: true

    property string  fontfamFB:  "Helvetica Neue"
    property string nameStatsText: ""
    property bool doimport: false
    property bool doverify: false


    property var pages:     [ "Account",  "Secret", "Bitcoin", "Balance"  ]
    property var altpages: [ "Fantasy Name",  "Brain Wallet", "Bitcoin Address", "Transactions and Balances"  ]


    property int  buyamount: 0
    Material.theme: Material.Light
    Material.accent: "#0080A6"// Material.Amber
    Material.primary: "#0E3656"//BlueGreyMaterial.Blue
//    Material.foreground: Material.BlueGrey// "#F3F3F3" //Material.BlueGrey// "#FAFAFA"
//    Material.background: "#F3F3F3"
//    Material.
//    Material.Blue


    header:  ToolBar {

//        Material.background: "blue"
        height: first  ? 0 : implicitHeight
        visible: !first
        Material.foreground: Material.background
        id: toobar
        implicitHeight: gl.implicitHeight

        RowLayout {
            spacing: 20
            anchors.fill: parent
            visible: !CoinSale.secretIsVerified

            Label {
                id: titleLabel
                text: (!tabBar || tabBar.currentIndex < 0) ? "" : pages[tabBar.currentIndex]
                font.pixelSize: 16
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
//                font.family: fontfamFB
                //                textFormat: Text.RichText
                //                text: "ƑantayɃit<sup>+Skill</sup> Sale"

            }
        }



        MouseArea {

            anchors.fill: parent
        GridLayout  {
            id: gl
            visible: CoinSale.secretIsVerified
            height: image12.height * 1.10
            anchors.fill: parent
            anchors.centerIn: parent
//            anchors.margins: 20
            clip: true
//                Layout.preferredHeight: 100
//                width: parent.width * .80
//                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            rows: 2
            columns:6
            Image {
                Layout.rowSpan: 2
                Layout.columnSpan: 2

                id: image12
                fillMode: Image.PreserveAspectFit
                source: "FantsyBitLogoSmall.png"
                Layout.column: 1
                Layout.row: 1

            }

            Row {
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                Layout.alignment: Qt.AlignBottom
                Layout.column: 3
                Layout.row: 1

                Label {
                    text: qsTr("price: ")
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
//                            Layout.columnSpan: 1
//                            Layout.rowSpan: 1
//                            Layout.alignment: Qt.AlignCenter
//                            Layout.column: 3
//                            Layout.row: 1

                }

                Label {
                    text: qsTr("0.00001 BTC")
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
//                            Layout.columnSpan: 1
//                            Layout.rowSpan: 1
//                            Layout.alignment: Qt.AlignCenter
//                            Layout.column: 4
//                            Layout.row: 1

                }
            }

            Row {
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                Layout.alignment: Qt.AlignTop
                Layout.column: 3
                Layout.row: 2

                Label {
                id: label222
                text: qsTr("available: ")
                font.pixelSize: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
//                        Layout.alignment: Qt.AlignCenter
//                        Layout.columnSpan: 1
//                        Layout.rowSpan: 1
//                        Layout.column: 3
//                        Layout.row: 2

            }

                Label {
                id: available2
                text: Number(Math.round(CoinSale.totalAvailable,10)).toLocaleString(Qt.locale("en-US"),'f',0)
//                                +  " ƑɃ"
                font.pixelSize: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: fontfamFB

//                        Layout.columnSpan: 1
//                        Layout.rowSpan: 1
//                        Layout.alignment: Qt.AlignHCenter
//                        Layout.column: 4
//                        Layout.row: 2


            }
            }

            SpinBox {
                focusPolicy: Qt.StrongFocus
                wheelEnabled: true
                id: bbox2
                stepSize: 1000
                from: 1000
                value: first ? bbox.value : box2.value
                property int decimals: 5
                property real realValue: value * .00001
                to: CoinSale.totalAvailable
                validator: DoubleValidator {
                    bottom: Math.min(bbox.from, bbox.to)
                    top:  Math.max(bbox.from, bbox.to)
                }
                textFromValue: function(value, locale) {
                    console.log( " from value" + value)
                    return Number(value * .00001 ).toLocaleString(Qt.locale("en-US"), 'f', bbox.decimals) + " BTC"
                }

                valueFromText: function(text, locale) {
                    console.log( " from text " + text)
                    var t = text.split(" ")[0]
                    var d = Number.fromLocaleString(Qt.locale("en-US"), t)
                    var dd = d * 100000
                    console.log(d)
                    console.log(d / 0.00001)
                    console.log(dd)
                    return Math.round(dd * 100000) / 100000
                }

                font.pixelSize: 10
//                        editable: true
                Layout.preferredHeight: 20
//                        Layout.preferredHeight: contentItem.implicitHeight
                Layout.preferredWidth: image12.implicitWidth * 2
//                        textFromValue: function(value, locale) {
//                            return Number(value).toLocaleString(locale, 'f', 0) + " ƑɃ";
//                        }

//                        Layout.fillWidth: true;
                editable: true
                onValueChanged: {
                    console.log("new value " + value)
//                            if ( focus ) {
//                                console.log("box new value " + value)

//                                box.value = value
//                            }

                }
                Layout.alignment: Qt.AlignCenter
                Layout.columnSpan: 1
                Layout.rowSpan: 2
                Layout.column: 6
                Layout.row: 1


        }

            SpinBox {
                focusPolicy: Qt.StrongFocus
                wheelEnabled: true
                focus: true
                id: box2
                stepSize: 1000
                from: 1000
                to: CoinSale.totalAvailable
                font.pixelSize: 12
                editable: true
                Layout.preferredHeight: 20
//                        Layout.preferredHeight: contentItem.implicitHeight
                Layout.preferredWidth: image12.implicitWidth * 2
                textFromValue: function(value, locale) {
                    return Number(value).toLocaleString(locale, 'f', 0) + " ƑɃ";
                }

                valueFromText: function(text, locale) {
                    console.log( "fb from text " + text)
                    var t = text.split(" ")[0]
                    var d = Number.fromLocaleString(Qt.locale("en-US"), t)
                    return d;
                }

                font.family: fontfamFB


               value: bbox2.value
               Layout.alignment: Qt.AlignCenter
                Layout.columnSpan: 1
                Layout.rowSpan: 2
                Layout.column: 5
                Layout.row: 1
            }
        }
        }

    }

    StackLayout  {
        id: stackView
        anchors.fill: parent

        currentIndex: tabBar.currentIndex+1
//        ToolTip {
//            id: tt
//            parent: stackView.currentItem
//        }


//        initialItem:
        Pane {
            id: pane

            Material.theme:  Material.Dark
            ColumnLayout {
//                anchors.centerIn: (parent.top - parent.center) / 2

                id: columnLayout2
                clip: false
                anchors.topMargin: 130
                spacing: 40
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width

//                Label {
//                    width: parent.width
//                    wrapMode: Label.Wrap
//                    horizontalAlignment: Qt.AlignHCenter
//                    text: "Fantasy name is your public id and Fantasybit wallet address, some use their twitter handle others remain anonymous"
//                    Component.onCompleted: {
//                        console.log(parent.width)
//                    }
//                }

                Label {
                    id: label1
                    text: qsTr("Welcome to Fantasybit Skill Coin Sale ")
//                    Layout.columnSpan: 4
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 17
                    font.bold: true
                    textFormat: Text.RichText
                    horizontalAlignment: Text.AlignHCenter
                    enabled: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }

//                BusyIndicator {
//                    anchors.centerIn: parent
//                    running: !button1.enabled
//                }

                GridLayout {
                    Layout.preferredHeight: 100
                    width: parent.width * .80
                    id: rowLayout3
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    rows: 2
                    columns:7
                    Image {
                        Layout.rowSpan: 2
                        Layout.columnSpan: 2

                        fillMode: Image.PreserveAspectFit
                        source: "FantsyBitLogoSmall.png"
                        Layout.column: 1
                        Layout.row: 1

                    }

                    Row {
                        Layout.columnSpan: 2
                        Layout.rowSpan: 1
                        Layout.alignment: Qt.AlignBottom
                        Layout.column: 3
                        Layout.row: 1

                        Label {
                            text: qsTr("price: ")
                            font.pixelSize: 12
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight
//                            Layout.columnSpan: 1
//                            Layout.rowSpan: 1
//                            Layout.alignment: Qt.AlignCenter
//                            Layout.column: 3
//                            Layout.row: 1

                        }

                        Label {
                            id: price
                            text: qsTr("0.00001 BTC")
                            font.pixelSize: 12
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
//                            Layout.columnSpan: 1
//                            Layout.rowSpan: 1
//                            Layout.alignment: Qt.AlignCenter
//                            Layout.column: 4
//                            Layout.row: 1

                        }
                    }

                    Row {
                        Layout.columnSpan: 2
                        Layout.rowSpan: 1
                        Layout.alignment: Qt.AlignTop
                        Layout.column: 3
                        Layout.row: 2

                        Label {
                        id: label2
                        text: qsTr("available: ")
                        font.pixelSize: 12
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
//                        Layout.alignment: Qt.AlignCenter
//                        Layout.columnSpan: 1
//                        Layout.rowSpan: 1
//                        Layout.column: 3
//                        Layout.row: 2

                    }

                        Label {
                        id: available
                        text: Number(Math.round(CoinSale.totalAvailable,10)).toLocaleString(Qt.locale("en-US"),'f',0)
//                                +  " ƑɃ"
                        font.pixelSize: 12
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: fontfamFB

//                        Layout.columnSpan: 1
//                        Layout.rowSpan: 1
//                        Layout.alignment: Qt.AlignHCenter
//                        Layout.column: 4
//                        Layout.row: 2


                    }
                    }

//                    Label {
//                        id: lblh
//                        text: "ƑɃ Qty"
//                        font.pixelSize: 10
//                        Layout.columnSpan: 1
//                        Layout.rowSpan: 1
//                        verticalAlignment: Text.AlignBottom
//                        Layout.alignment: Qt.AlignHCenter
//                        Layout.column: 5
//                        Layout.row: 1
//                        font.family: fontfamFB

//                    }

//                    Column {
//                        Layout.columnSpan: 1
//                        Layout.rowSpan: 2
//                        Layout.alignment: Qt.AlignCenter
//                        Layout.column: 5
//                        Layout.row: 1
//                        Layout.preferredHeight: 40



                    SpinBox {
                        focusPolicy: Qt.WheelFocus
                        wheelEnabled: true
                        hoverEnabled: true
                        focus: true
                        id: bbox
//                        value: 50
//                        width: itemWidth

//                        verticalAlignment: Text.AlignBottom
//                        Layout.alignment: Qt.AlignCenter
//                        Layout.preferredWidth: window.width / 4
                        stepSize: 1000
                        from: 1000
                        value: box.value
                        property int decimals: 5
                        property real realValue: value * .00001
                        to: CoinSale.totalAvailable
                        validator: DoubleValidator {
                            bottom: Math.min(bbox.from, bbox.to)
                            top:  Math.max(bbox.from, bbox.to)
                        }
                        textFromValue: function(value, locale) {
                            console.log( " from value" + value)
                            return Number(value * .00001 ).toLocaleString(Qt.locale("en-US"), 'f', bbox.decimals) + " BTC"
                        }

                        valueFromText: function(text, locale) {
                            console.log( " from text " + text)
                            var t = text.split(" ")[0]
                            var d = Number.fromLocaleString(Qt.locale("en-US"), t)
                            var dd = d * 100000
                            console.log(d)
                            console.log(d / 0.00001)
                            console.log(dd)
                            return Math.round(dd * 100000) / 100000
                        }

                        font.pixelSize: 10
//                        editable: true
                        Layout.preferredHeight: 20
//                        Layout.preferredHeight: contentItem.implicitHeight
                        Layout.preferredWidth: button1.implicitWidth * 2
//                        textFromValue: function(value, locale) {
//                            return Number(value).toLocaleString(locale, 'f', 0) + " ƑɃ";
//                        }

//                        Layout.fillWidth: true;
                        editable: true
                        onValueChanged: {
                            console.log("new value " + value)
//                            if ( focus ) {
//                                console.log("box new value " + value)

//                                box.value = value
//                            }

                        }
                        Layout.alignment: Qt.AlignBottom
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.column: 5
                        Layout.row: 1


                }

                    SpinBox {
                        focusPolicy: Qt.WheelFocus
                        wheelEnabled: true
                        hoverEnabled: true
                        id: box
                        stepSize: 1000
                        from: 1000
                        to: CoinSale.totalAvailable
                        font.pixelSize: 12
                        editable: true
                        Layout.preferredHeight: 20
//                        Layout.preferredHeight: contentItem.implicitHeight
                        Layout.preferredWidth: button1.implicitWidth * 2
                        textFromValue: function(value, locale) {
                            return Number(value).toLocaleString(locale, 'f', 0) + " ƑɃ";
                        }

                        valueFromText: function(text, locale) {
                            console.log( "fb from text " + text)
                            var t = text.split(" ")[0]
                            var d = Number.fromLocaleString(Qt.locale("en-US"), t)
                            return d;
                        }

                        font.family: fontfamFB


                       value: bbox.value
                        Layout.alignment: Qt.AlignTop
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.column: 5
                        Layout.row: 2


                    }
//                    }

                    Button {
                       id: button1
                       text: enabled ? "BUY" : "..."
                       enabled: CoinSale.totalAvailable > 0
                       onClicked: {
                           CoinSale.buy()
                       }

                       Layout.columnSpan: 1
                       Layout.rowSpan: 2
                       Layout.alignment: Qt.AlignCenter
                       Layout.column: 6
                       Layout.row: 1

                   }

//                    Label {
//                        id: lblh2
//                        text: "BTC Cost"
//                        font.pixelSize: 10
//                        Layout.columnSpan: 1
//                        Layout.rowSpan: 1
//                        verticalAlignment: Text.AlignBottom
//                        Layout.alignment: Qt.AlignHCenter
//                        Layout.column: 7
//                        Layout.row: 1

//                    }


            }
            }
        }

        Account {}
        Secret {}
        Bitcoin {}
        Balance {}
    }


    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        running: CoinSale.busySend
    }

    Label {
       Material.foreground: Material.Red

        rotation: -45
        text: "Demo Testing"
        anchors.centerIn: parent
        font.pixelSize: 20
        font.bold:  true
        visible: CoinSale.isTestNet
    }


    footer: Column {
        width: parent.width
        spacing: 5

        ProgressBar {
            Material.foreground: Material.Red
            id: pbar
            width: parent.width // (stackView.width / 3 ) * 2
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: !window.first
            visible: enabled
            value: 0
            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 18
                color: "#f9d7d7"//#e6e6e6"
                radius: 3
            }
            contentItem: Item {
                implicitWidth: 200
                implicitHeight: 12

                Rectangle {
                    width: pbar.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "#17a81a"
                }
            }
        }

        TabBar {
            property int prev: -1
            id: tabBar
            visible: !window.first
            enabled: !window.first
            Layout.preferredHeight:  window.first ? 0 : implicitHeight
            width: parent.width
            Layout.fillWidth: true
            currentIndex: -1
            Repeater {
                model: window.first ? 0 : pages.length;
                TabButton {
                    text: pages[index]
                    font.pixelSize: 10
                }
            }
        }
        ToolBar {
            Material.foreground: Material.Grey
            width: parent.width
            Layout.fillWidth: true;
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
    //            anchors.fill: parent
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                Layout.fillWidth: true;

    //            width: parent.width
                spacing: 5
                padding: 5

                Label {
        //            anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    id: logarea2
                    text: "..."
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 10

                    Layout.fillWidth: true;

        //            anchors.bottom: logarea.top
                }
                Label {
                    anchors.verticalCenter: parent.verticalCenter

        //            anchors.horizontalCenter: parent.horizontalCenter
                    id: logarea
                    text: CoinSale.currStatus//"..."
                    font.pixelSize: 10
                    Layout.fillWidth: true;

                }

            }

        }
    }

    Connections {
        target: CoinSale
        property var pagemap: []

        function startupFunction() {
            for (var i = 0; i < pages.length; i++) {
                    pagemap[pages[i]] = i;
            }
        }

        Component.onCompleted: startupFunction();

        onCurrDialogChanged: {
            if ( window.first )
                window.first = false;

           var nexti = -1;
           switch  ( currDialog ) {
               case "name":
                   nexti = pagemap["Account"];
                   break;
               case "fund":
                   nexti = pagemap["Bitcoin"];
                   if ( CoinSale.secretIsVerified )
                       pbar.value = .50;
                   break;
               case "secret":
                   doverify = false;
                   doimport = false;
                   nexti = pagemap["Secret"];
                   pbar.value = .25;
                   break;
               case "secretverify":
                   doverify = true;
                   doimport = false;
                   nexti = pagemap["Secret"];
                   break;
               case "secretimport":
                   doimport = true;
                   doverify = false;
                   nexti = pagemap["Secret"];
                   break;

               case "balance":
                   nexti = pagemap["Balance"];
                   pbar.value = .75;
                   break;
               case "done":
                   nexti = pagemap["Balance"];
                   pbar.value = 1;
                   break;
               default:
                   console.log("unknown currDialog" + currDialog)
//                   nexti = pagemap["Balance"];
                   break;

           }

           if ( nexti > -1 ) {
               console.log(" nexti " + currDialog + nexti)
               tabBar.setCurrentIndex(nexti);
           }
        }

        onNameCheckGet: {
            console.log( "namehcek " + name + status)
            if(status === "true" ) {
                window.nameStatsText = "name Check Pass... sending claim name tx to blockchain for: " + name;
                toobar.ToolTip.show( "name Check Pass... sending claim name tx to blockchain for: " + name ,5000)
                CoinSale.signPlayer(name);
            }
            else if ( status === "false"){
                window.nameStatsText = name + " not Available - try new name or import";
//                stackView.ToolTip.show(window.nameStatsText ,10000)
                toobar.ToolTip.show(name + " not Available - try new name or import",5000)

            }
            else {
                var toshow = name + status
                window.nameStatsText = name + ": " + status
                toobar.ToolTip.show(toshow ,6000)
            }
        }

        onCurrStatusChanged: {
//            logarea3.text = logarea2.text
            logarea2.text = logarea.text
            logarea.text = "... " + currStatus
//            logarea.text = currStatus;
//            logarea.append(currStatus + "\n")
//            logarea.text = currStatus + "\n" + logarea.text;
        }
    }


}

