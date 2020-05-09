/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** main
*/

#include <iostream>
#include "Reception.hpp"

static void help(const char *binName)
{
    std::cerr << "USAGE: " << binName << " <multiplier> <cooks> <refill>" << std::endl
              << "\tmultiplier\tcooking time multiplier" << std::endl
              << "\tcooks\tnumber of cooks per kitchen" << std::endl
              << "\trefill\ttime interval between ingredients stocks refills (in ms)" << std::endl;
}

int main(int argc, char const *argv[])
{
    unsigned int timeMultiplier = 0;
    unsigned int cooksPerKitchen = 0;
    unsigned int restoreDelay = 0;

    if (argc != 4) {
        if (argc == 2 && std::strcmp(argv[1], "-h")) {
            help(argv[0]);
            return (0);
        } else {
            return (84);
        }
    }
    timeMultiplier = std::atoi(argv[1]);
    cooksPerKitchen = std::atoi(argv[2]);
    restoreDelay = std::atoi(argv[3]);
    Reception reception(timeMultiplier, cooksPerKitchen, restoreDelay);

    try {
        reception.start();
    } catch (...) {
        return (84);
    }
    return (0);
}
