# CPPND: ACCPO - Autonomous CryptoCurrency Portfolio Optimization

This program is fully written in modern C++17.

This is a first draft of a Cryptocurrency trading bot simulator working with the Kraken exchange API.
The program continously and automatically polls Kraken API to retrieve data and performs operation in infinite loop until manually terminated.
Given an initial crypto portfolio, the bot automatically adapts a strategy balancing all crypto assets (and one riskfree asset, such as EUR or USD) to equal portfolio weight.

## Dependencies for Running Locally

* cmake >= 2.8:
    * All OSes: click here for installation instructions [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
    * Linux: make is installed by default on most Linux distros
    * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
    * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4:
    * Linux: gcc / g++ is installed by default on most Linux distros
    * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
    * Windows: recommend using [MinGW](http://www.mingw.org/)
* Kraken C API:
    * Platform-Independent Source Code: Compile the dynamic [Kraken C API library] (https://github.com/hANSIc99/Kraken_C_API)
* JSON for Modern C++ >= 3.9.1:
    * Source Code by Nils Lohmann and include [single_include header](https://github.com/nlohmann/json)

Both thirdparty libraries (Kraken C API and JSON for Modern C++) need to be placed in the `thirdparty` folder.

## Basic Build Instructions

1. Make a build directory in the top level directory: `mkdir build && cd build`.
2. Compile: `cmake .. && make`.
3. Run it: `./ACCPO`.

## Advanced Usage Options

1. See the basic build instrutions
2. Open the `input/config.txt` file
3. Set your preferred options.
4. Run the program.
5. Observe the trading performance

## Options in configuration file

Available options include:

* `API_KEY api_key` ### Your private key to Kraken API
* `SEC_KEY sec_key` ### Your secure key to Kraken API
* `RISKFREE_QUANTITY 3000` ### Initial quantity of riskfree asset
* `STARTTIME 2020,08,31,00,00,00` ### Startdate for download of historical data
* `INTERVAL 1440` ### Time interval between historical datapoints
* `TRADE_FEE 0.0026` ### Fee for trading (in percent)
* `WEIGHT_DIFF 0.02` ### Difference in portfolio weights before optimization kicks in
* `PAUSE_API 3` ### Pause Kraken API between each call
* `PAUSE_PROG 30` ### Pause program each iteration in infinite loop
* `AVAILABLE_TICKERS ADAUR,ALGOEUR,ATOMEUR,BALEUR,BATEUR,BCHEUR,COMPEUR,CRVEUR,DAIEUR,DASHEUR,DOTEUR,EOSEUR,GNOEUR,ICXEUR,KAVAEUR,KNCEUR,KSMEUR,LINKEUR,LSKEUR,NANOEUR,OMGEUR,OXTEUR,PAXGEUR,QTUMEUR,SCEUR,SNXEUR,STORJEUR,TRXEUR,WAVESEUR,XDGEUR,XETCZEUR,XETHZEUR,XLTCZEUR,XMLNZEUR,XREPZEUR,XTZEUR,XXBTZEUR,XXLMZEUR,XXMRZEUR,XXRPZEUR,XZECZEUR` ### List of possible crypto assets in Kraken format
* `ASSET_LIST XXRPZEUR,XXMRZEUR,XZECZEUR` ### Asset ticker symbols in Kraken format (length needs to match asset_quantities)
* `ASSET_QUANTITIES 1000,5,10` ### Initial quantities of assets (length needs to match asset_list)

## Structure of folders and files

* `\input\config.txt` - ACCPO configuration file
* `\source\*` - ACCPO source code folder
* `\thirdparty\*`- contains Kraken C API and JSON library
* `CMakeLists.txt`- cmake configuration file
* `LICENSE` - License for released ACCPO
* `README.md`- this file

## License

This software is released under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007.

You should have received a copy of the GNU General Public License along with this program.
If not, see <https://www.gnu.org/licenses/>
