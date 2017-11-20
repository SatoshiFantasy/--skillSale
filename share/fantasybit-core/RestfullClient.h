#ifndef RESTFULLCALL_H
#define RESTFULLCALL_H

#include <QObject>
#include <QSharedDataPointer>
#include <QNetworkAccessManager>
#include <QThread>
#include <QNetworkReply>
#include <QEventLoop>
#include <globals.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <google/protobuf/message.h>
#include <QJsonArray>
#include <QDebug>
#include <sstream>
//#include "fbutils.h"

/**
 * @brief The RestfullClient class : This is NOT a thread safe class.
 */
class RestfullClient : public QObject{

    Q_OBJECT

public:

    enum RestfullCallPostDataType {
        Binary,
        Json,
        String
    };

    enum RestfullCallTextEncoding {
        UTF8,
        Latin1,//ISO 8859-1, use it for ascii
        Local8Bits // convert to 8 bits according to the os local
    };

private:

    class PostDataDecorator   {

    public:

        PostDataDecorator(QNetworkAccessManager * manager, QNetworkRequest * request){
            Q_ASSERT(manager != NULL && request != NULL);
            myRequest = request;
            myNam = manager;
        }

        QNetworkReply * postBinaryData(const QString & contentType,const QByteArray & buffer){
            return postRawData(contentType,RestfullCallPostDataType::Binary,buffer);
        }

        QNetworkReply * postBinaryData(const QString & contentType,const char * data,int size){
            return postRawData(contentType,RestfullCallPostDataType::Binary,QByteArray(data,size));
        }

        QNetworkReply * postJsonData(const QString & contentType,const QJsonDocument & jsonDocString){
            return postGenericStringData(contentType,RestfullCallPostDataType::Json,QString(jsonDocString.toJson(QJsonDocument::Compact)),UTF8);
        }

        QNetworkReply * postStringData(const QString & contentType,const QString & str,RestfullCallTextEncoding encoding){
            return postGenericStringData(contentType,RestfullCallPostDataType::String,str,encoding);
        }

    private:
        QNetworkRequest * myRequest;
        QNetworkAccessManager * myNam;

        QNetworkReply * postRawData(const QString & contentType,
                                    RestfullCallPostDataType pType,
                                    const QByteArray & postData) {

            myRequest->setHeader(QNetworkRequest::ContentTypeHeader,contentType);
            myRequest->setHeader(QNetworkRequest::ContentLengthHeader,postData.length());
            if (myNam->networkAccessible()==QNetworkAccessManager::Accessible){
#ifdef TRACE
                qDebug() << "Post : " << myRequest->url().toDisplayString();
#endif
                return myNam->post(*myRequest,postData);
            }
            else {
                qDebug()<< "No network connection !";
                return NULL;
            }

        }

        QNetworkReply * postGenericStringData(const QString & contentType,RestfullCallPostDataType pType,
                                              const QString & theString,
                                              RestfullCallTextEncoding encoding){
            QByteArray buffer;
            switch (encoding) {
            case RestfullCallTextEncoding::UTF8:  buffer = theString.toUtf8(); break;
            case RestfullCallTextEncoding::Latin1: buffer = theString.toLatin1(); break;
            case RestfullCallTextEncoding::Local8Bits: buffer = theString.toLocal8Bit(); break;
            default:
                break;
            }
            return postRawData(contentType,pType,buffer);
        }
    };

public:

    RestfullClient(const QUrl & argBaseUrl,QThread * ownerThread = QThread::currentThread()){
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

    bool postProtoMessageData(const QString & route,const QString & contentType,const GOOGLE_NAMESPACE::protobuf::Message & protoMessage){
        std::string data = protoMessage.SerializeAsString().data();
        return postRawData(route,contentType,data.data(),data.size());
    }

    bool postRawData(const QString & route,const QString & contentType,const QByteArray & postData){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postBinaryData(contentType,postData);
        if (myCurrentNetworkReply == NULL) return false;
        waitForReply();
        return true;
    }

    bool postRawData(const QString & route,const QString & contentType,const char * data,int size){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postBinaryData(contentType,data,size);
        if (myCurrentNetworkReply == NULL) return false;
        waitForReply();
        return true;
    }

    bool postJasonData(const QString & route,const QString & contentType,const QJsonDocument & jsonDoc){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postJsonData(contentType,jsonDoc);
        if (myCurrentNetworkReply == NULL) return false;
        waitForReply();
        return true;
    }

    bool postStringData(const QString & route,const QString & contentType,const QString & text,
                        RestfullCallTextEncoding encoding){
        QNetworkRequest request;
        restNetworkStatus();
        request.setUrl(QUrl(myBaseUrl.toString()+"/"+route));
        PostDataDecorator decorator(&myNetworkManager,&request);
        myCurrentNetworkReply = decorator.postStringData(contentType,text,encoding);
        if (myCurrentNetworkReply == NULL) return false;
        waitForReply();
        return true;
    }

    bool    getData(const QString & route,
                 const QMap<QString,QVariant> parameters,
                 const QMap<QString,QString> headersMap)  {
        QNetworkRequest request;
        restNetworkStatus();
        //construct url with parameters
        QString url;
        if ( route != "" )
            url = myBaseUrl.toString()+"/"+route;
        else
            url = myBaseUrl.toString();

        // if (parameters.count()> 1) url+="/";


        bool first = true;
        foreach (QString paramName, parameters.keys()) {
            if ( first ) {
                first = false;
                url+=QString("?%1=%2").arg(paramName).arg(parameters.value(paramName).toString());
            }
            else {
                url+=QString("&%1=%2").arg(paramName).arg(parameters.value(paramName).toString());
            }
        }
        request.setUrl(QUrl(url));

        //add headers
        foreach (QString headerKey, headersMap.keys()) {
            request.setRawHeader(headerKey.toUtf8(),headersMap.value(headerKey).toUtf8());
        }

        //qDebug() << "Get : " << request.url().toDisplayString();
        if (myNetworkManager.networkAccessible()==QNetworkAccessManager::Accessible){
            myCurrentNetworkReply = myNetworkManager.get(request);
            //qDebug() << "waitForReply : " << request.url().toDisplayString();

            waitForReply();
            return true;
        }
        else {
            qDebug()<< "No network connection !";
            return false;
        }
    }

    bool getData(const QString & route) {
        QMap<QString,QString>  headers;
        QMap<QString,QVariant> params;
        return getData(route,params,headers);
    }

    bool getData(const QString & route,const QMap<QString,QVariant> parameters) {
        QMap<QString,QString>  headers;
        return getData(route,parameters,headers);
    }

    bool getData(const QString & route, const QMap<QString,QString> headersMap) {
        QMap<QString,QVariant> parameters;
        return getData(route,parameters,headersMap);
    }

    QByteArray lastReply() {
#ifdef TRACE3
        if ( myLastRepliedData.size() > 100 )
            qDebug() << myLastRepliedData.size();
        else
            qDebug() << myLastRepliedData;
#endif

        return myLastRepliedData;
    }

    int lastCode() {
        return statusCode;
    }

    int statusCode = 0;

    bool postTData(const QString & route,
                 const QMap<QString,QVariant> parameters,
                 const QMap<QString,QString> headersMap)  {
        QNetworkRequest request;
        restNetworkStatus();
        //construct url with parameters
        //if (parameters.count()> 0) url+="?";
        QString url;
        if ( route != "" )
            url = myBaseUrl.toString()+"/"+route;
        else
            url = myBaseUrl.toString();

        QByteArray postBodyContent;
        bool first = true;
        foreach (QString paramName, parameters.keys()) {
            if ( first ) {
                first = false;
                url+=QString("?%1=%2").arg(paramName).arg(parameters.value(paramName).toString());
            }
            else {
                url+=QString("&%1=%2").arg(paramName).arg(parameters.value(paramName).toString());
            }
            postBodyContent += QString("&%1=%2").arg(paramName).arg(parameters.value(paramName).toString());
        }
        request.setUrl(url);
        //add headers
        foreach (QString headerKey, headersMap.keys()) {
            request.setRawHeader(headerKey.toUtf8(),headersMap.value(headerKey).toUtf8());
        }

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
#ifdef TRACE
        qDebug() << "Post : " << request.url().toDisplayString();
#endif

        if (myNetworkManager.networkAccessible()==QNetworkAccessManager::Accessible){
            myCurrentNetworkReply = myNetworkManager.post(request,postBodyContent);
            //qDebug() << "waitForReply : " << request.url().toDisplayString();

            waitForReply();
            return true;
        }
        else {
            qDebug()<< "No network connection !";
            return false;
        }
    }
    /**/

signals:

    void doneReading();

private slots:

    void finishedSlot(QNetworkReply* reply){
        //qDebug() << "finishedSlot";
        if (reply != NULL){
            myLastRepliedData = reply->readAll();
            statusCode =
                    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            reply->deleteLater();
        }
        else
            qDebug() << "null reply";
        emit doneReading();
    }

    void sslErrorsSlot(QNetworkReply * reply, const QList<QSslError> & errors) {
        if (reply !=NULL)
            myCurentSSLErrors.clear();
        if (errors.count()>1)
            qDebug() << "ssl error for: ";
        myCurentSSLErrors.append(errors);
        reply->ignoreSslErrors();
    }

    void networkErrorSlot(QNetworkReply::NetworkError error ) {
        if (myCurrentNetworkReply == NULL)
            return;

        QUrl redirection =
                myCurrentNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

        if (redirection.isValid()) //no action is taken we're always allowing redirection
            qDebug() << "server redirection : " << redirection.toString() ;
        else {
            qDebug() << "net error : " << myCurrentNetworkReply->errorString();
            //the finsihed signal will follow
        }

        myCurrentNetworkError = error;
        myCurrentNetworkReply->ignoreSslErrors();

        statusCode =
                myCurrentNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
   }

    void networkAccessibleChangedSlot(QNetworkAccessManager::NetworkAccessibility accessible){
        if (accessible != QNetworkAccessManager::Accessible)
            if (myEventLoop.isRunning()) myEventLoop.quit();
    }

private:

    /**
      * @brief restNetworkStatus : reset network errors and status.
      */
    void restNetworkStatus(){
        myCurentSSLErrors.clear();
        myCurrentNetworkError = QNetworkReply::NoError;
        if (myCurrentNetworkReply != NULL)
            myCurrentNetworkReply->close();
        DELETE_AND_NULLIFY(myCurrentNetworkReply);
    }

    /**
       * @brief waitForReply : loop while waiting for asynchronus network reply to be available.
       * or a network error to happen.
       */
    void waitForReply(){
        myLastRepliedData.clear();
        //when network error, report it and exit wait loop.
        if (myCurrentNetworkReply != NULL) {
            connect(myCurrentNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)),
                    this, SLOT(networkErrorSlot(QNetworkReply::NetworkError)));
            myEventLoop.exec();
        }
    }

    QNetworkAccessManager  myNetworkManager;
    /**
     * @brief myCurrentNetworkReply : current network reply getting processed
     */
    QNetworkReply * myCurrentNetworkReply;

    /**
     * @brief myLastRepliedData : last network captured reply.
     */
    QByteArray myLastRepliedData;

    /**
     * @brief myCurrentNetworkError : last network error
     */
    QNetworkReply::NetworkError  myCurrentNetworkError;

    /**
     * @brief myCurentSSLErrors : last ssl error
     */
    QList<QSslError> myCurentSSLErrors;

    /**
     * @brief myBaseUrl : base url
     */
    QUrl myBaseUrl;

    /**
     * @brief myEventLoop : waiting loop.
     */
    QEventLoop myEventLoop;

};

#endif // RESTFULLCALL_H
