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

#include "config.h"

Configuration::Configuration() noexcept
{
    readParameter(apiKey, "API_KEY");
    readParameter(secKey, "SEC_KEY");

    std::stringstream streamStarttime(inputTime);
    std::vector<int> utc;
    while (streamStarttime.good())
    {
        std::string substring;
        std::getline(streamStarttime, substring, ',');
        utc.emplace_back(std::stoi(substring));
    }

    startTime = std::to_string(getUnixTime(utc[0], utc[1], utc[2], utc[3], utc[4], utc[5]));
    std::cout << "Configuration constructor() executed." << std::endl;
}

void Configuration::readParameter(std::string &data, const std::string &parameter) noexcept
{
    std::string line;

    std::string key;

    std::string value;
    std::ifstream stream(keyFile);

    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            std::istringstream lineStream(line);
            while (lineStream >> key >> value)
            {
                if (key == parameter)
                {
                    data = value;
                }
            }
        }
    }
    stream.close();
    std::cout << "readParameter() executed for parameter: " << parameter << " with value " << data << std::endl;
}
