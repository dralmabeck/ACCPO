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

#include "config.h"

// This file contains program configuration calss

Configuration::Configuration() noexcept
{
    // Read file entries - final values
    read_parameter(apikey, "API_KEY");
    read_parameter(seckey, "SEC_KEY");
    read_parameter(riskfree_quantity, "RISKFREE_QUANTITY");
    read_parameter(interval, "INTERVAL");
    read_parameter(trade_fee, "TRADE_FEE");
    read_parameter(weight_diff, "WEIGHT_DIFF");
    read_parameter(pause_api, "PAUSE_API");
    read_parameter(pause_program, "PAUSE_PROG");
    read_parameter(asset_list, "ASSET_LIST");

    // Read file entries - to be processed further
    std::string inputtime;

    std::string asset_quants;
    read_parameter(asset_quants, "ASSET_QUANTITIES");
    read_parameter(inputtime, "STARTTIME");

    // Set-up list of names of portfolio assets
    std::stringstream stream_asset_list(asset_list);
    while (stream_asset_list.good())
    {
        std::string substring;
        std::getline(stream_asset_list, substring, ',');
        assets_names.emplace_back(substring);
    }

    // Set-up list of quantities of portfolio assets
    std::stringstream stream_asset_quantities(asset_quants);
    while (stream_asset_quantities.good())
    {
        std::string substring;
        std::getline(stream_asset_quantities, substring, ',');
        asset_quantities.emplace_back(std::stod(substring));
    }

    // Set-up starttime for polling OHLC data from Kraken API
    std::stringstream stream_starttime(inputtime);
    std::vector<int> utc;
    while (stream_starttime.good())
    {
        std::string substring;
        std::getline(stream_starttime, substring, ',');
        utc.emplace_back(std::stoi(substring));
    }

    // From integer vector via a long unixtime to final string representation
    starttime = std::to_string(get_unix_time(utc[0], utc[1], utc[2], utc[3], utc[4], utc[5]));
    std::cout << "Configuration constructor executed." << std::endl;
}

Configuration::~Configuration() noexcept
{
    std::cout << "Configuration destructor executed." << std::endl;
}

void Configuration::read_parameter(std::string &data, const std::string &parameter) noexcept
{
    std::string line, key, value;
    std::ifstream stream(Configuration::configfile);

    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            std::istringstream linestream(line);
            while (linestream >> key >> value)
            {
                if (key == parameter)
                {
                    data = value;
                }
            }
        }
    }
    stream.close();
    std::cout << "read_parameter executed for parameter: " << parameter << std::endl;
}

void Configuration::read_parameter(double &data, const std::string &parameter) noexcept
{
    std::string convert;
    read_parameter(convert, parameter);
    data = std::stod(convert);
}

void Configuration::read_parameter(long &data, const std::string &parameter) noexcept
{
    std::string convert;
    read_parameter(convert, parameter);
    data = std::stol(convert);
}
