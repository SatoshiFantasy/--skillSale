//
// ex: set ro:
// DO NOT EDIT.
// generated by smc (http://smc.sourceforge.net/)
// from file : CoinSale.sm
//

#ifndef COINSALE_SM_H
#define COINSALE_SM_H


#define SMC_USES_IOSTREAMS

#include <statemap.h>

namespace fantasybit
{
    // Forward declarations.
    class StartMap;
    class StartMap_Init;
    class StartMap_NoName;
    class StartMap_WaitingImportConfirm;
    class StartMap_WaitingNameConfirm;
    class StartMap_WaitingShowAddress;
    class StartMap_WaitingForFunds;
    class StartMap_VerifySecret;
    class StartMap_WaitingConfirm;
    class StartMap_WaitingExedos;
    class StartMap_Default;
    class CoinSaleState;
    template<typename DERIVED> class CoinSaleContext;
    class CoinSale;

    class CoinSaleState :
        public statemap::State
    {
    public:

        CoinSaleState(const char * const name, const int stateId)
        : statemap::State(name, stateId)
        {};

        virtual void Entry(CoinSaleContext<CoinSale>&) {};
        virtual void Exit(CoinSaleContext<CoinSale>&) {};

        virtual void Buy(CoinSaleContext<CoinSale>& context);
        virtual void Claim(CoinSaleContext<CoinSale>& context);
        virtual void ExedosReceived(CoinSaleContext<CoinSale>& context);
        virtual void Forgot(CoinSaleContext<CoinSale>& context);
        virtual void Funded(CoinSaleContext<CoinSale>& context);
        virtual void Import(CoinSaleContext<CoinSale>& context);
        virtual void NameConfimed(CoinSaleContext<CoinSale>& context);
        virtual void NameNotConfimed(CoinSaleContext<CoinSale>& context);
        virtual void PacksConfirmed(CoinSaleContext<CoinSale>& context);
        virtual void SecretOk(CoinSaleContext<CoinSale>& context);
        virtual void Select(CoinSaleContext<CoinSale>& context);
        virtual void ShowAddress(CoinSaleContext<CoinSale>& context);
        virtual void Verify(CoinSaleContext<CoinSale>& context);
        virtual void Verify(CoinSaleContext<CoinSale>& context, const QString & secret);

    protected:

        virtual void Default(CoinSaleContext<CoinSale>& context);
    };

    class StartMap
    {
    public:

        static StartMap_Init Init;
        static StartMap_NoName NoName;
        static StartMap_WaitingImportConfirm WaitingImportConfirm;
        static StartMap_WaitingNameConfirm WaitingNameConfirm;
        static StartMap_WaitingShowAddress WaitingShowAddress;
        static StartMap_WaitingForFunds WaitingForFunds;
        static StartMap_VerifySecret VerifySecret;
        static StartMap_WaitingConfirm WaitingConfirm;
        static StartMap_WaitingExedos WaitingExedos;
    };

    class StartMap_Default :
        public CoinSaleState
    {
    public:

        StartMap_Default(const char * const name, const int stateId)
        : CoinSaleState(name, stateId)
        {};

        virtual void Forgot(CoinSaleContext<CoinSale>& context);
        virtual void Buy(CoinSaleContext<CoinSale>& context);
        virtual void Import(CoinSaleContext<CoinSale>& context);
        virtual void Claim(CoinSaleContext<CoinSale>& context);
        virtual void Select(CoinSaleContext<CoinSale>& context);
        virtual void NameConfimed(CoinSaleContext<CoinSale>& context);
        virtual void SecretOk(CoinSaleContext<CoinSale>& context);
        virtual void ShowAddress(CoinSaleContext<CoinSale>& context);
        virtual void NameNotConfimed(CoinSaleContext<CoinSale>& context);
        virtual void Funded(CoinSaleContext<CoinSale>& context);
        virtual void Verify(CoinSaleContext<CoinSale>& context);
        virtual void ExedosReceived(CoinSaleContext<CoinSale>& context);
        virtual void PacksConfirmed(CoinSaleContext<CoinSale>& context);
    };

    class StartMap_Init :
        public StartMap_Default
    {
    public:
        StartMap_Init(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void Buy(CoinSaleContext<CoinSale>& context);
    };

    class StartMap_NoName :
        public StartMap_Default
    {
    public:
        StartMap_NoName(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void Claim(CoinSaleContext<CoinSale>& context);
        virtual void Import(CoinSaleContext<CoinSale>& context);
        virtual void Select(CoinSaleContext<CoinSale>& context);
    };

    class StartMap_WaitingImportConfirm :
        public StartMap_Default
    {
    public:
        StartMap_WaitingImportConfirm(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void NameConfimed(CoinSaleContext<CoinSale>& context);
        virtual void NameNotConfimed(CoinSaleContext<CoinSale>& context);
    };

    class StartMap_WaitingNameConfirm :
        public StartMap_Default
    {
    public:
        StartMap_WaitingNameConfirm(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void NameConfimed(CoinSaleContext<CoinSale>& context);
        virtual void NameNotConfimed(CoinSaleContext<CoinSale>& context);
        virtual void SecretOk(CoinSaleContext<CoinSale>& context);
    };

    class StartMap_WaitingShowAddress :
        public StartMap_Default
    {
    public:
        StartMap_WaitingShowAddress(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void Entry(CoinSaleContext<CoinSale>&);
        virtual void ShowAddress(CoinSaleContext<CoinSale>& context);
    };

    class StartMap_WaitingForFunds :
        public StartMap_Default
    {
    public:
        StartMap_WaitingForFunds(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void Entry(CoinSaleContext<CoinSale>&);
        virtual void Funded(CoinSaleContext<CoinSale>& context);
    };

    class StartMap_VerifySecret :
        public StartMap_Default
    {
    public:
        StartMap_VerifySecret(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void Entry(CoinSaleContext<CoinSale>&);
        virtual void Forgot(CoinSaleContext<CoinSale>& context);
        virtual void Verify(CoinSaleContext<CoinSale>& context, const QString & secret);
    };

    class StartMap_WaitingConfirm :
        public StartMap_Default
    {
    public:
        StartMap_WaitingConfirm(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void Funded(CoinSaleContext<CoinSale>& context);
    };

    class StartMap_WaitingExedos :
        public StartMap_Default
    {
    public:
        StartMap_WaitingExedos(const char * const name, const int stateId)
        : StartMap_Default(name, stateId)
        {};

        virtual void Entry(CoinSaleContext<CoinSale>&);
        virtual void ExedosReceived(CoinSaleContext<CoinSale>& context);
    };

    template<typename DERIVED>
class CoinSaleContext :
        public statemap::FSMContext
    {
    public:

        explicit CoinSaleContext()
        : FSMContext(StartMap::Init)
        {};

        CoinSaleContext(const statemap::State& state)
        : FSMContext(state)
        {};

        virtual void enterStartState()
        {
            getState().Entry(*this);
            return;
        }

        inline CoinSale& getOwner()
        {
            return (*static_cast<DERIVED*>(this));
        };

        inline CoinSaleState& getState()
        {
            if (_state == NULL)
            {
                throw statemap::StateUndefinedException();
            }

            return dynamic_cast<CoinSaleState&>(*_state);
        };

        inline void Buy()
        {
            getState().Buy(*this);
        };

        inline void Claim()
        {
            getState().Claim(*this);
        };

        inline void ExedosReceived()
        {
            getState().ExedosReceived(*this);
        };

        inline void Forgot()
        {
            getState().Forgot(*this);
        };

        inline void Funded()
        {
            getState().Funded(*this);
        };

        inline void Import()
        {
            getState().Import(*this);
        };

        inline void NameConfimed()
        {
            getState().NameConfimed(*this);
        };

        inline void NameNotConfimed()
        {
            getState().NameNotConfimed(*this);
        };

        inline void PacksConfirmed()
        {
            getState().PacksConfirmed(*this);
        };

        inline void SecretOk()
        {
            getState().SecretOk(*this);
        };

        inline void Select()
        {
            getState().Select(*this);
        };

        inline void ShowAddress()
        {
            getState().ShowAddress(*this);
        };

        inline void Verify()
        {
            getState().Verify(*this);
        };

        inline void Verify(const QString & secret)
        {
            getState().Verify(*this, secret);
        };
    };
}


#endif // COINSALE_SM_H

//
// Local variables:
//  buffer-read-only: t
// End:
//
