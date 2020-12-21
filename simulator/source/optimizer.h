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

#include "config.h"
#include "portfolio.h"
#include "utils.h"

// Class for optimizing and manipulating a portfolio
class Optimizer
{
public:
    // Constructor - takes portfolio
    explicit Optimizer(std::shared_ptr<Portfolio> portfolio) noexcept;
    // Destructor - cleans up
    ~Optimizer() noexcept;
    // Dummies to comply with the Rule of Five
    Optimizer(const Optimizer &source) noexcept = delete;
    Optimizer(Optimizer &&source) noexcept = delete;
    auto operator=(const Optimizer &source) noexcept -> Optimizer & = delete;
    auto operator=(Optimizer &&source) noexcept -> Optimizer & = delete;

    // Initialize the first element in current one-time ticker information vectors
    // Accesses individual assets and manipulates their quantities directly
    void current_initialize() noexcept;

    // Perform optimization of between the "before" and "after" elements
    // Accesses individual assets and optimizes quantities of current one-time ticker vectors
    void current_update(const size_t &before, const size_t &after) noexcept;

    // Perform optimization on historic data in portfolio
    // Accesses individual assets and optimizes quantities of historic data struct
    void history_calculate() noexcept;

private:
    // Portfolio on which optimization should takes place
    std::shared_ptr<Portfolio> P;

    // Calculate returns - used in optimization
    double **returns;

    // Predict portfolio weights (individual asset quantities)
    // Invoke function for deep reinforcement learning algorithm
    auto predict_weights() noexcept -> const std::vector<double>;
};

#endif
