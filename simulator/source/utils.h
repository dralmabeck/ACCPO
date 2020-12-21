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

// Includes from C++ STL in one location
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

// Return system time as unix time
void get_system_time(long &data) noexcept;

// Convert a unix time to a human readable string
auto time2str(const long &unixtime) noexcept -> const std::string;

// Calculate unix time from given time parameters
auto get_unix_time(const int &year, const int &month, const int &day, const int &hour, const int &minute, const int &second) noexcept -> long;

// Format strings for output in console
auto num2str(const double &number) noexcept -> const std::string;

#endif
