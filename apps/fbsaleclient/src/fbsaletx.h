#ifndef FBSALETX_H
#define FBSALETX_H

#include <QObject>


//#include <QObject>
//#include <QString>
//#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
//#include "../QmlModels/QQmlObjectListModel.h"
//#include "../QmlSuperMacros/QQmlHelpersCommon.h"
//#include "NameData.pb.h"
//#include <QQmlHelpersCommon.h>
//#include "FantasyName.h"
//#include "Commissioner.h"

#include <QQmlConstRefPropertyHelpers.h>
#include <QQmlObjectListModel.h>


namespace fantasybit {

class FBSaleTXItem : public QObject {
    Q_OBJECT
    QML_WRITABLE_CSTREF_PROPERTY (QString, txid)
    QML_WRITABLE_CSTREF_PROPERTY (quint64, btc)
    QML_WRITABLE_CSTREF_PROPERTY (quint64, fb)
    QML_WRITABLE_CSTREF_PROPERTY (bool, btcconfirmed)
    QML_WRITABLE_CSTREF_PROPERTY (bool, serverconfirmed)
public:
    FBSaleTXItem() :  QObject(nullptr) {
        m_txid = "";
        m_btc = 0;
        m_fb = 0;
        m_btcconfirmed = false;
        m_serverconfirmed = false;
    }
};


class FBSaleTXModel : public QQmlObjectListModel<FBSaleTXItem> {
    Q_OBJECT
public:
    explicit FBSaleTXModel (QObject *          parent      = Q_NULLPTR,
                                    const QByteArray & displayRole = QByteArray (),
                                    const QByteArray & uidRole     = QByteArray ()) :
                                    QQmlObjectListModel<FBSaleTXItem>
                                                    (parent,displayRole,uidRole)
    {}

    QML_READONLY_CSTREF_PROPERTY(quint64, totalbtc)
    QML_READONLY_CSTREF_PROPERTY(quint64, totalfb)

};

}

using namespace fantasybit;
Q_DECLARE_METATYPE(FBSaleTXItem*)
Q_DECLARE_METATYPE(FBSaleTXModel*)

#endif // FBSALETX_H
