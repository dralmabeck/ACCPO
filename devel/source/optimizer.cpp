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


#include "math.h"


Optimizer::Optimizer(std::shared_ptr<Portfolio> P, const std::shared_ptr<Kraken>& K) noexcept : P(std::move(P)), K(std::move(K))
{
    std::cout << "Optimizer constructor() executed." << std::endl;
}

void Optimizer::propagateCurrent() noexcept
{
    std::vector<double> old_quant(P->currentAllQuantities(CF.INI));
    std::vector<double> prices(P->currentAllPrices(CF.BAL));
    double pv(std::inner_product(old_quant.begin(), old_quant.end(), prices.begin(), 0.0f));

    std::vector<double> real_weights;
    std::transform(prices.begin(), prices.end(), old_quant.begin(), std::back_inserter(real_weights), std::multiplies<double>());
    std::transform(real_weights.begin(), real_weights.end(), real_weights.begin(), [&pv](const auto &rw) { return rw / pv; });

    std::vector<double> target_weights = predictWeights();

    double cash_delta(0.0);

    for (size_t asset(1); asset < P->numberAssets(); asset++)
    {
        if (fabs(target_weights[asset] - real_weights[asset]) >= CF.weightDiff)
        {
            double target_quantity(pv * target_weights[asset] / prices[asset]);
            double tradeQuantity = target_quantity - old_quant[asset];

            if (fabs(tradeQuantity) >= P->assets[asset]->minimum)
            {
                double fee(fabs(tradeQuantity) * prices[asset] * CF.tradeFee);

                cash_delta -= tradeQuantity * prices[asset] + fee;

                P->assets[asset]->setCurrentQuantity(target_quantity, CF.BAL);

                Trade newTrade;
                newTrade.ticker = P->assets[asset]->ticker;
                newTrade.quantity = tradeQuantity;
                newTrade.price = prices[asset];
                newTrade.market = "market";
                K->addTradePipeline(newTrade);
            }
            else
            {
                P->assets[asset]->setCurrentQuantity(old_quant[asset], CF.BAL);
            }
        }
        else
        {
            P->assets[asset]->setCurrentQuantity(old_quant[asset], CF.BAL);
        }
    }

    double cash_balance(old_quant[0] + cash_delta);
    P->assets[0]->setCurrentQuantity(cash_balance, CF.BAL);

    std::cout << "Optimizer current_update() executed." << std::endl;
}

void Optimizer::calculateHistoric() noexcept
{
    for (size_t ti(1); ti < P->numberTimebins(); ti++)
    {
        std::vector<double> old_quant(P->idxAllQuantities(ti - 1));
        std::vector<double> prices(P->idxAllPrices(ti));

        double pv(std::inner_product(old_quant.begin(), old_quant.end(), prices.begin(), 0.0f));

        std::vector<double> real_weights;
        std::transform(prices.begin(), prices.end(), old_quant.begin(), std::back_inserter(real_weights), std::multiplies<double>());
        std::transform(real_weights.begin(), real_weights.end(), real_weights.begin(), [&pv](const auto &rw) { return rw / pv; });

        std::vector<double> target_weights = predictWeights();

        double cash_delta(0.0);

        for (size_t asset(1); asset < P->numberAssets(); asset++)
        {
            if (fabs(target_weights[asset] - real_weights[asset]) >= CF.weightDiff)
            {
                double target_quantity(pv * target_weights[asset] / prices[asset]);
                double tradeQuantity = target_quantity - old_quant[asset];

                if (fabs(tradeQuantity) >= P->assets[asset]->minimum)
                {
                    double fee(fabs(tradeQuantity) * prices[asset] * CF.tradeFee);

                    cash_delta -= tradeQuantity * prices[asset] + fee;

                    P->assets[asset]->setHistoricQuantity(target_quantity, ti);
                }
                else
                {
                    P->assets[asset]->setHistoricQuantity(old_quant[asset], ti);
                }
            }
            else
            {
                P->assets[asset]->setHistoricQuantity(old_quant[asset], ti);
            }
        }

        double cash_balance(old_quant[0] + cash_delta);
        P->assets[0]->setHistoricQuantity(cash_balance, ti);
    }

    std::cout << "Optimizer calculateHistoric() executed." << std::endl;
}
