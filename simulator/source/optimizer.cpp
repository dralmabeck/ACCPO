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

#include "optimizer.h"

Optimizer::Optimizer(std::shared_ptr<Portfolio> portfolio) noexcept : P(std::move(portfolio))
{
    returns = new double *[P->number_timebins()];
    for (size_t ti(0); ti < P->number_timebins(); ti++)
    {
        returns[ti] = new double[P->number_assets()];
    }

    for (size_t asset(0); asset < P->number_assets(); asset++)
    {
        returns[0][asset] = 0.0;
        for (size_t ti(1); ti < P->number_timebins(); ti++)
        {
            returns[ti][asset] = 100.0 * (log(P->assets[asset]->historic.idx_price(ti)) - log(P->assets[asset]->historic.idx_price(ti - 1)));
        }
    }

    std::cout << "Optimizer constructor executed." << std::endl;
}

Optimizer::~Optimizer() noexcept
{
    for (size_t ti(0); ti < P->number_timebins(); ti++)
    {
        delete[] returns[ti];
    }
    delete[] returns;

    std::cout << "Optimizer destructor executed." << std::endl;
}

void Optimizer::current_initialize() noexcept
{
    // Initialize RISKFREE quantity for current "initial"
    P->assets[Configuration::RF]->set_current_quantity(CF.riskfree_quantity, Configuration::INI);

    // Initialize all other assets quantity for current "initial"
    for (size_t asset(1); asset < P->number_assets(); asset++)
    {
        P->assets[asset]->set_current_quantity(CF.asset_quantities[asset - 1], Configuration::INI);
    }

    // Initialize RISKFREE price data for current "rebalanced"
    //P->assets[Configuration::RF]->set_current_riskfree(Configuration::BAL);

    // Initialize all other assets price data for current "rebalanced"
    for (auto const &asset : P->assets)
    {
        asset->copy_current_data(Configuration::INI, Configuration::BAL);
        asset->copy_current_data(Configuration::INI, Configuration::LAT);
    }

    // Rebalance RISKFREE and all assets for current "rebalanced"
    current_update(Configuration::INI, Configuration::BAL);
    current_update(Configuration::INI, Configuration::LAT);

    std::cout << "Optimizer current_initialize() executed." << std::endl;
}

void Optimizer::current_update(const size_t &before, const size_t &after) noexcept
{
    std::vector<double> old_quant(P->current_list_quantities(before));
    std::vector<double> prices(P->current_list_prices(after));
    double pv(std::inner_product(old_quant.begin(), old_quant.end(), prices.begin(), 0.0f));

    std::vector<double> real_weights;
    std::transform(prices.begin(), prices.end(), old_quant.begin(), std::back_inserter(real_weights), std::multiplies<double>());
    std::transform(real_weights.begin(), real_weights.end(), real_weights.begin(), [&pv](const auto &rw) { return rw / pv; });

    std::vector<double> target_weights = predict_weights();

    double cash_delta(0.0);

    for (size_t asset(1); asset < P->number_assets(); asset++)
    {
        if (fabs(target_weights[asset] - real_weights[asset]) > CF.weight_diff)
        {
            double target_quantity(pv * target_weights[asset] / prices[asset]);

            double pre_fee(fabs(target_quantity - old_quant[asset]) * prices[asset] * CF.trade_fee);
            target_quantity -= 0.5 * pre_fee / prices[asset];
            double real_fee(fabs(target_quantity - old_quant[asset]) * prices[asset] * CF.trade_fee);

            cash_delta -= (target_quantity - old_quant[asset]) * prices[asset] + real_fee;

            P->assets[asset]->set_current_quantity(target_quantity, after);
        }
        else
        {
            P->assets[asset]->set_current_quantity(old_quant[asset], after);
        }
    }

    double cash_balance(old_quant[Configuration::RF] + cash_delta);
    P->assets[Configuration::RF]->set_current_quantity(cash_balance, after);

    std::cout << "Optimizer current_update() executed." << std::endl;
}

void Optimizer::history_calculate() noexcept
{
    for (size_t ti(1); ti < P->number_timebins(); ti++)
    {
        std::vector<double> old_quant(P->idx_list_quantities(ti - 1));
        std::vector<double> prices(P->idx_list_prices(ti));

        double pv(std::inner_product(old_quant.begin(), old_quant.end(), prices.begin(), 0.0f));

        std::vector<double> real_weights;
        std::transform(prices.begin(), prices.end(), old_quant.begin(), std::back_inserter(real_weights), std::multiplies<double>());
        std::transform(real_weights.begin(), real_weights.end(), real_weights.begin(), [&pv](const auto &rw) { return rw / pv; });

        std::vector<double> target_weights = predict_weights();

        double cash_delta(0.0);

        for (size_t asset(1); asset < P->number_assets(); asset++)
        {
            if (fabs(target_weights[asset] - real_weights[asset]) > CF.weight_diff)
            {
                double target_quantity(pv * target_weights[asset] / prices[asset]);

                double pre_fee(fabs(target_quantity - old_quant[asset]) * prices[asset] * CF.trade_fee);
                target_quantity -= 0.5 * pre_fee / prices[asset];
                double real_fee(fabs(target_quantity - old_quant[asset]) * prices[asset] * CF.trade_fee);

                cash_delta -= (target_quantity - old_quant[asset]) * prices[asset] + real_fee;

                P->assets[asset]->set_historic_quantity(target_quantity, ti);
            }
            else
            {
                P->assets[asset]->set_historic_quantity(old_quant[asset], ti);
            }
        }

        double cash_balance(old_quant[Configuration::RF] + cash_delta);
        P->assets[Configuration::RF]->set_historic_quantity(cash_balance, ti);
    }

    std::cout << "Optimizer history_calculate() executed." << std::endl;
}
