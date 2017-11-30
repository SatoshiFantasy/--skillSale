//import QtQuick 2.2
//import QtQuick.Controls 2.1
//import QtQuick.Layouts 1.3
import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import QtQuick.Extras 1.4
import QtQuick.Extras.Private 1.0


ApplicationWindow {
    visible: true
//    width: 640
//    height: 480
    title: "Fantasybit Skill Sale App"

    property alias wantverify: secretpage.isverify
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Testjay {}

        Page1 {
            id: page1
        }

        SecretPage {
            id: secretpage
            isverify: false

        }

        Page3 {

        }

        SoldTx {

        }

    }

    Label {
        rotation: -45
        text: "Demo Testing"
        color: "#40000000"
        anchors.centerIn: parent
        font.pointSize: 20
        font.bold:  true
        visible: CoinSale.isTestNet
//        font.pixelSize: ProtoScreen.font( ProtoScreen.XXLARGE)
//        anchors.fill: parent
    }

//    Flickable{
//        id: f1
//        ScrollBar.vertical: ScrollBar { }
//        height: parent.height * .15
//        width: parent.width * .60
//        contentHeight: logarea.contentHeight
//        TextArea {
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.fill: parent
//            readOnly: true
//            id: logarea
//            text: "..."
//        }
//        anchors.bottom: parent.bottom
//        anchors.horizontalCenter: parent.horizontalCenter
//    }

    Label {
        anchors.horizontalCenter: parent.horizontalCenter
        id: logarea2
        text: "..."
        anchors.bottom: logarea.top
    }
    Label {
        anchors.horizontalCenter: parent.horizontalCenter
        id: logarea
        text: "..."
        anchors.bottom: parent.bottom
    }

    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        running: CoinSale.busySend
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Wallet")
        }
        TabButton {
            text: qsTr("WalletS")
        }
        TabButton {
            text: qsTr("Fund")
        }
        TabButton {
            text: qsTr("Balance")
        }

    }

    Connections {
        target: CoinSale


        onCurrDialogChanged: {
           switch  ( currDialog ) {
               case "name":
                   tabBar.currentIndex = 1
                   break;
               case "fund":
                   tabBar.currentIndex = 3
                   break;
               case "secret":
                   wantverify = false;
                   tabBar.currentIndex = 2
                   break;
               case "secretverify":
                   wantverify = true;
                   tabBar.currentIndex = 2
                   break;
               case "balance":
                   tabBar.currentIndex = 0
                   break;
               default:
                   tabBar.currentIndex = 0
                   break;

           }
        }

        onNameCheckGet: {
            console.log( "namehcek " + name + status)
            if(status === "true" ) {
                page1.nameStatsText = "name Check Pass... sending claim name tx to blockchain for: " + name;
                CoinSale.signPlayer(name);
            }
            else {
                page1.nameStatsText = name + " not Available - try new name or import"
            }
        }

        onCurrStatusChanged: {
            logarea.text = logarea2.text
            logarea2.text = currStatus
//            logarea.text = currStatus;
//            logarea.append(currStatus + "\n")
//            logarea.text = currStatus + "\n" + logarea.text;
        }
    }
}
