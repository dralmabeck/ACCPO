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

#include "kraken.h"

void Kraken::fetchAllTXID() noexcept
{
    std::string txIDList;
    for (auto &tradeObject : tradePipeline)
    {
        if (tradeObject.txid.size() == CF.lengthTxID)
        {
            txIDList += tradeObject.txid + ",";
        }
        else
        {
            std::cout << "Kraken INVALID TXID detected for " << tradeObject.ticker << " with TXID " << tradeObject.txid << std::endl;
        }
        
    }
    txIDList.pop_back();

    krakenAPI->priv_func->query_order_info(&krakenAPI, txIDList.c_str());
    txidBuffer = json::parse(krakenAPI->s_result);

    std::cout << "Kraken txIDBuffer contains now: " << txidBuffer.dump() << std::endl;
    std::cout << "Kraken fetchAllTXID() executed for: " << txIDList << std::endl;
    pauseApi();
}

void Kraken::processTradePipeline() noexcept
{
    std::cout << "===================================================================================================================" << std::endl;
    std::cout << "Kraken trade pipeline size " << std::to_string(tradePipeline.size()) << std::endl;

    std::sort(tradePipeline.begin(), tradePipeline.end(), compareTradePipeline);

    for (auto const &tradeObject : tradePipeline)
    {
        std::cout << "Kraken ";
        if (tradeObject.ticker.size() == 6)
        {
            std::cout << "  ";
        }
        std::cout << tradeObject.ticker << " with quantity " << num2str(tradeObject.quantity, 8);
        std::cout << " and execution status " << bool2str(tradeObject.executed) << " and PV ";
        std::cout << num2str(tradeObject.price * tradeObject.quantity, 8);
        std::cout << " and limit " << num2str(tradeObject.limit, 8);
        std::cout << " and type " << tradeObject.market << std::endl;
    }

    for (auto &tradeObject : tradePipeline)
    {
        placeOrder(tradeObject);
    }

    fetchAllTXID();

    for (auto &tradeObject : tradePipeline)
    {
        if (tradeObject.txid.size() == CF.lengthTxID)
        {
            std::string orderStatus(txidBuffer["result"][tradeObject.txid]["status"]);
            std::cout << "Kraken TXID " << tradeObject.txid << " with status " << orderStatus << std::endl;
            if (orderStatus == "closed")
            {
                tradeObject.executed = true;
            }
        }
        else
        {
            std::cout << "Kraken INVALID TXID detected for " << tradeObject.ticker << " with TXID " << tradeObject.txid << std::endl;
        }
        
    }
            
    for (auto const &tradeObject : tradePipeline)
    {
        std::cout << "Kraken ";
        if (tradeObject.ticker.size() == 6)
        {
            std::cout << "  ";
        }
        std::cout << tradeObject.ticker << " with quantity " << num2str(tradeObject.quantity, 8);
        std::cout << " and execution status " << bool2str(tradeObject.executed) << " and PV ";
        std::cout << num2str(tradeObject.price * tradeObject.quantity, 8);
        std::cout << " and limit " << num2str(tradeObject.limit, 8);
        std::cout << " and type " << tradeObject.market << std::endl;
    }
    std::cout << "Kraken trade pipeline size " << std::to_string(tradePipeline.size()) << std::endl;
    std::cout << "===================================================================================================================" << std::endl;
    std::cout << "Kraken executeTradePipeline() executed." << std::endl;
}

void Kraken::placeOrder(Trade &tradeObject) noexcept
{
    std::string orderType, orderMarket, orderTicker, orderQuantity, orderLimit;
    if (tradeObject.quantity > 0)
    {
        orderType = "buy";
        orderQuantity = num2str(fabs(tradeObject.quantity - 0.00000001), 8);
    }
    else
    {
        orderType = "sell";
        orderQuantity = num2str(fabs(tradeObject.quantity + 0.00000001), 8);
    }
    
    orderMarket = tradeObject.market;
    orderTicker = tradeObject.ticker;
    if (orderMarket == "limit")
    {
        orderLimit = num2str(tradeObject.limit, 8);
        krakenAPI->priv_func->add_order(&krakenAPI, orderType.c_str(), orderMarket.c_str(), orderTicker.c_str(), orderQuantity.c_str(), orderLimit.c_str());
    }
    else
    {
        krakenAPI->priv_func->add_order(&krakenAPI, orderType.c_str(), orderMarket.c_str(), orderTicker.c_str(), orderQuantity.c_str());
    }

    std::string result(krakenAPI->s_result);
    json orderStatus = json::parse(result);

    tradeObject.txid = orderStatus["result"]["txid"][0].dump();
    tradeObject.txid.pop_back();
    tradeObject.txid.erase(0, 1);

    std::cout << "Kraken TXID -- " << tradeObject.txid << " -- placed of " << orderStatus["result"]["descr"]["order"].dump() << std::endl;
    
    if (tradeObject.txid.size() == CF.lengthTxID)
    {
        tradeObject.placed = true;
    }

    std::cout << "Kraken placeOrder() executed for: " << tradeObject.ticker << std::endl;
    std::cout << result << std::endl;
    pauseApi();
}

void Kraken::addTradePipeline(const Trade &tradeObject) noexcept
{
    tradePipeline.emplace_back(tradeObject);

    std::cout << "Kraken trade pipeline addition: ";
    if (tradeObject.ticker.size() == 6)
    {
        std::cout << "  ";
    }
    std::cout << tradeObject.ticker << " Quantity: " << num2str(tradeObject.quantity, 8);
    std::cout << " Execution status: " << bool2str(tradeObject.executed) << std::endl;
}

void Kraken::fetchAccountBalance(Account &data) noexcept
{
    krakenAPI->priv_func->get_account_balance(&krakenAPI);
    json result = json::parse(krakenAPI->s_result);
    json account(result["result"]);

    data.assets.clear();
    data.quants.clear();

    for (auto slice : account.items())
    {
        std::string asset(static_cast<std::string>(slice.key().data()));
        data.assets.emplace_back(asset);
    }

    std::sort(data.assets.begin(), data.assets.end(), std::greater<std::string>());

    for (const auto &asset : data.assets)
    {
        for (auto &[key, value] : account.items())
        {
            if (key == asset)
            {
                std::string quantity(static_cast<std::string>(value));
                data.quants.emplace_back(std::stod(quantity));
                break;
            }
        }
    }

    data.tickers = std::vector<std::string>(data.assets.size(), "none");
    data.minimums = std::vector<double>(data.assets.size(), 0.0);
    data.prices = std::vector<double>(data.assets.size(), 0.0);
    data.active = std::vector<bool>(data.assets.size(), false);
    getSystemTime(data.lastQueryTime);

    std::cout << "Kraken fetchAccountBalance() executed." << std::endl;
    pauseApi();
}

void Kraken::fetchServerTime(long &data) noexcept
{
    krakenAPI->pub_func->get_server_time(&krakenAPI);
    json result = json::parse(krakenAPI->s_result);
    data = std::stol(result["result"]["unixtime"].dump());
    std::cout << "Kraken fetchServerTime() executed." << std::endl;
    pauseApi();
}

void Kraken::fetchOHLCData(Historic &data, const std::string &ticker) noexcept
{
    kraken_set_opt(&krakenAPI, "interval", CF.interVal.c_str());
    kraken_set_opt(&krakenAPI, "since", CF.startTime.c_str());

    if (ticker == CF.tickerList[0])
    {
        krakenAPI->pub_func->get_ohlc_data(&krakenAPI, CF.tickerList.back().c_str());
        json result = json::parse(krakenAPI->s_result);
        json ohlc(result["result"][CF.tickerList.back()]);

        for (auto slice : ohlc)
        {
            long extract_long = slice[0].get<long>();
            data.time.emplace_back(extract_long);
        }

        data.vwap = std::vector<double>(data.size(), 1.0);
        data.volume = std::vector<double>(data.size(), 0.0);
        data.quantity = std::vector<double>(data.size(), 0.0);
    }
    else
    {
        krakenAPI->pub_func->get_ohlc_data(&krakenAPI, ticker.c_str());
        json result = json::parse(krakenAPI->s_result);
        json ohlc(result["result"][ticker]);

        for (auto slice : ohlc)
        {
            long extract_long = slice[0].get<long>();
            data.time.emplace_back(extract_long);

            std::string extract_string = slice[5].get<std::string>();
            data.vwap.emplace_back(std::stod(extract_string));

            extract_string = slice[6].get<std::string>();
            data.volume.emplace_back(std::stod(extract_string));
        }
        data.quantity = std::vector<double>(data.size(), 0.0);
    }

    std::cout << "Kraken fetchOHLCData() executed for: " << ticker << std::endl;
    pauseApi();
}

void Kraken::fetchAllTickers() noexcept
{
    krakenAPI->pub_func->get_ticker_info(&krakenAPI, vec2str(CF.tickerList).c_str());
    tickerBuffer = json::parse(krakenAPI->s_result);
    std::cout << "Kraken fetchAllTickers() executed." << std::endl;
    pauseApi();
}

void Kraken::extractTickerData(Current &data, const std::string &ticker) noexcept
{
    if (ticker == CF.tickerList[0])
    {
        data.price = 1.0;
        data.ask = 1.0;
        data.bid = 1.0;
    }
    else
    {
        json extract(tickerBuffer["result"][ticker]);

        auto ask(extract["a"][0].get<std::string>());
        auto bid(extract["b"][0].get<std::string>());
        auto price(extract["c"][0].get<std::string>());

        data.price = std::stod(price);
        data.ask = std::stod(ask);
        data.bid = std::stod(bid);
    }

    getSystemTime(data.time);
}

void Kraken::extractTickerPrice(double &data, const std::string &ticker) noexcept
{
    if (ticker == CF.tickerList[0])
    {
        data = 1.0;
    }
    else
    {
        json extract(tickerBuffer["result"][ticker]);
        auto quote(extract["c"][0].get<std::string>());
        data = std::stod(quote);
    }
}

void Kraken::pauseApi() const noexcept
{
    free(krakenAPI->s_result);
    krakenAPI->s_result = nullptr;
    std::cout << "Kraken API is paused for: " << std::to_string(CF.pauseApi) << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(CF.pauseApi));
}

void Kraken::checkTimeStatus() noexcept
{
    long servertime, systemtime;
    fetchServerTime(servertime);
    getSystemTime(systemtime);

    std::cout << "Current servertime is: " << time2str(servertime) << std::endl;
    std::cout << "Current systemtime is: " << time2str(systemtime) << std::endl;
    std::cout << "The current time delay is " << fabs(static_cast<double>(servertime - systemtime)) << " seconds." << std::endl;
}

Kraken::Kraken(const std::string &apiKey, const std::string &secKey) noexcept
{
    kraken_init(&krakenAPI, apiKey.c_str(), secKey.c_str());
    std::cout << "Kraken constructor() executed." << std::endl;
}

Kraken::~Kraken() noexcept
{
    kraken_clean(&krakenAPI);
    std::cout << "Kraken destructor() executed." << std::endl;
}
