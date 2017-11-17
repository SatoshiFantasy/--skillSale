//
// 11/17/2017  by: Jay Berg
// using StateMachine from SMC Compiler
//

#ifndef COINSALE_H
#define COINSALE_H

#include "CoinSale_sm.h"

namespace fantasybit {

class CoinSale : public CoinSaleContext<CoinSale> {

public:
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
    void xDoVerifySecret() {}
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

};

}

#endif
