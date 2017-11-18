//
// 11/17/2017  by: Jay Berg
// using StateMachine from SMC Compiler
//

#ifndef COINSALE_H
#define COINSALE_H

#include <QQmlConstRefPropertyHelpers.h>
//#include <QQmlPtrPropertyHelpers.h>
//#include <QQmlAutoPropertyHelpers.h>
//#include <QQmlEnumClassHelper.h>

#include <QObject>
#include <QWebSocket>

#include "FantasyAgent.h"

#include "StateData.pb.h"
#include <fbutils.h>

#include "CoinSale_sm.h"

namespace fantasybit {

class CoinSale : public QObject, public CoinSaleContext<CoinSale>
{
    Q_OBJECT

    QML_WRITABLE_CSTREF_PROPERTY(int,totalAvailable)

    QWebSocket m_webSocket;
    std::string lastPk2name;
    fantasybit::FantasyAgent agent;
    int errCount;
    QUrl theServer;
public:
    CoinSale() {
        connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
        connect (&m_webSocket,SIGNAL(aboutToClose()),this,SLOT(handleAboutToClose()));
        connect (&m_webSocket, SIGNAL(disconnected()), this, SLOT(handleClosed()));

        // socket error
        connect (&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                 SLOT(handleSocketError(QAbstractSocket::SocketError)));

        // socket statte
        connect (&m_webSocket, SIGNAL (stateChanged(QAbstractSocket::SocketState)),
                 this, SLOT(handleSocketState(QAbstractSocket::SocketState)));

        QString wss("ws://%1:%2");
        QString lserver = wss.arg(PB_WS_LITE_AGENT.data()).arg(FBSALE_AGENT_PORT);

    #ifndef NODEBUG
        qDebug() << " connecting to lserver" << lserver;
    #endif

        theServer = QUrl(lserver);
        m_webSocket.open(theServer);

//        signPlayerStatus.setInterval(2000);
//        connect(&signPlayerStatus, SIGNAL(timeout()),
//                this, SLOT(getSignedPlayerStatus()),Qt::QueuedConnection);


//        connect(Mediator::instance(),&Mediator::ready,this, &LightGateway::ClientReady,Qt::QueuedConnection);


    }

    bool HasName() {
        return false;
    }
    bool IsSecretVerified() {
        return false;
    }
    bool isEqualFundedAmount() {
        return false;
    }
    bool DoVerifySecret() {
        return false;
    }
    bool SecretDisplayed() {
        return false;
    }
    bool isConfirmed() {
        return false;
    }
    bool hasUTXO() {
        return false;
    }

    bool isPacksEqualExedosAmount() {
        return false;
    }
    bool isNameNew() {
        return false;
    }
    bool hadfsUTXO() {
        return false;
    }
    bool hasUdddTXO() {
        return false;
    }


    bool  mNameIsnew = true;

    void NewNameDialog() {}
    void SetNameIsNew() {
        mNameIsnew = true;
    }
    void DisplaySecretDialog() {}
    void DisplayHiddenFundingAddress() {}
    void DisplayFundingAddress() {}
    void StartCheckFundsTimer() {}
    void SignSendServer() {}
    void SignSendExedos() {}
    void StopCheckFundsTimer() {}
    void VerifySecretDialog() {}
    void SecretIsVerified() {}
    void VerifyError() {}
    void DisplayAddressBalance() {}
    void StartCheckExedosTimer() {}
    void StopCheckExedosTimer() {}
    void DisplayAddressPacksBalance() {}
    void StopCheckPacksTimer() {}
    void StartCheckPacksTimer() {}
    void RequestNewPacks() {}
    void SetVerifySecret(bool) {}



//    void checkname(const QString &name) {
//        qDebug() << " in checkname " << name;
//        WsReq req;
//        req.set_ctype(CHECKNAME);
//        CheckNameReq cnr;
//        cnr.set_fantasy_name(name.toStdString());
//        req.MutableExtension(CheckNameReq::req)->CopyFrom(cnr);
//        qDebug() << " checkname sending " << req.DebugString().data();
//        auto txstr = req.SerializeAsString();
//        QByteArray qb(txstr.data(),(size_t)txstr.size());
//        m_webSocket.sendBinaryMessage(qb);
//    }

    void saleStateGet() {
        WsReq req;
        req.set_ctype(GETSALESTATE);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " globalStateGet sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

signals:
    void usingFantasyName(const QString &name);
    void error(QString);
    void importSuccess(const QString name, bool passfail);


protected slots:
    void onConnected() {
        errCount = 0;
        QHostAddress hInfo = m_webSocket.peerAddress ();
        qDebug() << "connected to " <<  hInfo.toString () << " on Port " << m_webSocket.peerPort ();
        connect(&m_webSocket, SIGNAL(binaryMessageReceived(QByteArray)),
                this, SLOT ( onBinaryMessageRecived(QByteArray) ));

        saleStateGet();
    }

    void onBinaryMessageRecived(const QByteArray &message) {
        qDebug() << "CoinSale::onBinaryMessageRecived size" << message.size();

        fantasybit::WSReply rep;
        if ( !rep.ParseFromArray(message.data(),message.size()) ) {
            qDebug() << " !no parse";
            return;
        }
        else
            qDebug() << " yes parse " << rep.ctype();

        #ifdef TRACE2
           // qDebug() << "LightGateway::onBinaryMessageRecived " << rep.ShortDebugString().data();
        #endif

        switch ( rep.ctype()) {
            case GETSALESTATE: {
                qDebug() << "GETSALESTATE";
                const GetSaleStateRep &ss = rep.GetExtension(GetSaleStateRep::rep);
                set_totalAvailable(ss.available());
                //ss.fbperbitcoin();
                break;
            }
            default:
                break;
        }
//            case PK2FNAME: {
//                const Pk2FnameRep &pk2 = rep.GetExtension(Pk2FnameRep::rep);
//                auto name= pk2.fname();

//                if ( m_lastSignedplayer == pk2.req().pk()) {
//                    if ( name == "" ) {
//                        noNameCount++;
//                        if ( noNameCount > 50) {
//                            signPlayerStatus.stop();
//                            noNameCount = 0;
//                        }
//                        break;
//                    }
//                    else {
//                        signPlayerStatus.stop();
//                        noNameCount = 0;
//                        if ( m_myPubkeyFname.at(m_lastSignedplayer) ==  name) {
//                            auto fnp = Commissioner::AddName(name,m_lastSignedplayer);
//                            if ( fnp != nullptr ) {
//                                fnp->setBlockNump(pk2.fnb().block(),pk2.fnb().count());
//                                emit NewFantasyName(pk2.fnb());
//                                UseName(name.data());
//                            }
//                        }

//                        m_lastSignedplayer = "";
//                    }
//                }
//                break;
//            }



    }

//    void getSignedPlayerStatus() {
//        qDebug() << " getSignedPlayerStatus ";
//        doPk2fname(m_lastSignedplayer);
//    }
    void handleAboutToClose() {
        qDebug() << "handleAboutToClose" << errCount;
    }

    void handleClosed() {
        qDebug() << "handleClosed" << errCount;
        if ( errCount < 100 )
            m_webSocket.open(theServer);
    }

    void handleSocketError(QAbstractSocket::SocketError wse) {
        qDebug() << "handleSocketError" << wse;
        errCount++;
    }

    void handleSocketState(QAbstractSocket::SocketState ss) {
        qDebug() << "handleSocketState" << ss;
    }

};

}

#endif
