//
// ex: set ro:
// DO NOT EDIT.
// generated by smc (http://smc.sourceforge.net/)
// from file : CoinSale.sm
//

#include "CoinSale.h"
#include "CoinSale_sm.h"

using namespace statemap;

namespace fantasybit
{
    // Static class declarations.
    StartMap_Init StartMap::Init("StartMap::Init", 0);
    StartMap_NoName StartMap::NoName("StartMap::NoName", 1);
    StartMap_WaitingImportConfirm StartMap::WaitingImportConfirm("StartMap::WaitingImportConfirm", 2);
    StartMap_WaitingNameConfirm StartMap::WaitingNameConfirm("StartMap::WaitingNameConfirm", 3);
    StartMap_WaitingShowAddress StartMap::WaitingShowAddress("StartMap::WaitingShowAddress", 4);
    StartMap_WaitingForFunds StartMap::WaitingForFunds("StartMap::WaitingForFunds", 5);
    StartMap_VerifySecret StartMap::VerifySecret("StartMap::VerifySecret", 6);
    StartMap_WaitingConfirm StartMap::WaitingConfirm("StartMap::WaitingConfirm", 7);
    StartMap_WaitingExedos StartMap::WaitingExedos("StartMap::WaitingExedos", 8);
    StartMap_WaitingPacks StartMap::WaitingPacks("StartMap::WaitingPacks", 9);

    void CoinSaleState::Buy(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::Claim(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::ExedosReceived(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::Forgot(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::Funded(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::Import(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::NameConfimed(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::NameNotConfimed(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::PacksConfirmed(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::SecretOk(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::Select(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::ShowAddress(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::Verify(CoinSaleContext<CoinSale>& context)
    {
        Default(context);
    }

    void CoinSaleState::Verify(CoinSaleContext<CoinSale>& context, const QString & secret)
    {
        Default(context);
    }

    void CoinSaleState::Default(CoinSaleContext<CoinSale>& context)
    {
        throw (
            TransitionUndefinedException(
                context.getState().getName(),
                context.getTransition()));

    }

    void StartMap_Default::Forgot(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        CoinSaleState& endState = context.getState();

        context.clearState();
        try
        {
            ctxt.DisplaySecretDialog();
            context.setState(endState);
        }
        catch (...)
        {
            context.setState(endState);
            throw;
        }

    }

    void StartMap_Default::Buy(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::Import(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::Claim(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::Select(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::NameConfimed(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::SecretOk(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::ShowAddress(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::NameNotConfimed(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::Funded(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::Verify(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::ExedosReceived(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Default::PacksConfirmed(CoinSaleContext<CoinSale>& context)
    {


    }

    void StartMap_Init::Buy(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        if ( !ctxt.HasName() )
        {
            context.getState().Exit(context);
            context.clearState();
            try
            {
                ctxt.NewNameDialog();
                context.setState(StartMap::NoName);
            }
            catch (...)
            {
                context.setState(StartMap::NoName);
                throw;
            }
            context.getState().Entry(context);
        }
        else if ( !ctxt.IsSecretVerified() )
    
    {
            context.getState().Exit(context);
            // No actions.
            context.setState(StartMap::WaitingShowAddress);
            context.getState().Entry(context);
        }
        else
        {
            context.getState().Exit(context);
            context.setState(StartMap::WaitingForFunds);
            context.getState().Entry(context);
        }

    }

    void StartMap_NoName::Claim(CoinSaleContext<CoinSale>& context)
    {

        context.getState().Exit(context);
        context.setState(StartMap::WaitingNameConfirm);
        context.getState().Entry(context);

    }

    void StartMap_NoName::Import(CoinSaleContext<CoinSale>& context)
    {

        context.getState().Exit(context);
        context.setState(StartMap::WaitingImportConfirm);
        context.getState().Entry(context);

    }

    void StartMap_NoName::Select(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        if ( !ctxt.IsSecretVerified() )
        {
            context.getState().Exit(context);
            // No actions.
            context.setState(StartMap::WaitingShowAddress);
            context.getState().Entry(context);
        }
        else
        {
            context.getState().Exit(context);
            context.setState(StartMap::WaitingForFunds);
            context.getState().Entry(context);
        }

    }

    void StartMap_WaitingImportConfirm::NameConfimed(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        context.getState().Exit(context);
        context.clearState();
        try
        {
            ctxt.SetNameIsVerified();
            context.setState(StartMap::WaitingForFunds);
        }
        catch (...)
        {
            context.setState(StartMap::WaitingForFunds);
            throw;
        }
        context.getState().Entry(context);

    }

    void StartMap_WaitingImportConfirm::NameNotConfimed(CoinSaleContext<CoinSale>& context)
    {

        context.getState().Exit(context);
        context.setState(StartMap::NoName);
        context.getState().Entry(context);

    }

    void StartMap_WaitingNameConfirm::NameConfimed(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        if ( !ctxt.SecretDisplayed() )
        {
            CoinSaleState& endState = context.getState();

            context.clearState();
            try
            {
                ctxt.DisplaySecretDialog();
                context.setState(endState);
            }
            catch (...)
            {
                context.setState(endState);
                throw;
            }
        }
        else
        {
             StartMap_Default::NameConfimed(context);
        }

    }

    void StartMap_WaitingNameConfirm::NameNotConfimed(CoinSaleContext<CoinSale>& context)
    {

        context.getState().Exit(context);
        context.setState(StartMap::NoName);
        context.getState().Entry(context);

    }

    void StartMap_WaitingNameConfirm::SecretOk(CoinSaleContext<CoinSale>& context)
    {

        context.getState().Exit(context);
        context.setState(StartMap::WaitingShowAddress);
        context.getState().Entry(context);

    }

    void StartMap_WaitingShowAddress::Entry(CoinSaleContext<CoinSale>& context)

{
        CoinSale& ctxt = context.getOwner();

        ctxt.DisplayHiddenFundingAddress();
    }

    void StartMap_WaitingShowAddress::ShowAddress(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        if ( !ctxt.IsSecretVerified() )
        {
            context.getState().Exit(context);
            // No actions.
            context.setState(StartMap::VerifySecret);
            context.getState().Entry(context);
        }
        else
        {
            context.getState().Exit(context);
            context.setState(StartMap::WaitingForFunds);
            context.getState().Entry(context);
        }

    }

    void StartMap_WaitingForFunds::Entry(CoinSaleContext<CoinSale>& context)

{
        CoinSale& ctxt = context.getOwner();

        ctxt.DisplayFundingAddress();
        ctxt.StartCheckFundsTimer();
    }

    void StartMap_WaitingForFunds::Funded(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        if ( !ctxt.hasUTXO() )
        {
            // No actions.
        }
        else if ( !ctxt.isConfirmed() )
    
    {
            context.getState().Exit(context);
            context.clearState();
            try
            {
                ctxt.SignSendServer();
                context.setState(StartMap::WaitingConfirm);
            }
            catch (...)
            {
                context.setState(StartMap::WaitingConfirm);
                throw;
            }
            context.getState().Entry(context);
        }
        else
        {
            context.getState().Exit(context);
            context.clearState();
            try
            {
                ctxt.SignSendExedos();
                ctxt.StopCheckFundsTimer();
                context.setState(StartMap::WaitingExedos);
            }
            catch (...)
            {
                context.setState(StartMap::WaitingExedos);
                throw;
            }
            context.getState().Entry(context);
        }

    }

    void StartMap_VerifySecret::Entry(CoinSaleContext<CoinSale>& context)

{
        CoinSale& ctxt = context.getOwner();

        ctxt.VerifySecretDialog();
    }

    void StartMap_VerifySecret::Forgot(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        context.getState().Exit(context);
        context.clearState();
        try
        {
            ctxt.DisplaySecretDialog();
            context.setState(StartMap::WaitingNameConfirm);
        }
        catch (...)
        {
            context.setState(StartMap::WaitingNameConfirm);
            throw;
        }
        context.getState().Entry(context);

    }

    void StartMap_VerifySecret::Verify(CoinSaleContext<CoinSale>& context, const QString & secret)
    {
        CoinSale& ctxt = context.getOwner();

        if ( ctxt.DoVerifySecret(secret) )
        {
            context.getState().Exit(context);
            context.clearState();
            try
            {
                ctxt.SecretIsVerified();
                context.setState(StartMap::WaitingForFunds);
            }
            catch (...)
            {
                context.setState(StartMap::WaitingForFunds);
                throw;
            }
            context.getState().Entry(context);
        }
        else
        {
            CoinSaleState& endState = context.getState();

            context.clearState();
            try
            {
                ctxt.VerifyError();
                context.setState(endState);
            }
            catch (...)
            {
                context.setState(endState);
                throw;
            }
        }

    }

    void StartMap_WaitingConfirm::Funded(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        if ( !ctxt.isConfirmed() )
        {
            // No actions.
        }
        else
        {
            context.getState().Exit(context);
            context.clearState();
            try
            {
                ctxt.SignSendExedos();
                ctxt.StopCheckFundsTimer();
                context.setState(StartMap::WaitingExedos);
            }
            catch (...)
            {
                context.setState(StartMap::WaitingExedos);
                throw;
            }
            context.getState().Entry(context);
        }

    }

    void StartMap_WaitingExedos::Entry(CoinSaleContext<CoinSale>& context)

{
        CoinSale& ctxt = context.getOwner();

        ctxt.DisplayAddressBalance();
        ctxt.StartCheckExedosTimer();
    }

    void StartMap_WaitingExedos::Exit(CoinSaleContext<CoinSale>& context)

{
        CoinSale& ctxt = context.getOwner();

        ctxt.StopCheckExedosTimer();
    }

    void StartMap_WaitingExedos::ExedosReceived(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        if ( ctxt.isEqualFundedAmount() )
        {
            context.getState().Exit(context);
            context.clearState();
            try
            {
                ctxt.DisplayAddressBalance();
                context.setState(StartMap::WaitingPacks);
            }
            catch (...)
            {
                context.setState(StartMap::WaitingPacks);
                throw;
            }
            context.getState().Entry(context);
        }
        else
        {
            CoinSaleState& endState = context.getState();

            context.clearState();
            try
            {
                ctxt.DisplayAddressBalance();
                context.setState(endState);
            }
            catch (...)
            {
                context.setState(endState);
                throw;
            }
        }

    }

    void StartMap_WaitingPacks::Entry(CoinSaleContext<CoinSale>& context)

{
        CoinSale& ctxt = context.getOwner();

        ctxt.DisplayAddressPacksBalance();
        ctxt.StartCheckPacksTimer();
    }

    void StartMap_WaitingPacks::Exit(CoinSaleContext<CoinSale>& context)

{
        CoinSale& ctxt = context.getOwner();

        ctxt.StopCheckPacksTimer();
    }

    void StartMap_WaitingPacks::PacksConfirmed(CoinSaleContext<CoinSale>& context)
    {
        CoinSale& ctxt = context.getOwner();

        if ( !ctxt.isPacksEqualExedosAmount())
        {
            CoinSaleState& endState = context.getState();

            context.clearState();
            try
            {
                ctxt.RequestNewPacks();
                context.setState(endState);
            }
            catch (...)
            {
                context.setState(endState);
                throw;
            }
        }
        else
        {
            context.getState().Exit(context);
            context.clearState();
            try
            {
                ctxt.SetVerifySecret(ctxt.isNameNew());
                context.setState(StartMap::Init);
            }
            catch (...)
            {
                context.setState(StartMap::Init);
                throw;
            }
            context.getState().Entry(context);
        }

    }
}

//
// Local variables:
//  buffer-read-only: t
// End:
//
