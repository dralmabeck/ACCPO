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

Portfolio::Portfolio(std::unique_ptr<Asset> &&asset) noexcept
{
    add_asset(std::move(asset));
    std::cout << "Portfolio constructor executed." << std::endl;
}

Portfolio::~Portfolio() noexcept
{
    std::cout << "Portfolio destructor executed." << std::endl;
}

void Portfolio::add_asset(std::unique_ptr<Asset> &&asset) noexcept
{
    assets.emplace_back(std::move(asset));
    std::cout << "Portfolio add_asset executed for: " << assets.back()->get_name() << std::endl;
}

auto Portfolio::idx_total_value(const size_t &idx) const noexcept -> double
{
    std::vector<double> values;
    std::transform(assets.begin(), assets.end(), std::back_inserter(values), [&idx](const auto &asset) { return asset->historic.idx_value(idx); });
    return static_cast<double>(std::accumulate(values.begin(), values.end(), 0.0f));
}

auto Portfolio::idx_total_weight(const size_t &idx) const noexcept -> double
{
    std::vector<double> weights;
    double pv(idx_total_value(idx));
    std::transform(assets.begin(), assets.end(), std::back_inserter(weights), [&idx, &pv](const auto &asset) { return asset->historic.idx_value(idx) / pv; });
    return static_cast<double>(std::accumulate(weights.begin(), weights.end(), 0.0f));
}

auto Portfolio::idx_list_quantities(const size_t &idx) const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(assets.begin(), assets.end(), std::back_inserter(result), [&idx](const auto &asset) { return asset->historic.idx_quantity(idx); });
    return result;
}

auto Portfolio::idx_list_prices(const size_t &idx) const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(assets.begin(), assets.end(), std::back_inserter(result), [&idx](const auto &asset) { return asset->historic.idx_price(idx); });
    return result;
}

auto Portfolio::current_total_value(const size_t &which) const noexcept -> double
{
    std::vector<double> values;
    std::transform(assets.begin(), assets.end(), std::back_inserter(values), [&which](const auto &asset) { return asset->current[which].value(); });
    return static_cast<double>(std::accumulate(values.begin(), values.end(), 0.0f));
}

auto Portfolio::current_total_weight(const size_t &which) const noexcept -> double
{
    std::vector<double> weights;
    double pv(current_total_value(which));
    std::transform(assets.begin(), assets.end(), std::back_inserter(weights), [&which, &pv](const auto &asset) { return asset->current[which].value() / pv; });
    return static_cast<double>(std::accumulate(weights.begin(), weights.end(), 0.0f));
}

auto Portfolio::current_list_quantities(const size_t &which) const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(assets.begin(), assets.end(), std::back_inserter(result), [&which](const auto &asset) { return asset->current[which].quantity; });
    return result;
}

auto Portfolio::current_list_prices(const size_t &which) const noexcept -> const std::vector<double>
{
    std::vector<double> result;
    std::transform(assets.begin(), assets.end(), std::back_inserter(result), [&which](const auto &asset) { return asset->current[which].price; });
    return result;
}

auto Portfolio::current_output_state(const size_t &which) const noexcept -> const std::stringstream
{
    std::stringstream result;

    result << "..................................................................................................................." << std::endl;
    result << CF.idx2str[which] << " Portfolio at ";
    result << " time: " << time2str(current_time(which)) << std::endl;
    result << "           Tot. value: " << num2str(current_total_value(which));
    result << "   Tot. weight: " << num2str(100.0 * current_total_weight(which));
    result << std::endl;

    for (auto const &asset : assets)
    {
        result << asset->current_output_state(current_total_value(which), which).str();
    }

    return result;
}

auto Portfolio::current_output_trade(const size_t &before, const size_t &after) const noexcept -> const std::stringstream
{
    std::stringstream result;

    result << "..................................................................................................................." << std::endl;
    result << "########## " << CF.idx2str[before] << " to " << CF.idx2str[after] << " ##########" << std::endl;
    for (auto const &asset : assets)
    {
        result << asset->current_output_trade(before, after).str();
    }

    return result;
}

auto Portfolio::current_output_performance(const size_t &before, const size_t &after) const noexcept -> const std::stringstream
{
    std::stringstream result;

    double pv2(current_total_value(after));
    double pv1(current_total_value(before));

    result << "..................................................................................................................." << std::endl;
    result << "Performance between " << CF.idx2str[before] << " and " << CF.idx2str[after] << std::endl;
    result << "Performance total:    " << num2str((pv2 / pv1 - 1.0) * 100.0) << " %" << std::endl;
    result << "PV before:              " << num2str(pv1) << std::endl;
    result << "PV after:               " << num2str(pv2) << std::endl;
    result << "PV change:              " << num2str(pv2 - pv1) << std::endl;
    result << "Time before: " << time2str(current_time(before)) << std::endl;
    result << "Time after:  " << time2str(current_time(after)) << std::endl;
    result << "..................................................................................................................." << std::endl;

    return result;
}

auto Portfolio::history_output(const size_t &before, const size_t &after) const noexcept -> const std::stringstream
{
    std::stringstream result;
    
    result << "..................................................................................................................." << std::endl;
    result << "Output of portfolio historic timeseries from " << std::to_string(before) << " to " << std::to_string(after - 1) << std::endl;
    result << "..................................................................................................................." << std::endl;
    for (size_t ti(before); ti < after; ti++)
    {
        double pv(idx_total_value(ti));

        result << "Time: " << time2str(idx_time(ti));
        result << " at vector element: " << std::to_string(ti) << std::endl;
        result << "           Tot. value: " << num2str(idx_total_value(ti));
        result << "   Tot. weight: " << num2str(100.0 * idx_total_weight(ti));
        result << std::endl;

        for (auto const &asset : assets)
        {
            result << asset->get_name();
            result << "   With value: " << num2str(asset->historic.idx_value(ti));
            result << "   With weight: " << num2str(100.0 * asset->historic.idx_value(ti) / pv);
            result << "   With quantity: " << num2str(asset->historic.idx_quantity(ti));
            result << "   With price: " << num2str(asset->historic.idx_price(ti));
            result << std::endl;
        }
    }

    double pv2(idx_total_value(after));
    double pv1(idx_total_value(before));
    auto time2(static_cast<double>(idx_time(after)));
    auto time1(static_cast<double>(idx_time(before)));

    double delta_t((time2 - time1) / (365.25 * 24.0 * 60.0 * 60.0 / 12.0));
    double perf(pow(pv2 / pv1, 1.0 / delta_t));

    result << "..................................................................................................................." << std::endl;
    result << "Performance total:    " << num2str((pv2 / pv1 - 1.0) * 100.0) << " %" << std::endl;
    result << "Performance monthly:  " << num2str((perf - 1.0) * 100.0) << " %" << std::endl;
    result << "PV before:              " << num2str(pv1) << std::endl;
    result << "PV after:               " << num2str(pv2) << std::endl;
    result << "PV change:              " << num2str(pv2 - pv1) << std::endl;
    result << "Time before: " << time2str(idx_time(before)) << std::endl;
    result << "Time after:  " << time2str(idx_time(after)) << std::endl;
    result << "..................................................................................................................." << std::endl;

    return result;
}
