/*
ACCPO - Autonomous CryptoCurrency Portfolio Optimization
Copyright (C) - 2020 - Dr. Alexander M. Beck - dralmabeck@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "accpo.h"

void accpo() noexcept
{
    std::shared_ptr<Kraken> K = std::make_shared<Kraken>(CF.apiKey, CF.secKey);

    K->checkTimeStatus();

    std::unique_ptr<Account> A = std::make_unique<Account>(K);
    std::cout << A->printAccount().str();

    //K->fetchAllTickers(); Not needed, we fetched and buffered tickers in Account constructor

    std::vector<std::unique_ptr<Asset>> assetVector;
    for (size_t ii(0); ii < A->assets.size(); ii++)
    {
        if (A->active[ii] == true)
        {
            std::unique_ptr<Asset> newAsset = std::make_unique<Asset>(A->assets[ii], A->tickers[ii], A->minimums[ii], K);
            //newAsset->setHistoricData();
            newAsset->setCurrentData(CF.INI);
            //newAsset->setHistoricQuantity(A->quants[ii], 0);
            newAsset->setCurrentQuantity(A->quants[ii], 0);
            newAsset->copyCurrentObject(CF.INI, CF.BAL);
            assetVector.push_back(std::move(newAsset));
        }
    }

    std::shared_ptr<Portfolio> P = std::make_shared<Portfolio>();
    for (auto &asset : assetVector)
    {
        P->addAsset(std::move(asset));
        asset = nullptr;
    }

    std::unique_ptr<Optimizer> O = std::make_unique<Optimizer>(P, K);

    //std::cout << P->printHistoric(0, P->numberTimebins() - 1).str();
    //O->calculateHistoric();
    //std::cout << P->printHistoric(0, P->numberTimebins() - 1).str();

    O->propagateCurrent();

    std::cout << P->printCurrentState(CF.INI).str();
    std::cout << P->printCurrentTrades(CF.INI, CF.BAL).str();
    std::cout << P->printCurrentState(CF.BAL).str();

    /*Trade newTrade;
    newTrade.ticker = "XTZEUR";
    newTrade.quantity = +10.0;
    newTrade.price = 1.8743;
    newTrade.market = "limit";
    newTrade.limit = 1.5;
    K->addTradePipeline(newTrade);*/

    K->processTradePipeline();

    A->updateAccount();
    std::cout << A->printAccount().str();
}

auto main() -> int
{
    auto progStart(std::chrono::high_resolution_clock::now());

    std::cout << "===================================================================================================================" << std::endl;
    std::cout << "======== Autonomous Autonomous ====================================================================================" << std::endl;
    std::cout << "============================== CryptoCurrency CryptoCurrency ======================================================" << std::endl;
    std::cout << "============================================================ Portfolio Portfolio ==================================" << std::endl;
    std::cout << "================================================================================ Optimization Optimization ========" << std::endl;
    std::cout << "======== Version: 0.2 =============================================================================================" << std::endl;
    std::cout << "===================================================================================================================" << std::endl;

    std::thread accpoThread = std::thread(accpo);

    std::system("read -n 1");
    CF.terminateProgram.store(true);

    std::cout << "===================================================================================================================" << std::endl;
    std::cout << "Keystroke detected." << std::endl;
    std::cout << "Preparing to terminate execution and stopping program." << std::endl;

    accpoThread.join();

    std::cout << "===================================================================================================================" << std::endl;
    std::cout << "Shutting down Autonomous CryptoCurrency Portfolio Optimization program." << std::endl;
    std::cout << "===================================================================================================================" << std::endl;

    auto progEnd(std::chrono::high_resolution_clock::now());
    auto progTime(static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(progEnd - progStart).count()) / 60.0);

    std::cout << "Program execution lasted for: " << num2str(progTime, 2) << " minutes." << std::endl;

    return 0;
}
