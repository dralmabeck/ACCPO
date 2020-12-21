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

#ifndef ASSET_H
#define ASSET_H

#include "config.h"
#include "utils.h"

// Struct for storing the historic OHLC data
struct Historic
{
    // Return length of all vectors (should be equal)
    [[nodiscard]] auto size() const noexcept -> size_t { return time.size(); }
    // Fields to be filled with data by Kraken API
    std::vector<long> time;
    std::vector<double> open;
    std::vector<double> high;
    std::vector<double> low;
    std::vector<double> close;
    std::vector<double> vwap;
    std::vector<double> volume;
    std::vector<long> count;
    // Field to be filled by user
    std::vector<double> quantity;
    // Calculate value of asset vector
    [[nodiscard]] auto value() const noexcept -> const std::vector<double>;
    // Return data at single index position
    [[nodiscard]] auto idx_time(const size_t &idx) const noexcept -> long { return time[idx]; }
    [[nodiscard]] auto idx_price(const size_t &idx) const noexcept -> double { return vwap[idx]; }
    [[nodiscard]] auto idx_value(const size_t &idx) const noexcept -> double { return quantity[idx] * vwap[idx]; }
    [[nodiscard]] auto idx_quantity(const size_t &idx) const noexcept -> double { return quantity[idx]; }
};

// Struct for storing one-time ticker information
struct Current
{
    long time;
    double ask;
    double bid;
    double price;
    double quantity;
    // Return value of asset
    [[nodiscard]] auto value() const noexcept -> double { return price*quantity; }
};

// Class for storing, accessing and manipulating one single cryptocurrency
class Asset
{
public:
    // Constructor - takes ticker name
    explicit Asset(std::string ticker) noexcept;
    // Destructor - clean up
    ~Asset() noexcept;
    // Dummies to comply with Rule of Five
    Asset(const Asset &source) = delete;
    Asset(Asset &&source) = delete;
    auto operator=(const Asset &source) -> Asset & = delete;
    auto operator=(Asset &&source) -> Asset & = delete;

    // Return name of asset
    [[nodiscard]] auto get_name() const noexcept -> const std::string { return name; }

    // Fill the asset with riskfree data
    void fill_historic_riskfree(const std::vector<long> &timebins) noexcept;
    // Set data of element "which" in one-time ticker information vector to riskfree data
    void set_current_riskfree(const size_t &which) noexcept;


    // Sets the historic quantity of this asset hold at index idx and beyond
    void set_historic_quantity(const double &quantity, const size_t &idx) noexcept;
    // Set data of element "which" in one-time ticker information to data struct
    void set_current_quantity(const double &quantity, const size_t &which) noexcept { current[which].quantity = quantity; }

    // Copy element in current vector from "source" location to "dest" location
    void copy_current_data(const size_t &source, const size_t &dest) noexcept;

    // Prints to the console the state of "which" element in current vector, requires total portfolio value
    [[nodiscard]] auto current_output_state(const double &pv, const size_t &which) const noexcept -> const std::stringstream;
    // Prints to the console the changes in quantities (from optimization) between "before" and "after"
    [[nodiscard]] auto current_output_trade(const size_t &before, const size_t &after) const noexcept -> const std::stringstream;

    // Variable to store entire historic OHLC data
    Historic historic;
    // Vector of single one-time ticker informations
    std::vector<Current> current;

private:
    // Name of asset
    std::string name;
};

#endif
