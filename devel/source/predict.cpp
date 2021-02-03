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

#include "predict.h"
#include "math.h"


auto Optimizer::predictWeights() noexcept -> const std::vector<double>
{
    // INVOKE POINT FOR DEEP REINFORCEMENT LEARNING ALGORITHM

    std::vector<double> result;

    // All assets have 0 weight, i.e. deinvestment
    //double equal_weight = 0.0;

    // All assets are weigthed equally, i.e. fully balanced investment
    double equal_weight = 1.0 / (static_cast<double>(P->numberAssets()) - 1.0);

    result = std::vector<double>(P->numberAssets(), equal_weight);

    // INVOKE POINT FOR DEEP REINFORCEMENT LEARNING ALGORITHM
    // MOVE DATA INTO NEURAL NETWORK
    // TRAIN AGENT ON RETURNS AND REWARDS
    // PREDICT PORTFOLIO WEIGHTS

    //std::cout << "Optimizer predict_weights() executed." << std::endl;
    return result;
}
