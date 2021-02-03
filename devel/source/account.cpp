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

#include "account.h"

Account::Account(const std::shared_ptr<Kraken>& K) noexcept : K(std::move(K))
{
    updateAccount();
    pvInitialAll = std::inner_product(quants.begin(), quants.end(), prices.begin(), 0.0f);
    std::cout << "Account constructor() executed." << std::endl;
}

auto Account::pvAll() const noexcept -> double
{
    return static_cast<double>(std::inner_product(quants.begin(), quants.end(), prices.begin(), 0.0f));
}

void Account::updateAccount() noexcept
{
    K->fetchAccountBalance(*this);
    K->fetchAllTickers();

    for (size_t ii(0); ii < assets.size(); ii++)
    {
        for (size_t jj(0); jj<CF.tickerList.size(); jj++)
        {
            std::string ticker(CF.tickerList[jj]);
            double minimum(CF.tickerMinimum[jj]);
            std::string assetName(assets[ii]);

            size_t tickerPosition(ticker.find(assetName));
            if (tickerPosition == 0 || tickerPosition == 1)
            {
                tickers[ii] = ticker;
                minimums[ii] = minimum;
                break;
            }

            assetName.erase(0, 1);
            tickerPosition = ticker.find(assetName);
            if (tickerPosition == 0)
            {
                tickers[ii] = ticker;
                minimums[ii] = minimum;
                break;
            }
        }

        K->extractTickerPrice(prices[ii], tickers[ii]);

        for (const auto &tradeAsset : CF.tradeableAssets)
        {
            std::string assetName(assets[ii]);
            size_t tickerPosition(tradeAsset.find(assetName));
            if (tickerPosition == 0 || tickerPosition == 1)
            {
                active[ii] = true;
                break;
            }

            assetName.erase(0, 1);
            tickerPosition = tradeAsset.find(assetName);
            if (tickerPosition == 0)
            {
                active[ii] = true;
                break;
            }
        }
    }

    std::cout << "Accout updateAccount() executed." << std::endl;
}

auto Account::printAccount() noexcept -> std::stringstream
{
    std::stringstream result;

    result << "===================================================================================================================" << std::endl;
    result << "Account summary:" << std::endl;
    result << "Last update at time: " << time2str(lastQueryTime) << std::endl;
    result << "Total number of assets:        " << std::to_string(active.size()) << std::endl;
    result << "Tradeable number of assets:    " << std::to_string(std::count_if(active.begin(), active.end(), [](bool x) { return x == true; })) << std::endl;

    for (size_t ii(0); ii < assets.size(); ii++)
    {
        if (assets[ii].size() == 3)
        {
            result << "Asset:  " << assets[ii] << " - ";
        }
        else
        {
            result << "Asset: " << assets[ii] << " - ";
        }

        if (tickers[ii].size() == 6)
        {
            result << "Ticker:   " << tickers[ii] << " - ";
        }
        else if (tickers[ii].size() == 7)
        {
            result << "Ticker:  " << tickers[ii] << " - ";
        }
        else
        {
            result << "Ticker: " << tickers[ii] << " - ";
        }

        result << "Value: " << num2str(quants[ii]*prices[ii], 8) << " - ";
        result << "Quantity: " << num2str(quants[ii], 8) << " - ";
        result << "Price:    " << num2str(prices[ii], 4) << " - ";
        result << "Minimum: " << num2str(minimums[ii], 4) << " - ";
        result << "Min Lot: " << num2str(minimums[ii]*prices[ii], 2) << " - ";
        result << "Tradeable: " << bool2str(active[ii]) << std::endl;
    }

    result << "Portfolio value total:        " << num2str(pvAll(), 2) << std::endl;
    result << "Initial PV total:             " << num2str(pvInitialAll, 2) << std::endl;
    result << "===================================================================================================================" << std::endl;

    return result;
}