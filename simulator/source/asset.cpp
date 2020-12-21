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

#include "asset.h"

auto Historic::value() const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(vwap.begin(), vwap.end(), quantity.begin(), std::back_inserter(result), std::multiplies<double>());
    return result;
}

Asset::Asset(std::string ticker) noexcept : name(std::move(ticker))
{
    current.reserve(CF.idx2str.size());
    std::cout << "Asset constructor executed for: " << name << " with " << std::to_string(CF.idx2str.size()) << " currents." << std::endl;
}

Asset::~Asset() noexcept
{
    std::cout << "Asset destructor executed for: " << name << std::endl;
}

void Asset::fill_historic_riskfree(const std::vector<long> &timebins) noexcept
{
    historic.time = timebins;
    historic.open = std::vector<double>(historic.size(), 1.0);
    historic.high = std::vector<double>(historic.size(), 1.0);
    historic.low = std::vector<double>(historic.size(), 1.0);
    historic.close = std::vector<double>(historic.size(), 1.0);
    historic.vwap = std::vector<double>(historic.size(), 1.0);
    historic.volume = std::vector<double>(historic.size(), 0.0);
    historic.count = std::vector<long>(historic.size(), 0);
    historic.quantity = std::vector<double>(historic.size(), 0.0);

    std::cout << "Asset fill_riskfree_data() executed for: " << name << std::endl;
}

void Asset::set_current_riskfree(const size_t &which) noexcept
{
    current[which].ask = 1.0;
    current[which].bid = 1.0;
    current[which].price = 1.0;
    get_system_time(current[which].time);
}

void Asset::set_historic_quantity(const double &quantity, const size_t &idx) noexcept
{
    historic.quantity[idx] = quantity;
    std::fill(historic.quantity.begin() + static_cast<long>(idx), historic.quantity.end(), quantity);
}

void Asset::copy_current_data(const size_t &source, const size_t &dest) noexcept
{
    current[dest].time = current[source].time;
    current[dest].ask = current[source].ask;
    current[dest].bid = current[source].bid;
    current[dest].price = current[source].price;
    current[dest].quantity = current[source].quantity;
}

auto Asset::current_output_state(const double &pv, const size_t &which) const noexcept -> const std::stringstream
{
    std::stringstream result;

    result << get_name();
    result << "   With value: " << num2str(current[which].value());
    result << "   With weight: " << num2str(100.0 * current[which].value() / pv);
    result << "   With quantity: " << num2str(current[which].quantity);
    result << "   With price: " << num2str(current[which].price);
    result << std::endl;

    return result;
}

auto Asset::current_output_trade(const size_t &before, const size_t &after) const noexcept -> const std::stringstream
{
    std::stringstream result;
    double pv_change((current[after].quantity - current[before].quantity) * current[after].price);

    result << name;
    result << "  Delta value: " << num2str(pv_change);
    if (name != CF.riskfree_name)
    {
        result << "       Fee:" << num2str(fabs(pv_change) * CF.trade_fee);
    }
    else
    {
        result << "                     ";
    }
    result << "       Delta quantity: " << num2str(pv_change / current[after].price);
    result << std::endl;

    return result;
}
