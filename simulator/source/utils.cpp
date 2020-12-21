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

#include "utils.h"

void get_system_time(long &data) noexcept
{
    time_t result(std::time(nullptr));
    std::asctime(std::localtime(&result));

    data = static_cast<long>(result);
}

auto time2str(const long &unixtime) noexcept -> const std::string
{
    time_t ut(unixtime);
    char converted[100];
    std::strftime(converted, sizeof(converted), "%F - %T", std::localtime(&ut));

    return std::string(converted);
}

auto get_unix_time(const int &years, const int &months, const int &days, const int &hours, const int &minutes, const int &seconds) noexcept -> long
{
    struct tm timeinfo;
    timeinfo.tm_year = years - 1900;
    timeinfo.tm_mon = months - 1;
    timeinfo.tm_mday = days;
    timeinfo.tm_hour = hours;
    timeinfo.tm_min = minutes;
    timeinfo.tm_sec = seconds;

    return static_cast<long>(timegm(&timeinfo));
}

auto num2str(const double &number) noexcept -> const std::string
{
    std::ostringstream stream;
    stream << std::fixed;
    stream << std::setprecision(4);
    stream << std::setw(10);
    stream << std::setfill(' ');
    stream << number;

    return stream.str();
}
