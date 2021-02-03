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
#include "utils.h"

class Asset;

class Portfolio
{
public:
    std::vector<std::unique_ptr<Asset>> assets{};

    void addAsset(std::unique_ptr<Asset> &&asset) noexcept;

    [[nodiscard]] auto idxTotalValue(const size_t &idx) noexcept -> double;
    [[nodiscard]] auto idxTotalWeight(const size_t &idx) noexcept -> double;
    [[nodiscard]] auto idxTime(const size_t &idx) const noexcept -> long { return assets[0]->historic.time[idx]; }
    [[nodiscard]] auto idxAllQuantities(const size_t &idx) const noexcept -> const std::vector<double>;
    [[nodiscard]] auto idxAllPrices(const size_t &idx) const noexcept -> const std::vector<double>;

    [[nodiscard]] auto currentTotalValue(const size_t &which) noexcept -> double;
    [[nodiscard]] auto currentTotalWeight(const size_t &which) noexcept -> double;
    [[nodiscard]] auto currentTime(const size_t &which) const noexcept -> long { return assets[0]->current[which].time; }

    [[nodiscard]] auto currentAllQuantities(const size_t &which) const noexcept -> const std::vector<double>;
    [[nodiscard]] auto currentAllPrices(const size_t &which) const noexcept -> const std::vector<double>;

    [[nodiscard]] auto numberAssets() const noexcept -> size_t { return assets.size(); }
    [[nodiscard]] auto numberTimebins() const noexcept -> size_t { return assets[0]->historic.size(); }

    [[nodiscard]] auto printCurrentState(const size_t &which) noexcept -> std::stringstream;
    [[nodiscard]] auto printCurrentTrades(const size_t &before, const size_t &after) noexcept -> std::stringstream;
    [[nodiscard]] auto printCurrentPerformance(const size_t &before, const size_t &after) noexcept -> std::stringstream;

    [[nodiscard]] auto printHistoric(const size_t &before, const size_t &after) noexcept -> std::stringstream;

    explicit Portfolio() noexcept = default;
    ~Portfolio() noexcept = default;
    Portfolio(const Portfolio &source) = delete;
    Portfolio(Portfolio &&source) = delete;
    auto operator=(const Portfolio &source) -> Portfolio & = delete;
    auto operator=(Portfolio &&source) -> Portfolio & = delete;
};

#endif
