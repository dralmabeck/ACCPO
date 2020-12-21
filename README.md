# CPPND: ACCPO - Autonomous CryptoCurrency Portfolio Optimization

This program is fully written in modern C++17.

This is development draft of a Cryptocurrency trading bot working with the Kraken exchange API.

### Simulator program:
The program continously and automatically polls Kraken API to retrieve data and performs operation in infinite loop until manually terminated.
Given an initial crypto portfolio, the bot automatically adapts a strategy balancing all crypto assets (and one riskfree asset, such as EUR or USD) to equal portfolio weight.
There is no live-trading or trading API access included, so this is really just a simulator program.

## Structure of folders and files

* `\devel\` - ACCPO development version - Including live-trading algorithm, one-time optimization
* `\simulator\` - ACCPO simulator folder - Bot simulator, no live-trading, infinite polling loop
* `LICENSE` - License for released ACCPO
* `README.md`- this file

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
* PyTorch C++ API 
    * C++ Distribution: Obtain and install from here [PyTorch C++ API] (https://pytorch.org/cppdocs/installing.html)

All thirdparty libraries (Kraken C API, JSON for Modern C++ and PyTorch C++ API) need to be placed in the `thirdparty` folder.

## License

This software is released under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007.

You should have received a copy of the GNU General Public License along with this program.
If not, see <https://www.gnu.org/licenses/>
