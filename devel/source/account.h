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

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "kraken.h"
#include "utils.h"

class Kraken;

class Account
{
public:
    std::vector<std::string> assets{};
    std::vector<std::string> tickers{};
    std::vector<double> minimums{};
    std::vector<double> prices{};
    std::vector<double> quants{};
    std::vector<bool> active{};

    double pvInitialAll{};
    long lastQueryTime{};
    std::shared_ptr<Kraken> K;

    [[nodiscard]] auto pvAll() const noexcept -> double;

    void updateAccount() noexcept;
    [[nodiscard]] auto printAccount() noexcept -> std::stringstream;

    explicit Account(const std::shared_ptr<Kraken>& K) noexcept;
    ~Account() noexcept = default;
    Account(const Account &source) = delete;
    Account(Account &&source) = delete;
    auto operator=(const Account &source) -> Account & = delete;
    auto operator=(Account &&source) -> Account & = delete;
};

#endif
