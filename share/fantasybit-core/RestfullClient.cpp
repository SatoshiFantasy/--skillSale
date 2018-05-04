#include "RestfullClient.h"



RestfullClient::RestfullClient(const QUrl &argBaseUrl, QThread *ownerThread){
    //make sure we move the network access manager and the event loop
    // the the thread we're going to operate within.
    myNetworkManager.setParent(this);
    myEventLoop.setParent(this);
    if (ownerThread != QThread::currentThread())
        this->moveToThread(ownerThread);

    myBaseUrl = argBaseUrl;
    myCurrentNetworkReply = NULL;

    //init waiting signals connection
    //when the network manager is finished recieving a net reply read it all
    connect(&myNetworkManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedSlot(QNetworkReply *)));

    //when done reading quit the waiting loop if it's on exec state.
    connect(this, SIGNAL(doneReading()),&myEventLoop, SLOT(quit()));

    //when ssl errors, report them.0
    connect(&myNetworkManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(sslErrorsSlot(QNetworkReply*,QList<QSslError>)));

    //when disconnected exit a probable current waiting loop
    connect(&myNetworkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            this, SLOT(networkAccessibleChangedSlot(QNetworkAccessManager::NetworkAccessibility)));
}
