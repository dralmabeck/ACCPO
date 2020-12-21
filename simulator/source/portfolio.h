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

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "asset.h"
#include "config.h"
#include "utils.h"

// Class for storing, accessing and manipulating several cryptocurrencies in a portfolio
class Portfolio
{
public:
    // Constructor - takes a single asset - we put the riskfree asset first always
    explicit Portfolio(std::unique_ptr<Asset> &&asset) noexcept;
    // Destructor - cleans up
    ~Portfolio() noexcept;
    // Dummies to comply with the Rule of Five
    Portfolio(const Portfolio &source) = delete;
    Portfolio(Portfolio &&source) = delete;
    auto operator=(const Portfolio &source) -> Portfolio & = delete;
    auto operator=(Portfolio &&source) -> Portfolio & = delete;

    // Adds one asset into the portfolio - assets should have equal data sizes
    void add_asset(std::unique_ptr<Asset> &&asset) noexcept;

    // Read out total historic portfolio value at element idx
    [[nodiscard]] auto idx_total_value(const size_t &idx) const noexcept -> double;
    // Read out sum of historic portfolio weights at element idx - consistency check
    [[nodiscard]] auto idx_total_weight(const size_t &idx) const noexcept -> double;
    // Read of historic time at elemend idx
    [[nodiscard]] auto idx_time(const size_t &idx) const noexcept -> long { return assets[0]->historic.idx_time(idx); }

    // Read out vector of historic asset quantities at element idx
    [[nodiscard]] auto idx_list_quantities(const size_t &idx) const noexcept -> const std::vector<double>;
    // Read out vector of historic asset prices at element idx
    [[nodiscard]] auto idx_list_prices(const size_t &idx) const noexcept -> const std::vector<double>;

    // Read out total portfolio value of current one-time ticker information element "which"
    [[nodiscard]] auto current_total_value(const size_t &which) const noexcept -> double;
    // Read out total weight of current one-time ticker information element "which" - consistency check
    [[nodiscard]] auto current_total_weight(const size_t &which) const noexcept -> double;
    // Read out time of current one-time ticker information element "which"
    [[nodiscard]] auto current_time(const size_t &which) const noexcept -> long { return assets[0]->current[which].time; }

    // Read out vector of current one-time ticker information asset quantities at element "which"
    [[nodiscard]] auto current_list_quantities(const size_t &which) const noexcept -> const std::vector<double>;
    // Read out vector of current one-time ticker information asset prices at element "which"
    [[nodiscard]] auto current_list_prices(const size_t &which) const noexcept -> const std::vector<double>;

    // Read out number of asses in portfolio
    [[nodiscard]] auto number_assets() const noexcept -> size_t { return assets.size(); }
    // Read out number of timebins in historic data - Assumes all assets have same length!
    [[nodiscard]] auto number_timebins() const noexcept -> size_t { return assets[0]->historic.size(); }

    // Prints to the console the summary state of "which" elements of all assets
    [[nodiscard]] auto current_output_state(const size_t &which) const noexcept -> const std::stringstream;
    // Prints to the console the summary of changes in quantities (from optimization) between "before" and "after"
    [[nodiscard]] auto current_output_trade(const size_t &before, const size_t &after) const noexcept -> const std::stringstream;
    // Prints to the console the performance in one-time ticker information between "before" and "after"
    [[nodiscard]] auto current_output_performance(const size_t &before, const size_t &after) const noexcept -> const std::stringstream;

    // Prints to the console the entire timeseries of historic data including performance
    [[nodiscard]] auto history_output(const size_t &before, const size_t &after) const noexcept -> const std::stringstream;

    // Vector storing pointers to the individual assets
    std::vector<std::unique_ptr<Asset>> assets;

private:
};

#endif
