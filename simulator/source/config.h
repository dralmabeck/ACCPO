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

#ifndef CONFIG_H
#define CONFIG_H

#include "utils.h"

// This file contains program configuration calss

// Class for configuration of entire program
class Configuration
{
public:
    // Path to configuration file
    static constexpr char configfile[]{"../input/config.txt"};

    // Variables controlling executino of program - Read and set during startup
    // Names correspond to entires in the configuration file
    std::string apikey, seckey, asset_list, starttime, interval;
    double riskfree_quantity, trade_fee, weight_diff;
    long pause_api, pause_program;

    // Variables controlling executino of program - Read and set during startup
    std::vector<std::string> assets_names;
    std::vector<double> asset_quantities;

    // Mapping of current (inside asset structure) portfolios to indicies
    std::map<size_t, std::string> idx2str = {std::make_pair(0, "INITIAL"),
                                          std::make_pair(1, "BALANCED"),
                                          std::make_pair(2, "LATEST")};

    // Fixed name of riskfree asset
    const std::string riskfree_name{"RISKFREE"};

    static constexpr size_t RF{0};  // Position of riskfree asset inside portfolio
    static constexpr size_t INI{0}; // Position of initial current
    static constexpr size_t BAL{1}; // Position of balanced current
    static constexpr size_t LAT{2}; // Position of latest current

    // Flag to indicate key-press
    std::atomic<bool> keyPress{false};

    // Constructor reading configfile and setting configuration parameters
    explicit Configuration() noexcept;

    // Dummies to comply with the Rule of Five
    Configuration(const Configuration &source) = delete;
    Configuration(Configuration &&source) = delete;
    auto operator=(const Configuration &source) -> Configuration & = delete;
    auto operator=(Configuration &&source) -> Configuration & = delete;

    // Destructor
    ~Configuration() noexcept;

private:
    // Read and return a single parameter from the configuration file, either string or double
    static void read_parameter(std::string &data, const std::string &parameter) noexcept;
    void read_parameter(double &data, const std::string &parameter) noexcept;
    void read_parameter(long &data, const std::string &parameter) noexcept;
};

// Initialize a program wide set of the configuration parameters
inline Configuration CF;

#endif
