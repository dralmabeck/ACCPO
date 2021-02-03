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

#define keyFile "/Users/almabeck/kraken.txt"

class Configuration
{
public:
    // Following variables can be user defined
    // From the key file we are only reading the Kraken API keys
    // All other variables are at the moment defined here in the code

    // Need to include riskfree asset as first element in list
    const std::vector<std::string> tickerList{"ZEURZEUR","XREPZEUR",
                                              "ADAEUR","ALGOEUR","ATOMEUR","BALEUR","COMPEUR","CRVEUR","DAIEUR","DASHEUR",
                                              "DOTEUR","EOSEUR","GNOEUR","KAVAEUR","KSMEUR","LINKEUR","NANOEUR","OMGEUR","OXTEUR",
                                              "PAXGEUR","QTUMEUR","SCEUR","SNXEUR","STORJEUR","TRXEUR","WAVESEUR","XDGEUR",
                                              "XETHZEUR","XLTCZEUR","XMLNZEUR","XTZEUR","XXLMZEUR","XXMRZEUR","XXRPZEUR","XZECZEUR"};
    const std::vector<double> tickerMinimum{0.0, 0.3,
                                            50.0, 50.0, 1.0, 0.1, 0.025, 1.0, 10.0, 0.03,
                                            1.0, 3.0, 0.02, 10.0, 0.1, 2.0, 10.0, 5.0, 50.0,
                                            0.005, 5.0, 2000.0, 1.0, 50.0, 500.0, 10.0, 3000.0,
                                            0.02, 0.1, 0.1, 1.0, 30.0, 0.1, 30.0, 0.03};
    const std::string inputTime{"2020,09,30,00,00,00"};
    const std::string interVal{"1440"};
    const double tradeFee{0.0026};
    const double weightDiff{0.001};
    const long pauseApi{3};
    const long pauseProgram{30};
    const size_t lengthTxID{19};
    const bool execute{false};
    std::string apiKey, secKey;
    std::string startTime;

    // Need to include riskfree asset as first element in list
    // Which assets do we want to trade? Define here
    const std::vector<std::string> tradeableAssets{"ZEUR", "XZEC", "XREP", "XXLM", "XXMR", "DASH"};

    // Following variables are internal
    std::map<size_t, std::string> idx2str = {std::make_pair(0, "INITIAL"),
                                             std::make_pair(1, "STRATEGY")};

    static constexpr size_t INI{0};
    static constexpr size_t BAL{1};

    std::atomic<bool> terminateProgram{false};

    void readParameter(std::string &data, const std::string &parameter) noexcept;

    Configuration() noexcept;
    ~Configuration() noexcept = default;
    Configuration(const Configuration &source) = delete;
    Configuration(Configuration &&source) = delete;
    auto operator=(const Configuration &source) -> Configuration & = delete;
    auto operator=(Configuration &&source) -> Configuration & = delete;
};

inline Configuration CF;

#endif
