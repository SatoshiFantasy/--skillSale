//
// 11/17/2017  by: Jay Berg
// using StateMachine from SMC Compiler
//

#ifndef COINSALE_H
#define COINSALE_H

#include <QQmlConstRefPropertyHelpers.h>
#include <QQmlPtrPropertyHelpers.h>
//#include <QQmlAutoPropertyHelpers.h>
//#include <QQmlEnumClassHelper.h>

#include <QObject>
#include <QWebSocket>

#include "FantasyAgent.h"

#include "StateData.pb.h"
#include <fbutils.h>
#include <QString>
#include <QTimer>
#include <RestfullService.h>

#include "CoinSale_sm.h"
#include <mnemonic.h>
#include <bitcoinapi.h>
#include <fbsaletx.h>

namespace fantasybit {

class CoinSale : public QObject, public CoinSaleContext<CoinSale>
{
    Q_OBJECT

    FBSaleTXModel mFBSaleTXModel;

    QML_READONLY_CSTREF_PROPERTY(int,totalAvailable)
    QML_READONLY_CSTREF_PROPERTY(double,priceFB)

    QML_READONLY_CSTREF_PROPERTY(bool,busySend)
    QML_WRITABLE_CSTREF_PROPERTY(QString,currDialog)
    QML_WRITABLE_CSTREF_PROPERTY(QString,currStatus)

    QML_READONLY_CSTREF_PROPERTY(QString,currName)

    QML_READONLY_CSTREF_PROPERTY(QString,bitcoinAddress)

    QML_READONLY_CSTREF_PROPERTY(bool, isTestNet)

    QML_READONLY_CSTREF_PROPERTY(QString,secretShow)

    QML_READONLY_CSTREF_PROPERTY(bool,secretIsVerified)

    QML_READONLY_PTR_PROPERTY(FBSaleTXModel, pFBSaleTXModel)



    QWebSocket m_webSocket;
    std::string m_lastPk2name;
    std::string m_lastCheckName;
    fantasybit::FantasyAgent agent;
    int errCount;
    QUrl theServer;

    QTimer  importOrClaimPlayerStatus;
    QTimer  checkFundsTimer;
    int     noNameCount;
    bool    mSecretVerifed = false;
    bool    mHasUTXO = false;

    std::unordered_map<std::string, bool> mSecretDisplayed;
    std::unordered_map<std::string, bool> mSecretVerified;

    std::vector<utxoData> mUtxOs;
    std::string mTx, mTxId;




public:
    CoinSale(const std::string &host, int port,QObject *parent = 0)
                                    : QObject(parent),
                                      mFBSaleTXModel{this,QByteArray(),{"txid"}},
                                      m_pFBSaleTXModel(&mFBSaleTXModel),
                                      m_totalAvailable{0},
                                      m_busySend(true),
                                      m_currDialog(""),
                                      m_currStatus("starting App"),
                                      m_isTestNet(IS_TEST_NET),
                                      m_secretShow(""),
                                      m_secretIsVerified(false),
                                      noNameCount(0)
    {
        connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
        connect (&m_webSocket,SIGNAL(aboutToClose()),this,SLOT(handleAboutToClose()));
        connect (&m_webSocket, SIGNAL(disconnected()), this, SLOT(handleClosed()));

        // socket error
        connect (&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                 SLOT(handleSocketError(QAbstractSocket::SocketError)));

        // socket statte
        connect (&m_webSocket, SIGNAL (stateChanged(QAbstractSocket::SocketState)),
                 this, SLOT(handleSocketState(QAbstractSocket::SocketState)));

        connect (this, &CoinSale::currNameChanged, this, &CoinSale::OnNewName, Qt::QueuedConnection);
        connect (this, &CoinSale::nameAvail, this, &CoinSale::onNameAvail, Qt::QueuedConnection);
        connect (this, &CoinSale::currStatusChanged, this, &CoinSale::OnCurrSatusChanged,Qt::QueuedConnection);
        connect (this, &CoinSale::setToolPop, this, &CoinSale::ToolPop,Qt::QueuedConnection);

        QString wss("ws://%1:%2");
        QString lserver = wss.arg(host.data()).arg(port);

    #ifndef NODEBUG
        set_currStatus("try connecting to lserver: " + lserver);
        qDebug() << m_currStatus;
    #endif

        theServer = QUrl(lserver);
        m_webSocket.open(theServer);

//        signPlayerStatus.setInterval(2000);
//        connect(&signPlayerStatus, SIGNAL(timeout()),
//                this, SLOT(getSignedPlayerStatus()),Qt::QueuedConnection);
//        connect(Mediator::instance(),&Mediator::ready,this, &LightGateway::ClientReady,Qt::QueuedConnection);

        importOrClaimPlayerStatus.setInterval(2000);
        connect(&importOrClaimPlayerStatus, SIGNAL(timeout()),
                this, SLOT(getPlayerStatus()),Qt::QueuedConnection);

        checkFundsTimer.setInterval(4000);
        connect(&checkFundsTimer, SIGNAL(timeout()),
                this, SLOT(checkFunds()),Qt::QueuedConnection);
        checkFundsTimer.setSingleShot(true);

    }

    //States from QML
    Q_INVOKABLE void buy() {
        qDebug() << "buy ";
        Buy();
    }

    Q_INVOKABLE void toolPop(QString strin) {
        emit setToolPop(strin);
    }


    Q_INVOKABLE void verify(QString secret) {
        set_currStatus("verifying");
        Verify(secret);
    }

    Q_INVOKABLE void forgot() {
        Forgot();
    }

    Q_INVOKABLE void secretOk() {
        SecretOk();
    }

    Q_INVOKABLE void showAddress() {
           ShowAddress();
    }

    Q_INVOKABLE void doimport(QString secret) {
        qDebug() << "import ";
        auto pk = agent.startImportMnemonic(secret.toStdString());
        if ( pk == "" ) {
            set_currStatus("import failed");
            emit nameCheckGet("","import failed");
            return;//error
        }

        setbusySend(true);
        m_lastPk2name = pk;
//        mSecretVerifed = true;
        Import();
        noNameCount = 0;
        importOrClaimPlayerStatus.start();
    }

    Q_INVOKABLE void checkname(QString fname) {
//        qDebug() << "checkname ";
        m_lastCheckName = fname.toStdString();
        set_currStatus(QString("checking name(%1) - Hash(%2)").arg(fname).arg(FantasyName::name_hash(m_lastCheckName)));
        setbusySend(true);
        fantasyNameCheck(m_lastCheckName);
    }

    Q_INVOKABLE void signPlayer(QString fname) {
        mSecretDisplayed[fname.toStdString()] = false;
        mSecretVerified[fname.toStdString()] = false;
        Claim();
        qDebug() << "signPlayer ";

        setbusySend(true);

        agent.signPlayer(fname.toStdString());
        NameTrans nt{};
        nt.set_public_key(agent.pubKeyStr());
        nt.set_fantasy_name(fname.toStdString());

        Transaction trans{};
        trans.set_version(2);
        trans.set_type(TransType::NAME);
        trans.MutableExtension(NameTrans::name_trans)->CopyFrom(nt);
        SignedTransaction sn = agent.makeSigned(trans);

        DoPostTx(sn);
        m_lastPk2name = agent.pubKeyStr();
        noNameCount = 0;
        importOrClaimPlayerStatus.start();

    }

    Q_INVOKABLE QString getSecret() {
        qDebug() << " in get secret";
        QString secretr =  agent.getMnemonic(m_currName.toStdString ()).data ();
        qDebug() << secretr;
        return secretr;
    }

    void DoPostTx(const SignedTransaction &st) {
#ifndef NO_DOPOSTTX
        auto txstr = st.SerializeAsString();
        RestfullClient rest(QUrl(PAPIURL.data()));
        rest.postRawData("tx","octet-stream",txstr.data(),((size_t)txstr.size()));
#endif
    }

    bool HasName() {
        return !m_currName.isEmpty();
    }
    bool IsSecretVerified() {
        return mSecretVerifed;
    }
    bool isEqualFundedAmount() {
        return false;
    }

    bool DoVerifySecret(const QString &secret) {
        //agent.verifyseret ()
        try {
            bool ret =  ( agent.fromMnemonic(secret.toStdString ()).get_secret().str() == agent.getSecret());
            QString qs("Verify %1");

            set_currStatus(qs.arg ( ret ? " success " : " fail"));
            emit nameCheckGet(m_currName, QString("%1").
                              arg( (ret ? " verified! " : " verify fail")));

            return ret;
        }
        catch (MnemonicException e) {
            set_currStatus(e.what());
            return false;
        }
    }

    bool SecretDisplayed() {
        if ( HasName() )
            return mSecretDisplayed[m_currName.toStdString()];
        else
            return false;
    }
    bool isConfirmed() {
        return mTxId != "";
    }
    bool hasUTXO() {
        return mHasUTXO;
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

    void NewNameDialog() {
        set_currDialog("name");
    }
    void SetNameIsVerified() {
        if ( HasName() ) {
            mSecretDisplayed[m_currName.toStdString()] = true;
            mSecretVerified[m_currName.toStdString()] = true;
            setsecretShow(getSecret());
            setsecretIsVerified (true);
        }
    }

    void DisplayFundingAddress() {
        auto btcaddress = pb::toBtcAddress(agent.pubKey());
        setbitcoinAddress(btcaddress.data());
        set_currDialog("fund");
        qDebug() << m_bitcoinAddress;
        auto vec = BitcoinApi::getSpentTx(btcaddress,FUNDING_ADDRESS);
        //,"90d0a159b432afd45b043a286b7a12e362775d02d95b0b8f17bb742baa5f0c0b");
        for( auto v : vec ) {
            FBSaleTXItem *fbi = new FBSaleTXItem();
            fbi->set_txid(v.tx_hash);
            fbi->set_btc(v.out_value);
            qDebug() << v.tx_hash << v.out_value;
            mFBSaleTXModel.append(fbi);
        }
    }

    void DisplaySecretDialog() {
        setsecretShow(getSecret());
        set_currStatus("to Display Secret");
        set_currDialog("secret");

    }
    void DisplayHiddenFundingAddress() {
        set_currStatus("DisplayHiddenFundingAddress");
        set_currDialog("fund");
    }

    void StartCheckFundsTimer() {
        checkFundsTimer.start();
    }
    void SignSendServer() {
        mTxId = BitcoinApi::sendrawTx(mTx);
        if ( mTxId != "" )
            set_currStatus(mTxId.data());
    }

    void SignSendExedos() {
        set_currStatus(std::string("send txid to server" + mTxId).data());
    }

    void StopCheckFundsTimer() {

    }

    void VerifySecretDialog() {
        set_currStatus ("secretverify");
        set_currDialog("secretverify");
    }
    void SecretIsVerified() {
        mSecretVerified[m_currName.toStdString()] = true;
        setsecretIsVerified (true);
    }


    void VerifyError() {
        set_currStatus ("Verify Error");
    }
    void DisplayAddressBalance() {
        set_currDialog("balance");
    }
    void StartCheckExedosTimer() {}
    void StopCheckExedosTimer() {}
    void DisplayAddressPacksBalance() {}
    void StopCheckPacksTimer() {}
    void StartCheckPacksTimer() {}
    void RequestNewPacks() {}
    void SetVerifySecret(bool) {}

    void NameNotConfirmed() {}



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

    void doPk2fname(const std::string &pkstr) {
        WsReq req;
        Pk2FnameReq pkreq;
        pkreq.set_pk(pkstr);
        req.set_ctype(PK2FNAME);
        req.MutableExtension(Pk2FnameReq::req)->CopyFrom(pkreq);
        qDebug() << " doPk2fname sending " << req.DebugString().data();

        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());

        m_webSocket.sendBinaryMessage(qb);
    }

    void saleStateGet() {
        WsReq req;
        req.set_ctype(GETSALESTATE);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " globalStateGet sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void doSignReq(SignPackReq &packreq ) {
        WsReq req;
        req.set_ctype(SIGNPACK);
        packreq.set_fname(m_currName.toStdString());
        req.SetAllocatedExtension(SignPackReq::req,&packreq);
        QByteArray qbuf;
        qbuf.resize(req.ByteSize());
        req.SerializeToArray(qbuf.data(), qbuf.size());
        req.ReleaseExtension(SignPackReq::req);
        m_webSocket.sendBinaryMessage(qbuf);
    }

    void fantasyNameCheck(const std::string &checkname) {
        WsReq req;
        req.set_ctype(CHECKNAME);
//        req.SetExtension(CheckNameReq::req,CheckNameReq::default_instance());
//        req.MutableExtension(CheckNameReq::req)->set_fantasy_name(checkname);
        CheckNameReq cnr;
        cnr.set_fantasy_name(checkname);
        req.SetAllocatedExtension(CheckNameReq::req,&cnr);
        QByteArray qbuf;
        qbuf.resize(req.ByteSize());
        req.SerializeToArray(qbuf.data(), qbuf.size());
        req.ReleaseExtension(CheckNameReq::req);
        m_webSocket.sendBinaryMessage(qbuf);
    }

signals:
    void usingFantasyName(const QString &name);
    void error(QString);
    void importSuccess(const QString name, bool passfail);
    void nameCheckGet( const QString & name, const QString & status );
    void nameAvail( const QString name, bool );
    void setToolPop(QString &in);

protected slots:
    void ToolPop(QString &in) {
        emit nameCheckGet("", in);
    }

    void OnCurrSatusChanged(QString in) {
        qDebug() << in;
    }

    void onNameAvail(QString name,bool avail) {
        if ( m_lastCheckName.data() == name ) {
            setbusySend(false);
            if ( !avail )
                m_lastCheckName == "";
            emit nameCheckGet(name,avail ? "true" : "false");
        }
    }

    void OnNewName(QString name) {
        WsReq req;
        req.set_ctype(SUBSCRIBEFNAME);
        auto txstr = req.SerializeAsString();
        QByteArray qb(txstr.data(),(size_t)txstr.size());
        qDebug() << " globalStateGet sending " << req.DebugString().data();
        m_webSocket.sendBinaryMessage(qb);
    }

    void getPlayerStatus() {
        qDebug() << " getPlayerStatus ";
        doPk2fname(m_lastPk2name);
    }

    void checkFunds() {
        qDebug() << " checkFunds " << mHasUTXO;

        if ( !mHasUTXO )  {
            std::string btcaddress = m_bitcoinAddress.toStdString();
            mUtxOs = BitcoinApi::getUtxo(btcaddress);
            if ( mUtxOs.size() != 0 ) {
                std::vector<std::string> in_script;
                std::vector<std::string> raw_transaction;
                uint64_t insatoshis =
                        agent.createInputsfromUTXO(mUtxOs,
                                                   in_script,
                                                   raw_transaction);

                if ( in_script.size() > 0 ) {
                    mHasUTXO = true;
                    mTx = agent.createTxFromInputs(insatoshis,
                               FUNDING_ADDRESS,
                               in_script,
                               raw_transaction);
                    Funded();
                }
            }
        }
        else if ( mTxId == "")
            SignSendServer();

        if ( mTxId != "")
            Funded();
        else
            checkFundsTimer.start();
    }

    void onConnected() {
        errCount = 0;
        QHostAddress hInfo = m_webSocket.peerAddress ();
        qDebug() << "connected to " <<  hInfo.toString () << " on Port " << m_webSocket.peerPort ();
        set_currStatus("Connected to FB Sale Server" + hInfo.toString ());
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

        set_currStatus("onBinaryMessageRecived");
        switch ( rep.ctype()) {
            case SIGNPACK: {
                qDebug() << "SIGNPACK";
                SignPackRep *sprep = rep.MutableExtension(SignPackRep::rep);
                SignPackReq spreq;
                spreq.mutable_sig()->Swap(sprep->mutable_id());
                for ( int i = 0; i < spreq.sig_size(); i++) {
                    auto *kv = spreq.mutable_sig(i);
                    kv->set_value(agent.sign(kv->key()));
                }
                doSignReq(spreq);
                emit nameCheckGet(m_currName, "Signing Packs");

                break;
            }
            case GETSALESTATE: {
                qDebug() << "GETSALESTATE";
                const GetSaleStateRep &ss = rep.GetExtension(GetSaleStateRep::rep);
                settotalAvailable(ss.available());
//                ss.fbperbitcoin()
                setbusySend(false);
                //ss.fbperbitcoin();
                break;
            }
            case PK2FNAME: {
                const Pk2FnameRep &pk2 = rep.GetExtension(Pk2FnameRep::rep);
                std::string name= pk2.fname();

                qDebug() << pk2.DebugString().data();
                if ( m_lastPk2name == pk2.req().pk()) {
                    if ( name == "" ) {
                        noNameCount++;
                        if ( noNameCount > ((m_lastCheckName == "") ? 2 : 25) ) {
                            importOrClaimPlayerStatus.stop();
                            noNameCount = 0;
                            m_lastCheckName = "";
                            m_lastPk2name = "";
                            setbusySend(false);
                            set_currStatus("timeout name");
                            emit nameCheckGet(m_currName, "Name Not-Confirmed - Timeout");
                            NameNotConfirmed();
                        }
                    }
                    else {
                        if ( name ==  m_lastCheckName ) {
                            importOrClaimPlayerStatus.stop();
                            noNameCount = 0;

                            if ( agent.UseName(name) )
                                setcurrName(name.data());

                            m_lastCheckName = "";
                            m_lastPk2name = "";
                            setbusySend(false);
                            emit nameCheckGet(m_currName, "Name Confirmed!");
                            NameConfimed();
                        }
                        else if ( m_lastCheckName == "" ) {

                            importOrClaimPlayerStatus.stop();
                            noNameCount = 0;
                            setbusySend(false);

                            if ( agent.finishImportMnemonic(m_lastPk2name,name) ) {
                                if ( agent.UseName(name) ) {
                                    setcurrName(name.data());
                                    emit nameCheckGet(m_currName, "Name Confirmed");

                                    NameConfimed();
                                }
                                else {
                                    emit nameCheckGet(m_currName, "Name Not Confirmed");
                                    NameNotConfirmed();
                                }
                            }
                            else {
                                emit nameCheckGet(m_currName, "Name Not Confirmed");
                                NameNotConfirmed();
                            }


                            m_lastPk2name = "";
                        }
                        else {
                            importOrClaimPlayerStatus.stop();
                            noNameCount = 0;
                            emit nameCheckGet(m_lastCheckName.data(), "false");
                            m_lastCheckName = "";
                            m_lastPk2name = "";
                            setbusySend(false);
                            NameNotConfirmed();
                        }
                    }
                }
                break;
            }

            case CHECKNAME: {
                const CheckNameRep &cnr = rep.GetExtension(CheckNameRep::rep);
                if (  cnr.req().fantasy_name() != "")
                    emit nameAvail(cnr.req().fantasy_name().data(),cnr.isavail() == "yes");
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
        set_currStatus("handleAboutToClose");
    }

    void handleClosed() {
        qDebug() << "handleClosed" << errCount;
        if ( errCount < 100 ) {
            set_currStatus(QString("socket closed retry %1 of 100").arg(errCount));
            m_webSocket.open(theServer);
        }
        else
            set_currStatus(QString("socket closed forever - plese restart App - unable to connect %1").arg(theServer.toString()));
    }

    void handleSocketError(QAbstractSocket::SocketError wse) {
        qDebug() << "handleSocketError" << wse;
        set_currStatus(QString("socket error %1").arg(wse));

        errCount++;
    }

    void handleSocketState(QAbstractSocket::SocketState ss) {
        qDebug() << "handleSocketState" << ss;
        set_currStatus(QString("socket state %1").arg(ss));
    }

};

}

#endif
