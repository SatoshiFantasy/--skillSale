//
//  fbutils.h
//  fantasybit
//
//  Created by Jay Berg on 3/19/14.
//
//

#ifndef fantasybit_fbutils_h
#define fantasybit_fbutils_h

#include <cstdint>
#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <string>
#include <unordered_map>
#include "platform.h"
#include <sstream>
#include "ProtoData.pb.h"

#define QTD QString::fromStdString


namespace fantasybit {

#if !defined(PRODFOOTBALL)

#ifdef USE_LOCALNETWORKHOST_SERVER
    static std::string PB_WS_LITE_AGENT = "192.168.42.80";
    static std::string PB_WS_CHAT = "192.168.42.80";
    static std::string PB_WS_TX = "192.168.42.80";
#elif USE_LOCALHOST_SERVER
    static std::string PB_WS_LITE_AGENT = "localhost";
    static std::string PB_WS_CHAT = "localhost";
    static std::string PB_WS_TX = "localhost";
#else
    static std::string PB_WS_LITE_AGENT = "app.trading.football";
    static std::string PB_WS_CHAT = "app.trading.football";
    static std::string PB_WS_TX = "app.trading.football";
#endif
    static int PB_WS_LITE_AGENT_PORT = 9111;

    static std::string PAPIURL = "https://stagingapi.trading.football:4545";
    static std::string LAPIURL = "https://stagingapi.trading.football:9854";
    static bool IS_TEST_NET = true;
#else
    static bool IS_TEST_NET = false;
    static int PB_WS_LITE_AGENT_PORT = 9111;
    static int PB_WS_TX_PORT = 9110;
    static int PB_WS_CHAT_PORT = 9112;


#ifdef USE_LOCALNETWORKHOST_SERVER
    static std::string PB_WS_LITE_AGENT = "192.168.42.80";
    static std::string PB_WS_CHAT = "192.168.42.80";
    static std::string PB_WS_TX = "192.168.42.80";
#elif USE_LOCALHOST_SERVER
    static std::string PB_WS_LITE_AGENT = "localhost";
    static std::string PB_WS_CHAT = "localhost";
    static std::string PB_WS_TX = "localhost";
#else
    static std::string PB_WS_LITE_AGENT = "app.trading.football";
    static std::string PB_WS_CHAT = "app.trading.football";
    static std::string PB_WS_TX = "app.trading.football";
#endif

    static std::string PAPIURL = "https://app.trading.football:4545";
    static std::string LAPIURL = "https://app.trading.football:9854";
#endif


    static std::string FBSALE_LOCALNET = "192.168.42.80";
    static std::string FBSALE_LOCALHOST = "localhost";
    static std::string FBSALE_HOST = PB_WS_LITE_AGENT;
    static int FBSALE_AGENT_PORT = 9222;

#ifndef USE_BTC_MAINNET
    static std::string BLOCKCHAINAPI = "https://testnet.blockchain.info";
    static std::string CHAINSOAPI = "https://chain.so/api/v2/%1/BTCTEST";
    static unsigned char BTC_NETWORK = 0x6f;//0xc4;//0x6f; //0
    static char P2SH_ADDRESS = '2';
    static std::string FUNDING_ADDRESS =
    //            "mpDr2HtBxfvjngf3wt8d76TTRoAMtybYtC";
                                             "2N8hwP1WmJrFF5QWABn38y63uYLhnJYJYTF";
#else
    static std::string BLOCKCHAINAPI = "https://blockchain.info";
    static unsigned char BTC_NETWORK = 0;//0xc4;//0x6f; //0
    static char P2SH_ADDRESS = '3';
    static std::string FUNDING_ADDRESS = "";
#endif

    //forwards
    struct FantasyName;

    using Int = int;
    using Uid = std::string;// uint_least64_t;
    using UInt64 = uint64_t;
    using Int64 = int64_t;

    using MapS2I = std::map < std::string, Int > ;
    template<class T>
    using NameValuePairs =
        std::unordered_map < std::string, T > ;
    //	std::vector<std::pair<std::shared_ptr<FantasyName>, T>>;

    using Signature = std::string;

    template <typename T>
    std::string To_String(T val)
    {
        std::stringstream stream;
        stream << val;
        return stream.str();
    }


    static std::string GET_ROOT_DIR() {
        return Platform::instance()->getRootDir();
    }

    static std::string FB_PORT(int port) {
        return std::string(":").append( To_String( port ) );
    }

    template < typename T >
    std::map<int, std::string> proto_enum_map() {
        std::map<int, std::string> ret;

        for (int i = T::Type_MIN; i < T::Type_ARRAYSIZE; i++) {

            if (!T::Type_IsValid(i)) continue;
            ret[i] = T::Type_Name(T::Type(i));
        }

        return ret;
    }


    struct PlayerDetail {
        PlayerBase base;
        PlayerStatus::Status team_status;
        PlayerGameStatus game_status;
        std::string symbol;
        std::string team;
    };

    struct GameRoster {
        GameInfo info;
        GameStatus::Status  status;
        std::unordered_map<std::string,PlayerDetail> homeroster;
        std::unordered_map<std::string,PlayerDetail> awayroster;
    };


}

#endif
