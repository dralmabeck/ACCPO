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

#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <atomic>
#include <algorithm>

struct Trade
{
    std::string ticker;
    std::string txid;
    std::string market{};
    double quantity{};
    double price{};
    double limit{0.0};
    bool executed{false};
    bool placed{false};
};

#define bool2str(input) ((input) ? "true" : "false")

void getSystemTime(long &data) noexcept;
auto getUnixTime(const int &years, const int &months, const int &days, const int &hours, const int &minutes, const int &seconds) noexcept -> long;

auto time2str(const long &unixtime) noexcept -> std::string;
auto num2str(const double &number, const int &precision) noexcept -> std::string;
auto vec2str(const std::vector<std::string> &input) noexcept -> std::string;

auto compareTradePipeline(const Trade &first, const Trade &second) -> bool;

#endif
