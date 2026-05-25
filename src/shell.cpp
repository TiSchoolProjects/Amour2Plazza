#include <format>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <algorithm>

#include "EnumPlazza.hpp"
#include <poll.h>
#include <unistd.h>
#include "shell.hpp"

Shell::Shell(__attribute_maybe_unused__ int ac, __attribute_maybe_unused__ char **argv) :
    _pizzaTypeMap({
        {"regina", PizzaType::Regina},
        {"margarita", PizzaType::Margarita},
        {"americana", PizzaType::Americana},
        {"fantasia", PizzaType::Fantasia}
    }),
    _pizzaSizeMap({
        {"S", PizzaSize::S},
        {"M", PizzaSize::M},
        {"L", PizzaSize::L},
        {"XL", PizzaSize::XL},
        {"XXL", PizzaSize::XXL}
    })

  {
  try {
    _cookingTime = std::stod(argv[1]);
    _numberOfCooks = std::stoi(argv[2]);
    _timeToRefillMs = std::stoi(argv[3]);
    _reception = std::make_unique<Reception>(_cookingTime, _numberOfCooks, _timeToRefillMs);
    std::cout << std::format("- Time (double) : {}\n", _cookingTime);
    std::cout << std::format("- Number of Cooks (int)    : {}\n",
                             _numberOfCooks);
    std::cout << std::format("- Time to refill the kitchens (int)    : {}\n",
                             _timeToRefillMs);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

void Shell::run() {
    std::string line;
    std::regex pizza_regex(R"(^\s*([a-zA-Z]+)\s+(S|M|L|XL|XXL)\s+x([1-9][0-9]*)\s*$)");
    struct pollfd pfd;
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;
    pfd.revents = 0;
    while (true) {
        _reception->update();
        int inter = poll(&pfd, 1, 50);
        if (inter > 0 && pfd.revents & POLLIN) {
            if (!std::getline(std::cin, line))
                break;
        
            if (line == "status") {
                _reception->displayStatus(); 
                continue;
            }

            std::stringstream ss(line);
            std::string token;
            bool syntax_error = false;
            std::vector<std::pair<PizzaType, PizzaSize>> newPizzaOrder;
            while (std::getline(ss, token, ';')) {
                std::smatch matches;

                if (std::regex_match(token, matches, pizza_regex)) {
                    std::string type_str = matches[1].str();
                    std::string size_str = matches[2].str();
                    int multiplier = std::stoi(matches[3].str());
                    std::transform(type_str.begin(), type_str.end(), type_str.begin(),
                                   [](unsigned char c) { return std::tolower(c); });

                    auto type_it = _pizzaTypeMap.find(type_str);
                    auto size_it = _pizzaSizeMap.find(size_str);

                    if (type_it != _pizzaTypeMap.end() && size_it != _pizzaSizeMap.end()) {
                        PizzaType final_type = type_it->second;
                        PizzaSize final_size = size_it->second;
                        for (int i = 0; i < multiplier; ++i) {
                            newPizzaOrder.push_back({final_type, final_size});
                        }
                    } else {
                        std::cerr << "Error : unkown'" << matches[1].str() << "'" << std::endl;
                        syntax_error = true;
                        break;
                    }

                } else {
                    std::cerr << "Syntax error: '" << token << "'" << std::endl;
                    syntax_error = true;
                    break;
                }
            }
            if (!syntax_error && !newPizzaOrder.empty()) {
                _reception->handleOrder(newPizzaOrder);
            }
        }
    }
}