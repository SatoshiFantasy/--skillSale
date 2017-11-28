#ifndef BITCOINAPI_H
#define BITCOINAPI_H

#include <QString>
#include <vector>
#include <RestfullService.h>
#include <utils.h>
#include <QByteArray>

namespace fantasybit {

struct utxoData {
    QString tx_hash;
    QString script;
    uint64_t in_value;
    uint32_t tx_output_n;
};

class BitcoinApi {
public:
    BitcoinApi();

    static std::vector<utxoData> getUtxo(const std::string &btcaddress) {
        std::vector<utxoData> ret;

        auto json = RestfullService::getBlockchainBtcAddressUnspent (btcaddress);
        if ( json != "Service Unavailable") {

            QJsonParseError * error = NULL;
            QJsonDocument doc = QJsonDocument::fromJson(json,error);
            if (error != NULL || doc.isEmpty()){
                    qDebug() << " error parsing json";
                if ( error != NULL ) qDebug() << error->errorString();
                    return ret;
            }

            qDebug() << json;
            qDebug() << doc.isNull() << doc.isEmpty() << doc.isArray() << doc.isObject();
            QJsonObject jo = doc.object();
            QJsonArray utxos = jo.value("unspent_outputs").toArray();
            for(QJsonValueRef ut : utxos) {
                ret.push_back({});
                utxoData &utd = ret.back();
                QJsonObject vo = ut.toObject();
                utd.tx_hash = vo.value("tx_hash").toString();
                utd.script = vo.value("script").toString();
                utd.in_value = vo.value("value").toInt();
                utd.tx_output_n = vo.value("tx_output_n").toInt();
            }

            return ret;
        }

        auto json2 = RestfullService::getChainsoBtcAddressUnspent (btcaddress);
        QJsonParseError * error = NULL;
        QJsonDocument doc = QJsonDocument::fromJson(json2,error);
        if (error != NULL || doc.isEmpty()){
                qDebug() << " error parsing json";
            if ( error != NULL )
                qDebug() << error->errorString();
            return ret;
        }

        qDebug() << json2;
        qDebug() << doc.isNull() << doc.isEmpty() << doc.isArray() << doc.isObject();
        QJsonObject jo = doc.object();
        if (jo.value("status") == "success") {
            QJsonValue data = jo.value("data");
            QJsonObject jo2 = data.toObject();
            QJsonArray utxos = jo2.value("txs").toArray();
            for(QJsonValueRef ut : utxos) {
                ret.push_back({});
                utxoData &utd = ret.back();
                QJsonObject vo = ut.toObject();
                auto txid = vo.value("txid").toString();
                pb::sha256 txhash(txid.toStdString());
//                auto reverserhash = pb::toReverseHexFromDecimal (txhash.begin());
                utd.tx_hash = txhash.reversestr().data();
                utd.script = vo.value("script_hex").toString();
                auto strval = vo.value("value").toString();
                utd.in_value = strval.remove('.').toULongLong();
                utd.tx_output_n = vo.value("output_no").toInt();
            }

            return ret;
        }
        else {
            qDebug() << " error getChainsoBtcAddressUnspent ";
        }

        return ret;

    }

    static std::string sendrawTx(const std::string &rawin) {
        auto json = RestfullService::pushBitcoinTx(rawin);
        if ( json == "Service Unavailable") {
            auto json2 = RestfullService::pushChainsoBitcoinTx(rawin);

            QJsonParseError * error = NULL;
            QJsonDocument doc = QJsonDocument::fromJson(json2,error);
            if (error != NULL || doc.isEmpty()){
                    qDebug() << " error parsing json";
                if ( error != NULL )
                    qDebug() << error->errorString();

               return "";
            }

            qDebug() << json2;
            qDebug() << doc.isNull() << doc.isEmpty() << doc.isArray() << doc.isObject();
            QJsonObject jo = doc.object();
            if (jo.value("status") == "success") {
                QJsonValue data = jo.value("data");
                QJsonObject jo2 = data.toObject();
                auto txidin = jo2.value("txid").toString();

                return txidin.toStdString();
            }
            else
                return "";
        }
    }

};

}
#endif // BITCOINAPI_H
