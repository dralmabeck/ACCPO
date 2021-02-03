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
#include "kraken.h"
#include "utils.h"

class Kraken;

struct Historic
{
    std::vector<long> time{};
    std::vector<double> vwap{};
    std::vector<double> volume{};
    std::vector<double> quantity{};
    [[nodiscard]] auto value() const noexcept -> const std::vector<double>;
    [[nodiscard]] auto idxValue(const size_t &idx) const noexcept -> double { return quantity[idx] * vwap[idx]; }
    [[nodiscard]] auto size() const noexcept -> size_t { return time.size(); }
};

struct Current
{
    long time;
    double ask;
    double bid;
    double price;
    double quantity;
    [[nodiscard]] auto value() const noexcept -> double { return bid * quantity; }
};

class Asset
{
public:
    Historic historic;
    std::vector<Current> current{};
    std::string name;
    std::string ticker;
    double minimum;
    std::shared_ptr<Kraken> K;

    void setHistoricData() noexcept;
    void setHistoricQuantity(const double &quantity, const size_t &idx) noexcept;

    void setCurrentData(const size_t &which) noexcept;
    void setCurrentQuantity(const double &quantity, const size_t &which) noexcept { current[which].quantity = quantity; }
    void copyCurrentObject(const size_t &source, const size_t &dest) noexcept;

    [[nodiscard]] auto printCurrentState(const double &pv, const size_t &which) noexcept -> std::stringstream;
    [[nodiscard]] auto printCurrentTrades(const size_t &before, const size_t &after) noexcept -> std::stringstream;

    explicit Asset(const std::string& name, const std::string& ticker, const double& minimum, const std::shared_ptr<Kraken>& K) noexcept;
    ~Asset() noexcept = default;
    Asset(const Asset &source) = delete;
    Asset(Asset &&source) = delete;
    auto operator=(const Asset &source) -> Asset & = delete;
    auto operator=(Asset &&source) -> Asset & = delete;
};

#endif
