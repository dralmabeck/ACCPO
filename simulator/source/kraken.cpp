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

Kraken::Kraken(const std::string &apikey, const std::string &seckey) noexcept
{
    kraken_init(&kr_api, apikey.c_str(), seckey.c_str());
    std::cout << "Kraken constructor executed." << std::endl;
}

Kraken::~Kraken() noexcept
{
    kraken_clean(&kr_api);
    std::cout << "Kraken destructor executed." << std::endl;
}

void Kraken::pause_api(long &seconds) const noexcept
{
    std::cout << "Kraken API is paused for: " << std::to_string(seconds) << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Kraken::get_server_time(long &data) noexcept
{
    kr_api->pub_func->get_server_time(&kr_api);
    store_and_free();

    data = std::stol(result_api["result"]["unixtime"].dump());
}

void Kraken::get_ohlc_data(Historic &data, const std::string &ticker, const std::string &since, const std::string &interval) noexcept
{
    kraken_set_opt(&kr_api, "interval", interval.c_str());
    kraken_set_opt(&kr_api, "since", since.c_str());

    kr_api->pub_func->get_ohlc_data(&kr_api, ticker.c_str());
    store_and_free();

    json ohlc(result_api["result"][ticker]);

    for (auto slice : ohlc)
    {
        long extract_long = slice[0].get<long>();
        data.time.emplace_back(extract_long);

        auto extract_string = slice[1].get<std::string>();
        data.open.emplace_back(std::stod(extract_string));

        extract_string = slice[2].get<std::string>();
        data.high.emplace_back(std::stod(extract_string));

        extract_string = slice[3].get<std::string>();
        data.low.emplace_back(std::stod(extract_string));

        extract_string = slice[4].get<std::string>();
        data.close.emplace_back(std::stod(extract_string));

        extract_string = slice[5].get<std::string>();
        data.vwap.emplace_back(std::stod(extract_string));

        extract_string = slice[6].get<std::string>();
        data.volume.emplace_back(std::stod(extract_string));

        extract_long = slice[7].get<long>();
        data.count.emplace_back(extract_long);
    }

    data.quantity = std::vector<double>(data.size(), 0.0);

    std::cout << "Kraken get_ohlc_data() executed for: " << ticker << std::endl;
}

void Kraken::fetch_all_tickers(const std::string &ticker_list) noexcept
{
    kr_api->pub_func->get_ticker_info(&kr_api, ticker_list.c_str());
    store_and_free();

    std::cout << "Kraken fetch_all_tickers() executed." << std::endl;
}

void Kraken::get_ticker_data(Current &data, const std::string &ticker) noexcept
{
    json extract(result_api["result"][ticker]);

    auto ask(extract["a"][0].get<std::string>());
    auto bid(extract["b"][0].get<std::string>());
    auto quote(extract["c"][0].get<std::string>());

    data.ask = std::stod(ask);
    data.bid = std::stod(bid);
    data.price = std::stod(quote);
    get_system_time(data.time);

    std::cout << "Kraken get_ticker_data() executed for: " << ticker << std::endl;
}

void Kraken::store_and_free() noexcept
{
    result_buffer = std::string(kr_api->s_result);
    result_api = json::parse(result_buffer);
    //output_result_buffer();

    free(kr_api->s_result);
    kr_api->s_result = nullptr;
    //std::cout << "Kraken store_and_free() executed." << std::endl;
}
