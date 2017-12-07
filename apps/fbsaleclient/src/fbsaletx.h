#ifndef FBSALETX_H
#define FBSALETX_H

#include <QObject>
#include <QQmlConstRefPropertyHelpers.h>
#include <QQmlObjectListModel.h>


namespace fantasybit {

class FBSaleTXItem : public QObject {
    Q_OBJECT
    QML_WRITABLE_CSTREF_PROPERTY (QString, txid)
    QML_WRITABLE_CSTREF_PROPERTY (qreal, btc)
    QML_WRITABLE_CSTREF_PROPERTY (quint64, fb)
    QML_WRITABLE_CSTREF_PROPERTY (bool, btcconfirmed)
    QML_WRITABLE_CSTREF_PROPERTY (bool, serverconfirmed)
public:
    FBSaleTXItem() :  QObject(nullptr) {
        m_txid = "";
        m_btc = 0.0;
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
                                                    (parent,displayRole,uidRole),
                                    m_totalbtc(0.0),
                                    m_totalfb(0)
    {}

    QML_READONLY_CSTREF_PROPERTY(qreal, totalbtc)
    QML_READONLY_CSTREF_PROPERTY(quint64, totalfb)

public:
    void DoAppend(FBSaleTXItem *in) {
        settotalbtc (m_totalbtc + in->get_btc ());
        settotalfb (m_totalfb + in->get_fb ());
        append(in);
    }

    bool txExists(const QString &txid) {
        return ( getByUid(txid) != nullptr );
    }

};

}

using namespace fantasybit;
Q_DECLARE_METATYPE(FBSaleTXItem*)
Q_DECLARE_METATYPE(FBSaleTXModel*)

#endif // FBSALETX_H
