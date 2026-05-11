#include <string>
#include <iostream>

#include "shell.hpp"

Shell::Shell(__attribute_maybe_unused__ int ac,__attribute_maybe_unused__ char **argv)
{
    /*fill info*/
}

void Shell::run()
{
    std::string line;

    while (std::getline(std::cin, line))
    {
        std::cout << line << std::endl;
    }
    
}