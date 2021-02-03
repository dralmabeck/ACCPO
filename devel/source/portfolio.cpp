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

#include "portfolio.h"

void Portfolio::addAsset(std::unique_ptr<Asset> &&asset) noexcept
{
    assets.emplace_back(std::move(asset));
    std::cout << "Portfolio addAsset() executed for: " << assets.back()->name << std::endl;
}

auto Portfolio::idxTotalValue(const size_t &idx) noexcept -> double
{
    std::vector<double> values;
    std::transform(assets.begin(), assets.end(), std::back_inserter(values), [&idx](const auto &asset) { return asset->historic.idxValue(idx); });
    return static_cast<double>(std::accumulate(values.begin(), values.end(), 0.0f));
}

auto Portfolio::idxTotalWeight(const size_t &idx) noexcept -> double
{
    std::vector<double> weights;
    double pv(idxTotalValue(idx));
    std::transform(assets.begin(), assets.end(), std::back_inserter(weights), [&idx, &pv](const auto &asset) { return asset->historic.idxValue(idx) / pv; });
    return static_cast<double>(std::accumulate(weights.begin(), weights.end(), 0.0f));
}

auto Portfolio::idxAllQuantities(const size_t &idx) const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(assets.begin(), assets.end(), std::back_inserter(result), [&idx](const auto &asset) { return asset->historic.quantity[idx]; });
    return result;
}

auto Portfolio::idxAllPrices(const size_t &idx) const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(assets.begin(), assets.end(), std::back_inserter(result), [&idx](const auto &asset) { return asset->historic.vwap[idx]; });
    return result;
}

auto Portfolio::currentTotalValue(const size_t &which) noexcept -> double
{
    std::vector<double> values;
    std::transform(assets.begin(), assets.end(), std::back_inserter(values), [&which](const auto &asset) { return asset->current[which].value(); });
    return static_cast<double>(std::accumulate(values.begin(), values.end(), 0.0f));
}

auto Portfolio::currentTotalWeight(const size_t &which) noexcept -> double
{
    std::vector<double> weights;
    double pv(currentTotalValue(which));
    std::transform(assets.begin(), assets.end(), std::back_inserter(weights), [&which, &pv](const auto &asset) { return asset->current[which].value() / pv; });
    return static_cast<double>(std::accumulate(weights.begin(), weights.end(), 0.0f));
}

auto Portfolio::currentAllQuantities(const size_t &which) const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(assets.begin(), assets.end(), std::back_inserter(result), [&which](const auto &asset) { return asset->current[which].quantity; });
    return result;
}

auto Portfolio::currentAllPrices(const size_t &which) const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(assets.begin(), assets.end(), std::back_inserter(result), [&which](const auto &asset) { return asset->current[which].price; });
    return result;
}

auto Portfolio::printCurrentState(const size_t &which) noexcept -> std::stringstream
{
    std::stringstream result;

    result << "===================================================================================================================" << std::endl;
    result << CF.idx2str[which] << " Portfolio at ";
    result << " time: " << time2str(currentTime(which)) << std::endl;
    result << "       Tot. value: " << num2str(currentTotalValue(which), 2);
    result << "   Tot. weight: " << num2str(100.0 * currentTotalWeight(which), 2);
    result << std::endl;

    for (auto const &asset : assets)
    {
        result << asset->printCurrentState(currentTotalValue(which), which).str();
    }

    result << "===================================================================================================================" << std::endl;
    return result;
}

auto Portfolio::printCurrentTrades(const size_t &before, const size_t &after) noexcept -> std::stringstream
{
    std::stringstream result;

    result << "============================= " << CF.idx2str[before] << " to ";
    result << CF.idx2str[after] << " =================================================================" << std::endl;
    for (auto const &asset : assets)
    {
        result << asset->printCurrentTrades(before, after).str();
    }

    return result;
}

auto Portfolio::printCurrentPerformance(const size_t &before, const size_t &after) noexcept -> std::stringstream
{
    std::stringstream result;

    double pv2(currentTotalValue(after));
    double pv1(currentTotalValue(before));

    result << "===================================================================================================================" << std::endl;
    result << "Performance between " << CF.idx2str[before] << " and " << CF.idx2str[after] << std::endl;
    result << "Performance total:    " << num2str((pv2 / pv1 - 1.0) * 100.0, 2) << " %" << std::endl;
    result << "PV before:              " << num2str(pv1, 2) << std::endl;
    result << "PV after:               " << num2str(pv2, 2) << std::endl;
    result << "PV change:              " << num2str(pv2 - pv1, 2) << std::endl;
    result << "Time before: " << time2str(currentTime(before)) << std::endl;
    result << "Time after:  " << time2str(currentTime(after)) << std::endl;
    result << "===================================================================================================================" << std::endl;

    return result;
}

auto Portfolio::printHistoric(const size_t &before, const size_t &after) noexcept -> std::stringstream
{
    std::stringstream result;

    result << "===================================================================================================================" << std::endl;
    result << "Portfolio historic timeseries from vector elements " << std::to_string(before) << " to " << std::to_string(after - 1) << std::endl;
    for (size_t ti(before); ti < after; ti++)
    {
        double pv(idxTotalValue(ti));

        result << "Time: " << time2str(idxTime(ti));
        result << " at vector element: " << std::to_string(ti) << std::endl;
        result << "       Tot. value: " << num2str(idxTotalValue(ti), 2);
        result << "   Tot. weight: " << num2str(100.0 * idxTotalWeight(ti), 2);
        result << std::endl;

        for (auto const &asset : assets)
        {
            result << asset->name;
            result << "   With value: " << num2str(asset->historic.idxValue(ti), 2);
            result << "   With weight: " << num2str(100.0 * asset->historic.idxValue(ti) / pv, 2);
            result << "   With quantity: " << num2str(asset->historic.quantity[ti], 8);
            result << "   With price: " << num2str(asset->historic.vwap[ti], 4);
            result << std::endl;
        }
    }

    double pv2(idxTotalValue(after));
    double pv1(idxTotalValue(before));
    auto time2(static_cast<double>(idxTime(after)));
    auto time1(static_cast<double>(idxTime(before)));

    double delta_t((time2 - time1) / (365.25 * 24.0 * 60.0 * 60.0 / 12.0));
    double perf(pow(pv2 / pv1, 1.0 / delta_t));

    result << "===================================================================================================================" << std::endl;
    result << "Performance total:      " << num2str((pv2 / pv1 - 1.0) * 100.0, 2) << " %" << std::endl;
    result << "Performance monthly:    " << num2str((perf - 1.0) * 100.0, 2) << " %" << std::endl;
    result << "Portfolio value before: " << num2str(pv1, 2) << std::endl;
    result << "Portfolio value after:  " << num2str(pv2, 2) << std::endl;
    result << "PV delta:               " << num2str(pv2 - pv1, 2) << std::endl;
    result << "Time before: " << time2str(idxTime(before)) << std::endl;
    result << "Time after:  " << time2str(idxTime(after)) << std::endl;
    result << "===================================================================================================================" << std::endl;

    return result;
}
