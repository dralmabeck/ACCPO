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

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "utils.h"
#include "config.h"
#include "portfolio.h"

class Optimizer
{
public:
    std::shared_ptr<Portfolio> P;
    std::shared_ptr<Kraken> K;

    auto predictWeights() noexcept -> const std::vector<double>;

    void propagateCurrent() noexcept;
    void calculateHistoric() noexcept;

    explicit Optimizer(std::shared_ptr<Portfolio> P, const std::shared_ptr<Kraken>& K) noexcept;
    ~Optimizer() noexcept = default;
    Optimizer(const Optimizer &source) noexcept = delete;
    Optimizer(Optimizer &&source) noexcept = delete;
    auto operator=(const Optimizer &source) noexcept -> Optimizer & = delete;
    auto operator=(Optimizer &&source) noexcept -> Optimizer & = delete;
};

#endif
