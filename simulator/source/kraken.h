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

#include "asset.h"
#include "utils.h"

// Using API to Kraken exchange written for C11 language
extern "C"
{
#include "../thirdparty/kraken/kraken_api.h"
}

#include "../thirdparty/json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

// Wrapper for communication with Kraken exchange
class Kraken
{
public:
    // Constructor - takes two API keys
    Kraken(const std::string &apikey, const std::string &seckey) noexcept;
    // Destructor - clean up
    ~Kraken() noexcept;
    // Dummies to comply with Rule of Five
    Kraken(const Kraken &source) = delete;
    Kraken(Kraken &&source) = delete;
    auto operator=(const Kraken &source) -> Kraken & = delete;
    auto operator=(Kraken &&source) -> Kraken & = delete;

    // Kraken exchange number of limits calls to API per time interval
    // Pause API for some time to prevent being blocked out by API
    void pause_api(long &seconds) const noexcept;

    // Get server time - Useful for comparing with local time
    void get_server_time(long &data) noexcept;

    // Get historic data OHLC for one ticker at a time from "start" time in spacing of "interval"
    void get_ohlc_data(Historic &data, const std::string &ticker, const std::string &since, const std::string &interval) noexcept;

    // Get latest price/ticker information for several tickers in list - store result in buffer
    void fetch_all_tickers(const std::string &ticker_list) noexcept;

    // From buffer extract one individual ticker information - execute "fetch_all_tickers" first
    void get_ticker_data(Current &data, const std::string &ticker) noexcept;

private:
    // Pointer to API
    struct kraken_api *kr_api = nullptr;

    // Buffer to store ersult from last API query
    std::string result_buffer;
    json result_api;

    // Call after each API usage to store result and clean API
    void store_and_free() noexcept;
};

#endif
