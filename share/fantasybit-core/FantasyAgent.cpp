//
//  FanatsyAgent.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//


#include <iostream>
#include <utility>
#include <memory>
#include "FantasyAgent.h"
#include <Commissioner.h>
#include "DataPersist.h"
//#include "optional.hpp"
#include "FantasyName.h"
#include "mnemonic.h"
#include "utils/utils.h"
#include <openssl/rand.h>
//#include <QFile>
//#include <QString>
#include <QStandardPaths>
#include <RestFullService.h>
#include <fbutils.h>
#include <QDebug>


using namespace fantasybit;

FantasyAgent::FantasyAgent(string filename ) : client2{nullptr} {

    if ( filename != "" )
        walldfilename = filename;
    else
        walldfilename = "walldfilename.dat";

    if ( !readFromSecret( GET_ROOT_DIR() +  walldfilename, false) ) {
        qDebug() << " readFromSecret ";
    }
}

bool FantasyAgent::readFromSecret(const std::string &readfrom, bool transfer) {

    std::vector<WalletD> temp;
    {
        Reader<WalletD> read{ readfrom };
            if ( !read.good() )
                return false;

        WalletD secret;
        while (read.ReadNext(secret)) {
            if (transfer) temp.push_back(secret);

            if ( !testIt(secret) ) {
                qCritical() << " secret verify fail" << secret.fantasy_name().data();
                continue;
            }

            if ( secret.has_mnemonic_key() )
                secret.clear_mnemonic_key();

            m_secrets.push_back(secret);



//            auto idsig = getIdSig ("testit",pvk);

            qDebug() << "";//idsig.first.data () << idsig.second.data ();

//            testBtc(secret);
        }
    }

    return true;

}

std::string FantasyAgent::getMnemonic(std::string fname) {
    Reader<WalletD> read{ GET_ROOT_DIR() +  walldfilename};
    if ( !read.good() ) {
        qDebug() << "getMnemonic bad read";
        return "bad read";
    }

    string ret = fname;
    WalletD secret{};
    while (read.ReadNext(secret)) {

        if ( secret.fantasy_name() == fname ) {
            if ( !testIt(secret) ) {
                qCritical() << " secret verify fail" << secret.fantasy_name().data();
                ret += " verify fail";
                continue;
            }

            if ( secret.has_mnemonic_key() )
                return secret.mnemonic_key();
            else
                ret += " no has_mnemonic_key";
        }
    }

    return ret + " not found";
}

/*
bool FantasyAgent::testBtc(WalletD secret) {
//    long a =RestfullService::getBtcAddressBalance ("13yhCtK9q1qKmx79pNBqqsGSrtJBKYzsXJ");
//    qDebug() << "13yhCtK9q1qKmx79pNBqqsGSrtJBKYzsXJ" << a;


//    a =RestfullService::getBtcAddressBalance ("1M4WFYhNg8j1zYQ2qpjWvkyEWd1GuuxCyc");
//    qDebug() << "1M4WFYhNg8j1zYQ2qpjWvkyEWd1GuuxCyc" << a;


//    a =RestfullService::getBtcAddressBalance ("13yhCtK9q1qKmx79pNBqqsGSrtJBKYzsXk");
//    qDebug() << "13yhCtK9q1qKmx79pNBqqsGSrtJBKYzsXk" << a;

    auto pk = Commissioner::str2pk (secret.public_key ());
    std::string btc1 = pb::toBtcAddress (pk);
    qDebug() << secret.public_key ().data () << btc1.data ();

    if ( btc1 != "1D7JJrwsTnCBT8r9ctLqEbVMP3BGDdh9Wq") return false;
//    std::string fbtc1 = pb::fromBtcAddress (btc1);

//    auto pvk = str2priv(secret.private_key ());

//    auto sha = pb::hashit( "testit");
//    auto sig = pvk.sign(sha);

//    std::string fordebug = "";
    std::string btcaddress = btc1;//"1D7JJrwsTnCBT8r9ctLqEbVMP3BGDdh9Wq";
    std::string fundaddress = "17AWWXT4f12i7gyHs7t6cFtMDsfJfLb2Ky";


    */
bool FantasyAgent::testBtc(WalletD secret) {

    auto pk = Commissioner::str2pk (secret.public_key ());
    std::string btc1 = pb::toBtcAddress (pk);
    qDebug() << secret.public_key ().data () << btc1.data ();

    if ( btc1 != "1D7JJrwsTnCBT8r9ctLqEbVMP3BGDdh9Wq") return false;

        std::string btcaddress = btc1;//"1D7JJrwsTnCBT8r9ctLqEbVMP3BGDdh9Wq";
    std::string fundaddress = "17AWWXT4f12i7gyHs7t6cFtMDsfJfLb2Ky";
    auto json = RestfullService::getBtcAddressUnspent (btcaddress);

    QJsonParseError * error = NULL;
    QJsonDocument doc = QJsonDocument::fromJson(json,error);

    if (error != NULL || doc.isEmpty()){

        qDebug() << " error parsing json";
        if ( error != NULL ) qDebug() << error->errorString();
        return false;
    }
    qDebug() << json;

    qDebug() << doc.isNull() << doc.isEmpty() << doc.isArray() << doc.isObject();
    QJsonObject jo = doc.object();
    QJsonArray utxos = jo.value("unspent_outputs").toArray();


    std::vector<std::string> fordebug;
    std::vector<std::string> fordebugscript;
    std::vector<std::string> raw_transaction;
    std::string raw_transaction_out{""};
    std::vector<std::string> in_script;
    int numinputs = 0;
    uint64_t inputsatoshis = 0;
    for(QJsonValueRef ut : utxos) {
        QJsonObject vo = ut.toObject();
        QString tx_hash = vo.value("tx_hash").toString();
//        QString tx_hash_big_endian = vo.value("tx_hash_big_endian").toString();
        QString script = vo.value("script").toString();
        uint64_t in_value = vo.value("value").toInt();
        uint32_t tx_output_n = vo.value("tx_output_n").toInt();

        inputsatoshis += in_value;

        qDebug() << tx_hash << script <<  in_value << tx_output_n ;
        qDebug() << "";
        numinputs++;

        raw_transaction.push_back (tx_hash.toStdString());
        fordebug.push_back (tx_hash.toStdString() + "\n");
        auto reversetxoun = pb::toReverseHexFromDecimal (tx_output_n);
        raw_transaction.back () += reversetxoun;
        fordebug.back () += reversetxoun + "\n";
        auto ssize = ( unsigned char )( script.size( ) / 2 );
        auto sstr = pb::to_hex ( &ssize, sizeof( unsigned char ) );
        in_script.push_back( sstr + script.toStdString());
        fordebugscript.push_back (sstr + "\n");
        fordebugscript.back () += script.toStdString() + "\n";
//        qDebug() << " in_script.back() " << in_script.back().data();
    }


//    raw_transaction += in_script.back();
//    raw_transaction += "ffffffff"; //seqfff

    //end of inputs

    std::string fordebugout = "";
    uint64_t satoshifee = 100000;
    int numoutputs = 2;
    {
    auto size = ( unsigned char )numoutputs;
    auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
    fordebugout += sstr + "\n";
    raw_transaction_out += sstr;
    }
    auto amstr = pb::toReverseHexFromDecimal(uint64_t(inputsatoshis - satoshifee));
    raw_transaction_out += amstr;
    fordebugout += amstr + "\n";

    //OP_DUP OP_HASH160 <PubkeyHash> OP_EQUALVERIFY OP_CHECKSIG
    std::string OP_DUP = "76";
    std::string OP_HASH160 = "a9";
    std::string OP_EQUALVERIFY = "88";
    std::string OP_CHECKSIG = "ac";
    std::string OP_RETURN = "6a";

    std::string p2pkh  = OP_DUP + OP_HASH160;
    std::string pubkeyHash = pb::fromBtcAddress (fundaddress);
    {
    auto size = ( unsigned char )( pubkeyHash.size( ) / 2 );
    auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
    p2pkh += sstr;
    }
    p2pkh += pubkeyHash;
    p2pkh += OP_EQUALVERIFY + OP_CHECKSIG;


//    qDebug() << "sigscript:" << sigscript.data ();
    qDebug() << "p2pkh:" << p2pkh.data ();

    {
    auto size = ( unsigned char )( p2pkh.size( ) / 2 );
    auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
    raw_transaction_out += sstr;
    fordebugout += sstr + "\n";
    }
    fordebugout += p2pkh + "\n";
    raw_transaction_out += p2pkh;


    if ( numoutputs == 2) {
        //OP_RETURN
        std::string opret_out = OP_RETURN;
        std::string debug_opret_out = OP_RETURN + "\n";
        std::string hexname = pb::to_hex(secret.fantasy_name ());
        {
        auto size = ( unsigned char )(hexname.size ()/2);
        auto holdsize = pb::to_hex ( &size, sizeof( unsigned char ) );
        opret_out  += holdsize;
        debug_opret_out += holdsize + "\n";
        }
        opret_out += hexname;
        debug_opret_out += hexname + "\n";
        {
        auto size = ( unsigned char )(opret_out.size() /2);
        auto holdsize = pb::to_hex ( &size, sizeof( unsigned char ) );
        opret_out  = holdsize + opret_out;
        debug_opret_out = holdsize + "\n" + debug_opret_out;
        }
        raw_transaction_out += "0000000000000000";
        raw_transaction_out += opret_out;
        fordebugout += "0000000000000000\n" + debug_opret_out;
    }

    std::string locktime = "00000000";
    fordebugout += locktime + "\n";
    raw_transaction_out += locktime;
//    raw_transaction_out += sighash_all;
//    qDebug() << "raw_transaction_out:" << raw_transaction_out.data ();


    std::string sighash_all = "01000000";
//    fordebugout += sighash_all + "\n";

    std::string  raw_transaction_pre = "01000000"; // i.e. version
    std::string fordebug_pre =  raw_transaction_pre + "\n";
    {
    auto size = ( unsigned char )numinputs;
    auto holdsize = pb::to_hex ( &size, sizeof( unsigned char ) );
    fordebug_pre += holdsize + "\n";
    raw_transaction_pre += holdsize;
    }
    //raw_transaction = raw_transaction;
    std::vector<std::string> to_sign;
    std::vector<std::string> to_sign_debug;

    for ( int i=0; i< numinputs; i++) {
        to_sign.push_back(raw_transaction_pre);
        to_sign_debug.push_back (fordebug_pre);
        for ( int j=0; j<numinputs; j++) {
            if ( j == i) {
                to_sign.back () += raw_transaction.at(i);
                to_sign_debug.back() += fordebug.at(i);
                to_sign.back() += in_script[i];
                to_sign_debug.back () += fordebugscript[i];
            }
            else {
                to_sign.back () += raw_transaction.at(j);
                to_sign_debug.back() += fordebug.at(j);
                to_sign.back() += "00";
                to_sign_debug.back () += "00\n";
            }
            to_sign_debug.back () +=  "ffffffff\n";
            to_sign.back() += "ffffffff";
        }
    }




//    fordebug += fordebugout;
//    qDebug() << fordebug.data();

     auto pvk = str2priv(secret.private_key ());

    //    auto sha = pb::hashit( "testit");
    //    auto sig = pvk.sign(sha);


    std::vector<std::string> sigouts;
    std::vector<std::string> debug_sigouts;
    for ( int i=0; i< to_sign.size(); i++) {
        to_sign.at(i) += raw_transaction_out;
        to_sign.at(i) += sighash_all;
        to_sign_debug.at(i) +=  fordebugout + sighash_all;

        qDebug() << i << "to_sign.at()";
        qDebug() << to_sign_debug.at(i).data();

        auto dblhash = pb::hashit(pb::hashfromhex(to_sign.at(i)));
        auto sig = pvk.sign(dblhash);
        auto dersig = pb::serialize_der (sig);
        dersig = pb::to_hex(dersig);
        auto debugdersig = dersig + "\n";
//        qDebug() << " dersig" << dersig.data ();

        dersig += "01";
        debugdersig += "01\n";
        auto size = ( unsigned char )( dersig.size( ) / 2 );
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        dersig = sstr + dersig;
        debugdersig = sstr + "\n" + debugdersig;

//        qDebug() << " size " << size << "dersig.size(  " << dersig.size( ) << " sstr " << sstr.data();

        dersig += "21";
        debugdersig += "21\n";
        sstr = pb::to_hex(pk.begin (),33);
        dersig.append (sstr);
        debugdersig.append (sstr + "\n");
        sigouts.push_back (dersig);//sig.data,64));
        debug_sigouts.push_back (debugdersig);
    }

//    qDebug() << sigouts[0].data ();

    std::string final_tx = raw_transaction_pre;
    std::string final_debug_tx = fordebug_pre;
    for ( int i=0; i< numinputs; i++) {
        final_tx += raw_transaction.at(i);
        final_debug_tx += fordebug.at(i);

        auto ssize = ( unsigned char )( sigouts.at(i).size( ) / 2 );
        auto sstr = pb::to_hex ( &ssize, sizeof( unsigned char ) );
//        qDebug() << " ssize " << ssize << "sigouts.at(i).size(  " << sigouts.at(i).size( ) << " sstr " << sstr.data();
        final_debug_tx += sstr + "\n" + debug_sigouts.at(i);
        final_tx += sstr + sigouts.at(i);
        final_tx += "ffffffff";
        final_debug_tx += "ffffffff\n";
    }

    final_tx += raw_transaction_out;
    final_debug_tx += fordebugout;
    qDebug() << "final tx" << final_tx.data ();
    qDebug() << "final_debug_tx";
    qDebug() << final_debug_tx.data ();


    std::string testtx = "0100000001eccf7e3034189b851985d871f91384b8ee357cd47c3024736e5676eb2debb3f2010000001976a914010966776006953d5567439e5e39f86a0d273bee88acffffffff01605af405000000001976a914097072524438d003d23a2f23edb65aae1bb3e46988ac0000000001000000";
    auto testh = pb::hashit(pb::hashfromhex(testtx));
    qDebug() << " test hash " << testh.str();

//    raw_transaction += seqfff; //sequence number


    //    std::string raw_tx_forsign = raw_transaction;
//    std::string tx_hash_big_endian2 = "6dd11048c73bbca4d165494fc0f3084b349071da6261f6326d6ddcd21df4f5df";
//    std::string scriptPubKey = "76a91420a91b56f1e4b608ee14be192dc0136eee76b80f88ac";
//    std::string scriptPubKeySize = "19";
//


    /*
    std::string raw_input = tx_hash_big_endian2;


    uint32_t tx_output_n2 = 0;
    raw_input += pb::toReverseHexFromDecimal (tx_output_n2);

    tx_output_n2 = 32;
    std::string raw_input2 = tx_hash_big_endian2;
    std::string hexdec = pb::toReverseHexFromDecimal (tx_output_n2);
    raw_input2 += hexdec;

    qDebug() << "raw_transaction:" << raw_transaction.data ();
    qDebug() << "raw_input:" << raw_input.data ();
    qDebug() << "raw_input2:" << raw_input2.data ();

//    for ( uint32_t i = 950; i<1000; i++) {
//        qDebug() << i << ":" << pb::toReverseHexFromDecimal (i).data ();
//    }

    std::string raw_tx_forsign = raw_transaction + raw_input + scriptPubKeySize + scriptPubKey + seqfff;

    */



}

//std::string FantasyAgent::createTestTx(const std::string &fudingAddr) {}

uint64_t FantasyAgent::createInputsfromUTXO(const std::vector<utxoData>&vec,
                                        std::vector<std::string> &in_script,
                                        std::vector<std::string> &raw_transaction) {

//    std::string OP_DUP = "76";
//    std::string OP_HASH160 = "a9";
//    std::string OP_EQUALVERIFY = "88";
//    std::string OP_CHECKSIG = "ac";
//    std::string OP_RETURN = "6a";
//    std::string OP_EQUAL = "87";



    std::vector<std::string> fordebug;
    std::vector<std::string> fordebugscript;
//    std::vector<std::string> raw_transaction;
//    std::vector<std::string> in_script;
    int numinputs = 0;
    uint64_t inputsatoshis = 0;

    for ( auto v : vec ) {
        inputsatoshis += v.in_value;

        qDebug() << v.tx_hash << v.script <<  v.in_value << v.tx_output_n ;
        qDebug() << "";
        numinputs++;

        raw_transaction.push_back (v.tx_hash.toStdString());
        fordebug.push_back (v.tx_hash.toStdString() + "\n");
        auto reversetxoun = pb::toReverseHexFromDecimal (v.tx_output_n);
        raw_transaction.back () += reversetxoun;
        fordebug.back () += reversetxoun + "\n";
        auto ssize = ( unsigned char )( v.script.size( ) / 2 );
        auto sstr = pb::to_hex ( &ssize, sizeof( unsigned char ) );

//        if ( btcaddress[0] != P2SH_ADDRESS )
        if (true) {
            in_script.push_back( sstr + v.script.toStdString());
            fordebugscript.push_back (sstr + "\n");
            fordebugscript.back () += v.script.toStdString() + "\n";
        }
        else{
            qDebug() << " error - can only handle P2Pk";
//          pb::toRedeemScript(pubKey());
            std::string rdeemscript;
            rdeemscript = pb::to_hex(pubKey().begin(),33);
            {
            auto ssize = ( unsigned char )( rdeemscript.size( ) / 2 );
            auto sstr = pb::to_hex ( &ssize, sizeof( unsigned char ));
            }
            in_script.push_back(sstr + rdeemscript);

            /*
            std::string p2sh  = OP_HASH160;
            std::string scriptHash = pb::fromBtcAddress (btcaddress);
            {
                auto size = ( unsigned char )( scriptHash.size( ) / 2 );
                auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
                p2sh += sstr;
            }
            p2sh += scriptHash;
            p2sh += OP_EQUAL ;

            qDebug() << "in p2psh:" << p2sh.data ();

            {
                auto size = ( unsigned char )( p2sh.size( ) / 2 );
                auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
                rdeemscript = sstr;
                fordebugscript.push_back(sstr + "\n");
            }
            fordebugscript.back() += p2sh + "\n";
            rdeemscript += p2sh;
            in_script.push_back(rdeemscript);
            */
        }

    }
    return inputsatoshis;
}

std::string FantasyAgent::createTxFromInputs(uint64_t inputsatoshis,
                                             const std::string &fundaddress,
                                      std::vector<std::string> &in_script,
                                      std::vector<std::string> &raw_transaction) {
    std::string raw_transaction_out{""};
    std::vector<std::string> fordebug = raw_transaction;
    std::vector<std::string> fordebugscript = in_script;

    int numinputs = in_script.size();

    auto pk = m_priv.get_public_key().serialize();
    std::string fordebugout = "";
    uint64_t satoshifee = 25000;
    int numoutputs = 2;
    {
        auto size = ( unsigned char )numoutputs;
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        fordebugout += sstr + "\n";
        raw_transaction_out += sstr;
    }

    int64_t thamount = inputsatoshis - satoshifee;
    while ( thamount <= 0 ) {
        satoshifee /= 10;
        thamount =  inputsatoshis - satoshifee;
    }


    uint64_t amount = thamount;

    auto amstr = pb::toReverseHexFromDecimal(amount);
    raw_transaction_out += amstr;
    fordebugout += amstr + "\n";

    //OP_DUP OP_HASH160 <PubkeyHash> OP_EQUALVERIFY OP_CHECKSIG
    std::string OP_DUP = "76";
    std::string OP_HASH160 = "a9";
    std::string OP_EQUALVERIFY = "88";
    std::string OP_CHECKSIG = "ac";
    std::string OP_RETURN = "6a";
    std::string OP_EQUAL = "87";

    bool usep2sh = fundaddress.at(0) == P2SH_ADDRESS;
//    usep2sh = true;

    if ( !usep2sh ) {
        std::string p2pkh  = OP_DUP + OP_HASH160;
        std::string pubkeyHash = pb::fromBtcAddress (fundaddress);
        {
            auto size = ( unsigned char )( pubkeyHash.size( ) / 2 );
            auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
            p2pkh += sstr;
        }
        p2pkh += pubkeyHash;
        p2pkh += OP_EQUALVERIFY + OP_CHECKSIG;

        //    qDebug() << "sigscript:" << sigscript.data ();
        qDebug() << "out p2pkh:" << p2pkh.data ();

        {
            auto size = ( unsigned char )( p2pkh.size( ) / 2 );
            auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
            raw_transaction_out += sstr;
            fordebugout += sstr + "\n";
        }
        fordebugout += p2pkh + "\n";
        raw_transaction_out += p2pkh;
    }
    if ( usep2sh ) {

        std::string p2sh  = OP_HASH160;
        std::string scriptHash = pb::fromBtcAddress (fundaddress);
        {
            auto size = ( unsigned char )( scriptHash.size( ) / 2 );
            auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
            p2sh += sstr;
        }
        p2sh += scriptHash;
        p2sh += OP_EQUAL ;

        //    qDebug() << "sigscript:" << sigscript.data ();
        qDebug() << "p2psh:" << p2sh.data ();

        {
            auto size = ( unsigned char )( p2sh.size( ) / 2 );
            auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
            raw_transaction_out += sstr;
            fordebugout += sstr + "\n";
        }
        fordebugout += p2sh + "\n";
        raw_transaction_out += p2sh;
    }




    if ( numoutputs == 2) {
        //OP_RETURN
        std::string opret_out = OP_RETURN;
        std::string debug_opret_out = OP_RETURN + "\n";
        std::string hexname = pb::to_hex(client2->alias());
        {
            auto size = ( unsigned char )(hexname.size ()/2);
            auto holdsize = pb::to_hex ( &size, sizeof( unsigned char ) );
            opret_out  += holdsize;
            debug_opret_out += holdsize + "\n";
        }
        opret_out += hexname;
        debug_opret_out += hexname + "\n";
        {
            auto size = ( unsigned char )(opret_out.size() /2);
            auto holdsize = pb::to_hex ( &size, sizeof( unsigned char ) );
            opret_out  = holdsize + opret_out;
            debug_opret_out = holdsize + "\n" + debug_opret_out;
        }
        raw_transaction_out += "0000000000000000";
        raw_transaction_out += opret_out;
        fordebugout += "0000000000000000\n" + debug_opret_out;
    }

    std::string locktime = "00000000";
    fordebugout += locktime + "\n";
    raw_transaction_out += locktime;
//    raw_transaction_out += sighash_all;
//    qDebug() << "raw_transaction_out:" << raw_transaction_out.data ();


    std::string sighash_all = "01000000";
//    fordebugout += sighash_all + "\n";

    std::string  raw_transaction_pre = "01000000"; // i.e. version
    std::string fordebug_pre =  raw_transaction_pre + "\n";
    {
        auto size = ( unsigned char )numinputs;
        auto holdsize = pb::to_hex ( &size, sizeof( unsigned char ) );
        fordebug_pre += holdsize + "\n";
        raw_transaction_pre += holdsize;
    }
    //raw_transaction = raw_transaction;
    std::vector<std::string> to_sign;
    std::vector<std::string> to_sign_debug;

    for ( int i=0; i< numinputs; i++) {
        to_sign.push_back(raw_transaction_pre);
        to_sign_debug.push_back (fordebug_pre);
        for ( int j=0; j<numinputs; j++) {
            if ( j == i) {
                to_sign.back () += raw_transaction.at(i);
                to_sign_debug.back() += fordebug.at(i);
                to_sign.back() += in_script[i];
                to_sign_debug.back () += fordebugscript[i];
            }
            else {
                to_sign.back () += raw_transaction.at(j);
                to_sign_debug.back() += fordebug.at(j);
                to_sign.back() += "00";
                to_sign_debug.back () += "00\n";
            }
            to_sign_debug.back () +=  "ffffffff\n";
            to_sign.back() += "ffffffff";
        }
    }


//    fordebug += fordebugout;
//    qDebug() << fordebug.data();

     auto pvk = m_priv;//str2priv(secret.private_key ());

    //    auto sha = pb::hashit( "testit");
    //    auto sig = pvk.sign(sha);


    std::vector<std::string> sigouts;
    std::vector<std::string> debug_sigouts;
    for ( int i=0; i< to_sign.size(); i++) {
        to_sign.at(i) += raw_transaction_out;
        to_sign.at(i) += sighash_all;
        to_sign_debug.at(i) +=  fordebugout + sighash_all;

        qDebug() << i << "to_sign.at()";
        qDebug() << to_sign_debug.at(i).data();

        auto dblhash = pb::hashit(pb::hashfromhex(to_sign.at(i)));
        auto sig = pvk.sign(dblhash);
        auto dersig = pb::serialize_der (sig);
        dersig = pb::to_hex(dersig);
        auto debugdersig = dersig + "\n";
//        qDebug() << " dersig" << dersig.data ();

        dersig += "01";
        debugdersig += "01\n";
        auto size = ( unsigned char )( dersig.size( ) / 2 );
        auto sstr = pb::to_hex ( &size, sizeof( unsigned char ) );
        dersig = sstr + dersig;
        debugdersig = sstr + "\n" + debugdersig;

//        qDebug() << " size " << size << "dersig.size(  " << dersig.size( ) << " sstr " << sstr.data();

        dersig += "21";
        debugdersig += "21\n";
        sstr = pb::to_hex(pk.begin (),33);
        dersig.append (sstr);
        debugdersig.append (sstr + "\n");
        sigouts.push_back (dersig);//sig.data,64));
        debug_sigouts.push_back (debugdersig);
    }

//    qDebug() << sigouts[0].data ();

    std::string final_tx = raw_transaction_pre;
    std::string final_debug_tx = fordebug_pre;
    for ( int i=0; i< numinputs; i++) {
        final_tx += raw_transaction.at(i);
        final_debug_tx += fordebug.at(i);

        auto ssize = ( unsigned char )( sigouts.at(i).size( ) / 2 );
        auto sstr = pb::to_hex ( &ssize, sizeof( unsigned char ) );
//        qDebug() << " ssize " << ssize << "sigouts.at(i).size(  " << sigouts.at(i).size( ) << " sstr " << sstr.data();
        final_debug_tx += sstr + "\n" + debug_sigouts.at(i);
        final_tx += sstr + sigouts.at(i);
        final_tx += "ffffffff";
        final_debug_tx += "ffffffff\n";
    }

    final_tx += raw_transaction_out;
    final_debug_tx += fordebugout;
    qDebug() << "final tx" << final_tx.data ();
    qDebug() << "final_debug_tx";
    qDebug() << final_debug_tx.data ();


//    std::string testtx = "0100000001eccf7e3034189b851985d871f91384b8ee357cd47c3024736e5676eb2debb3f2010000001976a914010966776006953d5567439e5e39f86a0d273bee88acffffffff01605af405000000001976a914097072524438d003d23a2f23edb65aae1bb3e46988ac0000000001000000";
    auto testh = pb::hashit(pb::hashfromhex(final_tx));
    qDebug() << " test hash " << testh.reversestr();

    return final_tx;

}

string FantasyAgent::sign(const string &in) {
    return Commissioner::sig2str(m_priv.sign(pb::sha256(in)));
}

bool FantasyAgent::testIt(WalletD secret) {


    //qDebug() << OPENSSL_VERSION_NUMBER;
    auto mnpriv = fromMnemonic(secret.mnemonic_key());

    auto ret = secp256k1_ec_seckey_verify(pb::TheCTX::instance()->CTX(), mnpriv.data());

    string in = "sdfsfsdfs80999nbb b^&%#^  *(*& 65  \\00xx  *(&(54cV f" + secret.SerializeAsString();
    auto idsig1 = getRawIdSig(in,mnpriv);
    pb::secp256k1_privkey privateKey = str2priv(secret.private_key());
    auto idsig2 = getRawIdSig(in,privateKey);



//    auto pubk1 = mnpriv.get_public_key().serialize();
//    auto pubk2 = Commissioner::privStr2Pub(secret.private_key());

//    ret = secp256k1_ec_seckey_verify(pb::CTX,
//                                          reinterpret_cast<const unsigned char *>(secret.private_key().data()));
//    secp256k1_pubkey pubkey;
//    ret = secp256k1_ec_pubkey_create(pb::CTX, &pubkey,
//                                          reinterpret_cast<const unsigned char *>(secret.private_key().data()));

//    unsigned char pubkeyc[65];
//    size_t pubkeyclen = 65;
//    ret = secp256k1_ec_pubkey_serialize(pb::CTX, pubkeyc, &pubkeyclen, &pubkey, SECP256K1_EC_COMPRESSED);
//    memset(&pubkey, 0, sizeof(pubkey));
//    ret = secp256k1_ec_pubkey_parse(pb::CTX, &pubkey, pubkeyc, pubkeyclen);


//    unsigned char privkey[32];
//    unsigned char privkey2[32];
//    unsigned char privkeyc[300];
//    size_t prickeyclen = 300;
//    memcpy(privkey,secret.private_key().data(),32);

////    CHECK(ec_privkey_export_der(pb::CTX, privkeyc, &prickeyclen, privkey, 1) == 1);
////    CHECK(ec_privkey_import_der(pb::CTX, privkey2, privkeyc, prickeyclen) == 1);
////    CHECK(memcmp(privkey, privkey2, 32) == 0);

    auto pubk1 = mnpriv.get_public_key().serialize();

    auto priv2 = str2priv(secret.private_key());
    auto pubk2 = Commissioner::privStr2Pub(secret.private_key());
    auto pubk22 = priv2.get_public_key().serialize();

    auto pubk3 = privateKey.get_public_key().serialize();

    auto s1 = Commissioner::pk2str(pubk1);
    auto s2 = Commissioner::pk2str(pubk2);
    auto s3 = Commissioner::pk2str(pubk3);
    auto s4 = Commissioner::pk2str(pubk22);


    auto ss1 = privateKey.get_secret().str();
    auto s21 = mnpriv.get_secret().str();


//    std::string btc1 = pb::toBtcAddress (pubk1);

//    qDebug() << btc1.data ();
//    if ( pubk1 == pubk2 ) {
//        qDebug() << " == " << Commissioner::pk2str(pubk1) << Commissioner::pk2str(pubk2);
//    }
//    else
//        qDebug() << " != " << Commissioner::pk2str(pubk1) << Commissioner::pk2str(pubk2);

    for ( auto h : {idsig1.second, idsig2.second}) {
        for ( auto s : {idsig1.first, idsig2.first})
            for ( auto p : {pubk1, pubk2})

        if ( !Commissioner::verify(h,s,p) )
            return false;
    }

    return true;
}

std::multimap<std::string,std::string> FantasyAgent::getMyNames() {
    std::multimap<std::string,std::string> ret;
    for ( auto s2 : m_secrets ) {
        ret.emplace(s2.fantasy_name(),s2.public_key());
    }

    return ret;
}

std::vector<MyFantasyName> FantasyAgent::getMyNamesStatus(bool selectlast) {
    MyFantasyName selected;
    selected.set_status(MyNameStatus::none);
    std::vector<MyFantasyName> ret{};
    for ( auto s2 : m_secrets ) {
        MyFantasyName fn{};
        fn.set_name(s2.fantasy_name());
        auto fname = Commissioner::getName(Commissioner::str2pk(s2.public_key()));
        if ( fname == nullptr ) {
            if ( Commissioner::isAliasAvailable(s2.fantasy_name()))
                fn.set_status(MyNameStatus::requested);
            else
                fn.set_status(MyNameStatus::notavil);
        }
        else if ( fname->alias() == s2.fantasy_name() )
            fn.set_status(MyNameStatus::confirmed);
        else if ( FantasyName::name_hash(s2.fantasy_name()) == fname->hash() ) {
            fn.set_status(MyNameStatus::confirmed);
            fn.set_name(fname->alias());
        }
        else
            fn.set_status(MyNameStatus::none);

//        auto s = ret.find(fn.name());
//        if ( s != end(ret) ) {
//            if ( s->second.status() >= fn.status() )
//                continue;
//        }

        ret.push_back(fn);
        if (fn.status() >= selected.status()) {
            selected = fn;
        }
    }

    if ( selectlast && selected.name() != "") {
       UseName(selected.name());
    }
    return ret;
}

MyFantasyName FantasyAgent::getCurrentNamesStatus() {
    MyFantasyName fn{};
    fn.set_status(MyNameStatus::none);

    if ( !HaveClient()) return fn;

    fn.set_name(client2->alias());

    auto fname = Commissioner::getName(client2->pubkey());
    if ( fname == nullptr ) {
        if ( Commissioner::isAliasAvailable(fn.name()))
            fn.set_status(MyNameStatus::requested);
        else
            fn.set_status(MyNameStatus::notavil);
    }
    else if ( fname->alias() == fn.name() )
        fn.set_status(MyNameStatus::confirmed);
    else if ( FantasyName::name_hash(fn.name()) == fname->hash() ) {
        fn.set_status(MyNameStatus::confirmed);
        fn.set_name(fname->alias());
    }
    else
        fn.set_status(MyNameStatus::none);

    return fn;
}

void FantasyAgent::onSignedTransaction(SignedTransaction &sn)
{
    pendingTrans.emplace_back(sn);
//    qDebug() << sn.DebugString();
}

bool FantasyAgent::HaveClient() const {

    return client2 != nullptr;
}

/*!
 * \brief FantasyAgent::currentClient
 * \return
 * returns the current fantasyName of the end user.
 * if the "client" is not running it returns NOT GOOD
 */
std::string FantasyAgent::currentClient() {

    if ( !HaveClient() ) {
        return "NOT GOOD";
    }
    return client2->alias();
}


SignedTransaction FantasyAgent::makeSigned(Transaction &trans) {
   SignedTransaction st{};
    st.mutable_trans()->CopyFrom(trans);
    string strTrans = string(trans.SerializeAsString());
    std::pair<std::string, std::string> p = getIdSig(strTrans);
    st.set_id(p.first);
    st.set_sig(p.second);
    st.set_fantasy_name(client2->alias());
    return st;
}

SignedBlockHeader FantasyAgent::makeSigned(BlockHeader &bh) {
   SignedBlockHeader sbh{};
   sbh.mutable_head()->CopyFrom(bh);
    string strHeader = string(bh.SerializeAsString());
   std::pair<std::string, std::string> p = getIdSig(strHeader);
   sbh.set_sig(p.second);
//    qDebug() << p.first;
   return sbh;
}


std::string FantasyAgent::getSecret() const {
    if ( !HaveClient()) return "";
    return m_priv.get_secret().str();
}

pb::public_key_data FantasyAgent::pubKey() {
//    pb::public_key_data retpk;
//    size_t pubkeyclen = 33;
//    secp256k1_pubkey pubkey;
//    unsigned char pubkeyc[33];
//    auto ret = secp256k1_ec_pubkey_create(pb::CTX, &pubkey,
//                 reinterpret_cast<const unsigned char *>(m_priv.get_secret().data()));


//    ret = secp256k1_ec_pubkey_serialize(pb::CTX, pubkeyc, &pubkeyclen, &pubkey, SECP256K1_EC_COMPRESSED);

//    memcpy(retpk.key_data,pubkeyc,33);
//    return retpk;

    return m_priv.get_public_key().serialize();
//    return (*m_priv).get_public_key().serialize();
}

std::string FantasyAgent::pubKeyStr() {
//    qDebug() << "priv|" << m_priv.data() << "|" ;
    return (m_priv.data() != nullptr) ?
                Commissioner::pk2str(m_priv.get_public_key().serialize())
              : "";
}


std::pair<std::string, std::string> FantasyAgent::getIdSig(const std::string &in) {
    return (m_priv.data() != nullptr) ? getIdSig(in, m_priv)
                   :  std::make_pair("","");
}


std::pair<std::string, std::string>
FantasyAgent::getIdSig(const std::string &in,pb::secp256k1_privkey &pk) {
    auto sha = pb::hashit( in );
    return make_pair(sha.str(), Commissioner::sig2str(pk.sign(sha)));
}


std::pair<pb::sha256, secp256k1_ecdsa_signature>
FantasyAgent::getRawIdSig(const std::string &in,pb::secp256k1_privkey &pk) {
    const auto sha = pb::hashit( in );
    return make_pair(sha, pk.sign(sha));
}


pb::secp256k1_privkey FantasyAgent::str2priv(const std::string &in) {
//    int ret = secp256k1_ec_seckey_verify(pb::CTX, reinterpret_cast<const unsigned char *>(in.data()));
//    qDebug() << ret;
    return pb::secp256k1_privkey::regenerate(in);
}

std::string FantasyAgent::startImportMnemonic(std::string mn) {
    try {
        auto priv = fromMnemonic(mn);
        WalletD secret{};
        secret.set_private_key(priv.get_secret().str());
        secret.set_public_key(Commissioner::pk2str(priv.get_public_key().serialize()));
        secret.set_mnemonic_key(mn);

        m_pending[secret.public_key()] = secret;

        return secret.public_key();
    }
    catch (MnemonicException e) {
        qDebug() << e.what();
        return "";
    }
}

bool FantasyAgent::finishImportMnemonic(const std::string &pk,
                                               const std::string &name) {
    auto it = m_pending.find(pk);
    if ( it == end(m_pending) ) return false;

    WalletD sec = it->second;
    sec.set_fantasy_name(name);

    for ( auto s2 : m_secrets ) {
        if ( s2.public_key() == sec.public_key() &&
             s2.fantasy_name() == sec.fantasy_name() ) {
                m_pending.erase(it);
                return true;
        }
    }

    Writer<WalletD> writer{ GET_ROOT_DIR() + walldfilename, ios::app };
    writer(sec);
    m_secrets.push_back(sec);

    m_pending.erase(it);
    return true;
}

MyFantasyName FantasyAgent::UseMnemonic(std::string mn, bool store) {

    MyFantasyName mfn;
    mfn.set_status(MyNameStatus::none);
    mfn.set_name("");
    pb::secp256k1_privkey priv;
    try {
        priv = fromMnemonic(mn);
//        m_priv = priv;
    }
    catch (MnemonicException) {
        return mfn;
    }


    auto pk = priv.get_public_key().serialize();
    auto fn = Commissioner::getName(pk);

    if ( !fn ) {
        qDebug() << "name not found from mnemonic" << Commissioner::pk2str(pk).data();
        return mfn;
    }

    string name = fn->alias();
    qInfo() << "name found" << fn->ToString();

//    client = make_unique<FantasyName>(name, m_priv.get_public_key().serialize());
//    client2 = make_unique<FantasyNameCHash>(name, m_priv.get_public_key().serialize());
    mfn.set_status(MyNameStatus::confirmed);
    mfn.set_name(name);


    if ( UseName(name) ) //dont write if already have it
        return mfn;

    WalletD secret{};
    secret.set_private_key(priv.get_secret().str());
    secret.set_public_key(Commissioner::pk2str(pk));
    secret.set_fantasy_name(name);
    m_secrets.push_back(secret);

    if ( store ) {
        Writer<WalletD> writer{ GET_ROOT_DIR() + walldfilename, ios::app };
        secret.set_mnemonic_key(mn);
        writer(secret);
    }
    qInfo() << walldfilename.data ()<< "name available saving secret to file " << name.data ();

    UseName(name);
    return mfn;
}

bool FantasyAgent::UseName(const std::string &name) {
    for ( auto s : m_secrets ) {

        if ( s.fantasy_name() == name) {
            auto str = s.private_key();
            auto pr = str2priv(str);
            m_priv = pr;
            client2 = make_unique<FantasyNameCHash>
                    (name, m_priv.get_public_key().serialize());

            return true;
        }
    }

    return false;
}

std::string FantasyAgent::defaultName() {

    std::string name = "";
    for (int i = m_secrets.size(); i > 0; i--) {
        if ( m_secrets[i-1].fantasy_name() != "" ) {
            name = m_secrets[i-1].fantasy_name();
            auto str = m_secrets[i-1].private_key();
            auto pr = str2priv(str);
            m_priv = pr;
            client2 = make_unique<FantasyNameCHash>
                    (name, m_priv.get_public_key().serialize());

            return name;
        }
    }

    return name;
}


pair<pb::secp256k1_privkey,string> FantasyAgent::makePrivMnemonic() {
//    auto priv = pb::secp256k1_privkey::generate();

//    static int init = init_openssl();
    uint8_t buf[32];

    int result = RAND_bytes((unsigned char*)buf, 32);
    string m_mnemonic = createMnemonic(buf);
    return make_pair(fromMnemonic(m_mnemonic),m_mnemonic);
}

pb::secp256k1_privkey FantasyAgent::fromMnemonic(const string &in) {
    auto hseed = mnemonicToSeed(in);
    pb::sha256 secret;
    // 32 bytes is 256bits
    memcpy(secret.begin(), hseed.data(), 32);
    int ret = secp256k1_ec_seckey_verify(pb::TheCTX::instance()->CTX(), hseed.data());

    return  pb::secp256k1_privkey::regenerate(secret);
}

FantasyAgent::status FantasyAgent::signPlayer(std::string name) {
    status ret = NOTAVAILABLE;

    //if ( Commissioner::isAliasAvailable(name) )
    {
        for (auto fn : m_secrets) {
            if (fn.fantasy_name() == name) {
                auto pr = str2priv(fn.private_key());
                m_priv = pr;
                client2 = make_unique<FantasyNameCHash>(name, m_priv.get_public_key().serialize());

                ret = AVAIL;
                //LOG(lg, info) << "name available, already have it in secret file " << name;
                qInfo() <<"name available, already have it in secret file " << name;
            }
        }

        if (ret != AVAIL) {

            auto pm = makePrivMnemonic();
//            qDebug() << "makePrivMnemonic " << pm.second.data() << " priv | " << pm.first.data() << "|";

            m_priv = pm.first;
            auto m_mnemonic = pm.second;
//            client = make_unique<FantasyName>(name, m_priv.get_public_key().serialize());
                client2 = make_unique<FantasyNameCHash>(name, m_priv.get_public_key().serialize());

//            qDebug() << "pubKeyStr |" << pubKeyStr().data() << "|";


            Writer<WalletD> writer{ GET_ROOT_DIR() + walldfilename, ios::app };
//            qDebug() << writer.good() << "file name " << (GET_ROOT_DIR() + secretfilename3).data();
            WalletD secret{};
            secret.set_private_key(getSecret());
            secret.set_mnemonic_key(m_mnemonic);
            secret.set_public_key(pubKeyStr());
            secret.set_fantasy_name(name);
            writer(secret);
//            qDebug() << "\n"<< secret.DebugString ().data ();
            secret.clear_mnemonic_key();
            m_secrets.push_back(secret);

            //LOG(lg, info) << "name available saving secret to file " << name;
//            qDebug() << "name available saving secret to file " << name.data();

            ret = AVAIL;

        }

    }
    return ret;
}
