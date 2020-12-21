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

#include "accpo.h"

// Main function of ACCPO logic
void accpo() noexcept
{
    // Initialize Kraken API
    std::unique_ptr<Kraken> K = std::make_unique<Kraken>(CF.apikey, CF.seckey);

    // Obtain and output Kraken servertime
    long servertime, systemtime;
    // Obtain and output local system time    
    K->get_server_time(servertime);
    get_system_time(systemtime);

    std::cout << "Current servertime is: " << time2str(servertime) << std::endl;
    std::cout << "Current systemtime is: " << time2str(systemtime) << std::endl;
    // Check if Kraken and local server are in sync
    std::cout << "The current time delay is " << fabs(static_cast<double>(servertime - systemtime)) << " seconds." << std::endl;

    // Create vector of assets of non-riskfree assets
    std::vector<std::unique_ptr<Asset>> asset_vector;
    for (size_t asset(0); asset < CF.assets_names.size(); asset++)
    {
        // Create new asset
        std::unique_ptr<Asset> new_asset = std::make_unique<Asset>(CF.assets_names[asset]);

        // Fill asset with historic data from Kraken
        K->pause_api(CF.pause_api);
        K->get_ohlc_data(new_asset->historic, CF.assets_names[asset], CF.starttime, CF.interval);
        // Set quantity of asset
        new_asset->set_historic_quantity(CF.asset_quantities[asset], 0);

        // Move asset into asset vector
        asset_vector.push_back(std::move(new_asset));
    }

    // Create a riskfree asset
    std::unique_ptr<Asset> RiskFree = std::make_unique<Asset>(CF.riskfree_name);
    // Use same timebins as in non-riskfree assets
    RiskFree->fill_historic_riskfree(asset_vector[0]->historic.time);
    // Set quantity of riskfree
    RiskFree->set_historic_quantity(CF.riskfree_quantity, 0);

    // Fetch from Kraken API all current latest ticker information (all assets)
    K->pause_api(CF.pause_api);
    K->fetch_all_tickers(CF.asset_list);

    // Set the initial current struct to latest data
    // For Riskfree
    RiskFree->set_current_riskfree(CF.INI);
    for (auto const &asset : asset_vector)
    {
        // For all non-riskfree assets
        K->get_ticker_data(asset->current[CF.INI], asset->get_name());
    }

    // Create a portfolio and set riskfree asset as first element
    std::shared_ptr<Portfolio> P = std::make_shared<Portfolio>(std::move(RiskFree));
    RiskFree = nullptr;
    for (auto &asset : asset_vector)
    {
        // Add all other assets int the portfolio
        P->add_asset(std::move(asset));
        asset = nullptr;
    }

    // Initialize an instance of the optimizer and put the portfolio into it
    std::unique_ptr<Optimizer> O = std::make_unique<Optimizer>(P);

    // Output the historic state before any optimization
    std::cout << P->history_output(0, P->number_timebins() - 1).str();
    // Perform optimization on historic data
    O->history_calculate();
    // Output the historic state after optimization
    std::cout << P->history_output(0, P->number_timebins() - 1).str();

    // Initialize the current structs of all assets
    O->current_initialize();
    
    // Output the initial current state before any optimization
    std::cout << P->current_output_state(CF.INI).str();
    // Optimize and output the potential trades for a balanced current
    std::cout << P->current_output_trade(CF.INI, CF.BAL).str();
    // Output the balanced current state after optimization
    std::cout << P->current_output_state(CF.BAL).str();

    std::cout << "..............................................................................................." << std::endl;
    std::cout << "Entering infinite polling loop now." << std::endl;

    long loopCounter(1);
    // Entering an infinite loop polling regularly new data and performing optimization and output functions
    while (!CF.keyPress.load())
    {
        std::cout << "###############################################################################################################" << std::endl;
        std::cout << "Infinite polling loop iteration number " << std::to_string(loopCounter) << std::endl;
        std::cout << "Program is paused for: " << std::to_string(CF.pause_program) << " seconds." << std::endl;
        std::cout << "Press KEY + ENTER to exit loop and terminate program." << std::endl;
        std::cout << "###############################################################################################################" << std::endl;
        
        // Put the program to rest before polling Kraken API for new data and performing optimization
        std::this_thread::sleep_for(std::chrono::seconds(CF.pause_program));

        // Update servertiem and system time afte wakeup from sleep
        K->get_server_time(servertime);
        get_system_time(systemtime);

        std::cout << "Current servertime is: " << time2str(servertime) << std::endl;
        std::cout << "Current systemtime is: " << time2str(systemtime) << std::endl;
        // Check synchronization between Kraken server and local system
        std::cout << "The current time delay is " << fabs(static_cast<double>(servertime - systemtime)) << " seconds." << std::endl;

        if (loopCounter > 1)
        {
            for (auto &asset : P->assets)
            {
                // We move the lastest current struct into the balanced current struct
                // Since we are using logic with three currents (initial, balanced and latest)
                asset->copy_current_data(CF.LAT, CF.BAL);
            }
        }

        // Pause Kraken API for short time to prevent floodign and being locked out
        K->pause_api(CF.pause_api);
        // Fetch all Tickers
        K->fetch_all_tickers(CF.asset_list);

        // Set the latest current for the riskfree asset
        P->assets[CF.RF]->set_current_riskfree(CF.LAT);
        
        // Fill current with latest ticker information from Kraken API all non-riskfree assets
        for (size_t asset(1); asset < P->number_assets(); asset++)
        {
            K->get_ticker_data(P->assets[asset]->current[CF.LAT], P->assets[asset]->get_name());
         }

        std::stringstream output_buffer = P->current_output_performance(CF.INI, CF.LAT);

        std::cout << P->current_output_state(CF.INI).str();
        std::cout << P->current_output_trade(CF.INI, CF.BAL).str();
        std::cout << P->current_output_state(CF.BAL).str();
        O->current_update(CF.BAL, CF.LAT);
        std::cout << P->current_output_trade(CF.BAL, CF.LAT).str();
        std::cout << P->current_output_state(CF.LAT).str();

        std::cout << output_buffer.str();

        long last_time(P->idx_time(P->number_timebins() - 1));
        std::cout << "Last historic time was: " << time2str(last_time) << std::endl;

        double next_historic(std::stod(CF.interval) - static_cast<double>(P->current_time(CF.LAT) - last_time) / 60.0);

        std::cout << "New historic data will become available in: ";
        std::cout << std::to_string(next_historic) << " minutes (interval = " << CF.interval << " minutes)." << std::endl;

        loopCounter++;
    }
}

// Main function of executable
auto main() -> int
{
    auto progStart(std::chrono::high_resolution_clock::now());

    std::cout << "###############################################################################################################" << std::endl;
    std::cout << "###### Autonomous Autonomous ##################################################################################" << std::endl;
    std::cout << "############################ CryptoCurrency CryptoCurrency ####################################################" << std::endl;
    std::cout << "########################################################## Portfolio Portfolio ################################" << std::endl;
    std::cout << "############################################################################## Optimization Optimization ######" << std::endl;
    std::cout << "###############################################################################################################" << std::endl;

    // Run ACCPO in second thread
    std::thread accpoThread = std::thread(accpo);

    // Read key stokes
    std::string key_input;
    std::cin >> key_input;
    // Set the flag with true to break the loop.
    CF.keyPress.store(true);

    std::cout << "###################################################################################################################" << std::endl;
    std::cout << "Keystroke detected." << std::endl;
    std::cout << "Exiting main loop and stopping program execution." << std::endl;
    for (size_t fop(0); fop<5; fop++)
    {
        std::cout << "Please wait for final operations .............................................................................." << std::endl;
    }
    
    // Wait for the accpo thread to finish.
    accpoThread.join();

    std::cout << "###############################################################################################################" << std::endl;
    std::cout << "Shutting down Autonomous CryptoCurrency Portfolio Optimization program." << std::endl;
    std::cout << "###############################################################################################################" << std::endl;

    auto progEnd(std::chrono::high_resolution_clock::now());
    auto progTime(static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(progEnd - progStart).count()) / 60.0);

    std::cout << "Program execution lasted for: " << num2str(progTime) << " minutes." << std::endl;

    return 0;
}
