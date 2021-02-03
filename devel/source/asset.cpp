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
alongwith this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "asset.h"


#include "math.h"


auto Historic::value() const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(vwap.begin(), vwap.end(), quantity.begin(), std::back_inserter(result), std::multiplies<>());
    return result;
}

Asset::Asset(const std::string& name, const std::string& ticker, const double& minimum, const std::shared_ptr<Kraken>& K) noexcept : name(std::move(name)), ticker(std::move(ticker)), minimum(std::move(minimum)), K(std::move(K))
{
    current.reserve(CF.idx2str.size());
    if (this->name.size() == 3)
    {
        this->name = " " + this->name;
    }
    std::cout << "Asset constructor() executed for: " << this->name << " with " << std::to_string(CF.idx2str.size()) << " currents." << std::endl;
}

void Asset::setHistoricData() noexcept
{
    K->fetchOHLCData(historic, ticker);
    //std::cout << "Asset setHistoricData() executed." << std::endl;
}

void Asset::setCurrentData(const size_t &which) noexcept
{
    K->extractTickerData(current[which], ticker);
    //std::cout << "Asset setCurrentData() executed." << std::endl;
}

void Asset::setHistoricQuantity(const double &quantity, const size_t &idx) noexcept
{
    historic.quantity[idx] = quantity;
    std::fill(historic.quantity.begin() + static_cast<long>(idx), historic.quantity.end(), quantity);
}

void Asset::copyCurrentObject(const size_t &source, const size_t &dest) noexcept
{
    current[dest].time = current[source].time;
    current[dest].ask = current[source].ask;
    current[dest].bid = current[source].bid;
    current[dest].price = current[source].price;
    current[dest].quantity = current[source].quantity;
}

auto Asset::printCurrentState(const double &pv, const size_t &which) noexcept -> std::stringstream
{
    std::stringstream result;

    result << name;
    result << "   With value: " << num2str(current[which].value(), 2);
    result << "   With weight: " << num2str(100.0 * current[which].value() / pv, 2);
    result << "   With quantity: " << num2str(current[which].quantity, 8);
    result << "   With price: " << num2str(current[which].price, 4);
    result << std::endl;

    return result;
}

auto Asset::printCurrentTrades(const size_t &before, const size_t &after) noexcept -> std::stringstream
{
    std::stringstream result;
    double pvChange((current[after].quantity - current[before].quantity) * current[CF.BAL].price);

    result << name;
    result << "  Delta value: " << num2str(pvChange, 2);
    if (name == CF.tradeableAssets[0])
    {
        result << "                   ";
    }
    else
    {
        result << "       Fee:" << num2str(fabs(pvChange * CF.tradeFee), 2);
    }
    result << "       Delta quantity: " << num2str(pvChange / current[CF.BAL].price, 8);
    result << std::endl;

    return result;
}
