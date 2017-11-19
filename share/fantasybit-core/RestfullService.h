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

};

#endif // RESTFULLSERVICE_H
