import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {

    anchors.top: parent.top
    ListView {
        anchors.fill: parent
        model: CoinSale.pFBSaleTXModel

        delegate: Column {
            Label {
                text: model.txid
            }
            Label {
                text: "btc: " + model.btc
            }
            Label {
                text: "fb: " + model.fb
            }

        }
    }


}
