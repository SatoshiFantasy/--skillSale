//
//  Comissioner.cpp
//  fantasybit
//
//  Created by Jay Berg on 4/1/14.
//
//

#include "Commissioner.h"
#include "FantasyName.h"
#include <iostream>
#include <ctime>
#include "DataPersist.h"
#include "platform.h"
#include "appsettings.h"
#include "ldbwriter.h"
#include "qurl.h"
#include <RestfullClient.h>

using namespace std;

namespace fantasybit {

pb::public_key_data Commissioner::GENESIS_PUB_KEY
{
#ifndef PRODFOOTBALL
      Commissioner::str2pk(std::string("mT1M2MeDjA1RsWkwT7cjE6bbjprcNi84cWyWNvWU1iBa"))
#else
      Commissioner::str2pk(std::string("00000000000000000000000000000000000000000000"))
#endif
};



std::unordered_map<pb::public_key_data,std::shared_ptr<FantasyName>> Commissioner::FantasyNames{};
std::map<hash_t,pb::public_key_data> Commissioner::Hash2Pk{};


std::recursive_mutex Commissioner::name_mutex{};

decltype(Commissioner::GENESIS_NFL_TEAMS) Commissioner::GENESIS_NFL_TEAMS {
    "ARI" ,
    "ATL" ,
    "BAL" ,
    "BUF" ,
    "CAR" ,
    "CHI" ,
    "CIN" ,
    "CLE" ,
    "DAL" ,
    "DEN" ,
    "DET" ,
    "GB" ,
    "HOU" ,
    "IND" ,
    "JAC" ,
    "KC" ,
    "MIA" ,
    "MIN" ,
    "NE" ,
    "NO" ,
    "NYG" ,
    "NYJ" ,
    "OAK" ,
    "PHI" ,
    "PIT" ,
    "LAC" ,
    "SEA" ,
    "SF" ,
    "LAR" ,
    "TB" ,
    "TEN" ,
    "WAS"
};

}
