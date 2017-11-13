//
//  Comissioner.h
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#ifndef __fantasybit__Comissioner__
#define __fantasybit__Comissioner__

#include "FantasyName.h"
#include <iostream>
#include <set>

#include "FantasyAgent.h"
#include "StateData.pb.h"
#include "ldbwriter.h"

namespace fantasybit {

//uint64_t difficulty( const fc::sha224& hash_value );

class Commissioner
{
    volatile bool running = true;
    static std::unordered_map<pb::public_key_data,std::shared_ptr<FantasyName>> FantasyNames;
    static std::map<hash_t,pb::public_key_data> Hash2Pk;


public:
    Commissioner() {	}

    static int DeveloperCheckpointHigh() {
#ifdef CHECKPOINT_2100
        return 4064;
#else
        return 4064;
#endif

    }

    static string DeveloperCheckPointId() {
        //return "2bf0172182bd7d425860cffe127507ef60f0675416d1382ca84988b0cbc68e29";
#ifdef CHECKPOINT_2100
        return "c64b5e7eb550b5b1bafc037f98074ec8e0807ca21c7712c71070209bad44cf97";
#else
        return "802debd372421418ab30b074133f49ddb663c44130dcb1876ebd2ad25cfc3091";
#endif
    }
    static Block getCheckPoint() {
        Block b;
        return b;
    }

    static std::recursive_mutex name_mutex;
    static pb::public_key_data MASTER_PUB_KEY;
    static pb::public_key_data GENESIS_PUB_KEY;
    static bool isAliasAvailable(alias_t alias) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        return Commissioner::Hash2Pk.find(FantasyName::name_hash(alias)) == end(Commissioner::Hash2Pk);
    }


    static void clearAll()
    {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        FantasyNames.clear();
        Hash2Pk.clear();
    }

    static std::vector<std::shared_ptr<FantasyName>> GetFantasyNames() {
        std::vector<std::shared_ptr<FantasyName>> names;

        for (auto p : FantasyNames)
            names.emplace_back(p.second);

        return names;
    }

    static std::shared_ptr<FantasyName> getName(const pb::public_key_data &pub  ) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        auto iter = Commissioner::FantasyNames.find(pub);
        if ( iter == end(Commissioner::FantasyNames) ) return nullptr;

        return iter->second;
    }

    static std::shared_ptr<FantasyName> getName(const hash_t &hash) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        auto iter = Hash2Pk.find(hash);
        if (iter == end(Hash2Pk)) return nullptr;

        return getName(iter->second);
    }

    static std::shared_ptr<FantasyName> getName(const alias_t &alias) {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        return getName(FantasyName::name_hash(alias));
    }


    static std::shared_ptr<FantasyName> AddName(const alias_t &alias, const std::string &pubkey)
    {
        auto pfn = makeName(alias,pubkey);
        {
            std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
            Commissioner::Hash2Pk.emplace(pfn->hash(), pfn->pubkey());
            Commissioner::FantasyNames.emplace(pfn->pubkey(), pfn);
        }
        return pfn;

    }

    static std::shared_ptr<FantasyName> makeName(const alias_t &alias, const std::string &pubkey)
    {
        auto pk  = str2pk(pubkey);
        auto pfn = std::make_shared< FantasyName >(alias,pk);
        return pfn;
    }

    static std::string FantasyAgentName() { return "FantasyAgent"; }

    static GlobalState InitialGlobalState () {
        GlobalState gs{};
        gs.set_season(2014);
        gs.set_state(GlobalState::OFFSEASON);
        gs.set_week(0);

        return gs;
    }

    static Bootstrap makeGenesisBoot(LdbWriter &ldb,string = "201700");
    static bool BootStrapFileExists(string);

    static const int BLOCK_VERSION = 1;
    static const int TRANS_VERSION = 2;
    static const int GENESIS_NUM = 1;


    static bool verify(const pb::signature &sig, const pb::sha256 &digest, const pb::public_key_data& pk) {
          return pb::public_key(pk).verify(digest, sig);// pb::public_key(sig, digest) == pub;
    }

    static bool verifyOracle(const pb::signature &sig, const pb::sha256 &digest)
    {
        return verify(sig, digest, GENESIS_PUB_KEY);
    }

    static bool verifyByName(const pb::signature &sig, const pb::sha256 &digest,const std::string &fn)
    {
        std::lock_guard<std::recursive_mutex> lockg{ name_mutex };
        auto iter = Hash2Pk.find(FantasyName::name_hash(fn));
        if (iter == Hash2Pk.end()) {
            qCritical() << "cant find fantasyname: " << fn.data();
            return false;
        }
        else {
            auto ret = verify(sig, digest, iter->second);
            if ( !ret )
                qCritical()  << "!verify(sig, digest, iter->second)";
            return ret;
        }
    }

    static std::string pk2str(const pb::public_key_data &pk)
    {
        return pb::to_base58(pk);
    }

    static pb::public_key_data privStr2Pub(const std::string &priv_key) {
        return pb::secp256k1_privkey::regenerate(priv_key).get_public_key().serialize();
    }

    static pb::public_key_data str2pk(const std::string &str)
    {
        pb::public_key_data pbb;
        pb::from_base58(str, (char *)pbb.key_data, 33);
        return pbb;
    }

    static std::string sig2str(const pb::signature &sig)
    {
        return pb::to_base58((char *)sig.data, 64);
    }

    static pb::signature str2sig(const std::string &str)
    {
        unsigned char data[72];
        pb::signature sig;
        if ( pb::from_base58(str, (char *)data, 64) > 64 ) {
            //Warning: still dealing with "bad" sigs from 2015
            auto sz = pb::from_base58(str, (char *)data, 72);
            sig = pb::parse_der(data,sz < 72 ? sz : 72);
            sig = pb::signature_normalize(sig);
        }
        else memcpy(sig.data,data,64);

        return sig;
    }

    static std::vector<std::string> GENESIS_NFL_TEAMS;
    static std::vector<std::string> GET_GENESIS_NFL_TEAMS() {
        return GENESIS_NFL_TEAMS;
    }

};

}

#endif /* defined(__fantasybit__Comissioner__) */

