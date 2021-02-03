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

#ifndef KRAKEN_H
#define KRAKEN_H

#include "config.h"
#include "utils.h"
#include "asset.h"
#include "account.h"

extern "C"
{
#include "../thirdparty/kraken/kraken_api.hpp"
}

#include "../thirdparty/json/json.hpp"
using json = nlohmann::json;

class Account;
struct Historic;
struct Current;

class Kraken
{
public:
    void fetchAllTXID() noexcept;
    void processTradePipeline() noexcept;

    void placeOrder(Trade &tradeObject) noexcept;
    void addTradePipeline(const Trade &tradeObject) noexcept;

    void fetchAccountBalance(Account &data) noexcept;
    void fetchServerTime(long &data) noexcept;
    void fetchOHLCData(Historic &data, const std::string &ticker) noexcept;

    void fetchAllTickers() noexcept;
    void extractTickerData(Current &data, const std::string &ticker) noexcept;
    void extractTickerPrice(double &data, const std::string &ticker) noexcept;

    void pauseApi() const noexcept;
    void checkTimeStatus() noexcept;

    Kraken(const std::string &apiKey, const std::string &secKey) noexcept;
    ~Kraken() noexcept;
    Kraken(const Kraken &source) = delete;
    Kraken(Kraken &&source) = delete;
    auto operator=(const Kraken &source) -> Kraken & = delete;
    auto operator=(Kraken &&source) -> Kraken & = delete;

private:
    struct kraken_api *krakenAPI = nullptr;
    json tickerBuffer{};
    json txidBuffer{};

    std::vector<Trade> tradePipeline{};
};

#endif
