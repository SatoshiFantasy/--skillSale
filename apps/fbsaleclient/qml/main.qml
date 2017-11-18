import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Testjay {}

        Page1 {
        }

        Page3 {

        }


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
            text: qsTr("Fund")
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
                   tabBar.currentIndex = 2
                   break;
               default:
                   tabBar.currentIndex = 0
                   break;

           }
        }
    }
}
