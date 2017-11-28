#ifndef RESTFULLSERVICE_H
#define RESTFULLSERVICE_H

#include <RestfullClient.h>
#include <fbutils.h>
/**
 * @brief The RestfullClient class : This is NOT a thread safe class.
 */

class RestfullService {

public:

    static long getBtcAddressBalance(const std::string &addr,
                 QThread * ownerThread = QThread::currentThread()) {

        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        QString customRoute("q/addressbalance/%1");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute.arg(addr.data()),params,headers);

        return client.lastReply().toLong ();
        //toStdString();
    }


    static QByteArray getBtcAddressUnspent(const std::string &addr,
                 QThread * ownerThread = QThread::currentThread()) {
        return getBlockchainBtcAddressUnspent(addr,ownerThread);
    }

    static QByteArray getBlockchainBtcAddressUnspent(const std::string &addr,
                 QThread * ownerThread = QThread::currentThread()) {

        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        params.insert ( QString("active"),QString(addr.data()));
        QString customRoute("unspent");
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }

    static QByteArray getChainsoBtcAddressUnspent(const std::string &addr,
                 QThread * ownerThread = QThread::currentThread()) {

        QString url = QString(fantasybit::CHAINSOAPI.data()).arg("get_tx_unspent");
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        QString customRoute = addr.data();
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }

    static QByteArray getChainsoBtcAddress(const std::string &addr,
                 QThread * ownerThread = QThread::currentThread()) {
        //https://chain.so/api/v2/address/BTCTEST/mnBZiPHayJMSCc5Vj32Tn65tn1CcuCPWkv
        QString url = QString(fantasybit::CHAINSOAPI.data()).arg("address");
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        QString customRoute = addr.data();
        //customRoute = customRoute.arg(route).arg(blockNum);
        client.getData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }

    static QByteArray pushTxXXX(const std::string &rawTx,
                             QThread * ownerThread = QThread::currentThread()) {
        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);

        std::string sendit = "tx=" + rawTx;
        QString customRoute("pushtx");
//        client.postStringData(customRoute,"application/x-www-form-urlencoded",sendit.data(),RestfullClient::UTF8);
        QByteArray data(sendit.data(),sendit.size());
        client.postRawData(customRoute,"multipart/form-data;boundary=-------",data);
//        client.postRawData(customRoute,"",data);

        return client.lastReply();
        //toStdString();
    }

    static QByteArray pushChainsoBitcoinTx(const std::string &rawTx,
                             QThread * ownerThread = QThread::currentThread()) {
        QString url = QString(fantasybit::CHAINSOAPI.data()).arg("send_tx");
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        params.insert ( QString("tx_hex"),QString(rawTx.data()));
        QString customRoute("");
        client.postTData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }


    static QByteArray pushBitcoinTx(const std::string &rawTx,
                             QThread * ownerThread = QThread::currentThread()) {
        QString url = QString(fantasybit::BLOCKCHAINAPI.data());
        RestfullClient client(QUrl(url),ownerThread);
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        params.insert ( QString("tx"),QString(rawTx.data()));
        QString customRoute("pushtx");
        client.postTData(customRoute,params,headers);

        return client.lastReply();
        //toStdString();
    }


};

#endif // RESTFULLSERVICE_H
