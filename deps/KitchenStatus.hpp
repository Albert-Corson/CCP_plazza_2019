/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** KitchenStatus
*/

#pragma once

#include <regex>
#include <tuple>

#include "pizzaSize.hpp"

struct KitchenStatus {
    size_t activeCooks = 0;
    size_t totalCooks = 0;
    size_t orderQueueCapacity = 0;
    std::vector<std::tuple<std::string, pizzaSize_t, bool>> orderQueue;

    // returns a string buffer containing the kitchen infos in the following format:
    // <activeCooks> <totalCooks> <orderQueueCapacity> [<pizza> <size> <status>]...
    const std::string &serialize() const noexcept
    {
        static std::string buffer;

        buffer.clear();
        buffer += std::to_string(activeCooks);
        buffer += ' ';
        buffer += std::to_string(totalCooks);
        buffer += ' ';
        buffer += std::to_string(orderQueueCapacity);
        for (const auto &it : orderQueue) {
            buffer += ' ';
            buffer += std::get<0>(it);
            buffer += ' ';
            buffer += std::to_string(int(std::get<1>(it)));
            buffer += ' ';
            buffer += std::to_string(std::get<2>(it));
        }
        return (buffer);
    }

    // parses a string buffer formatted the way of `dump()`'s output
    bool deserialize(const std::string &line)
    {
        std::regex format("^ *([0-9]+) +([0-9]+) +([0-9]+) *");
        std::smatch matches;

        if (!std::regex_search(line, matches, format))
            return (false);
        try {
            activeCooks = std::stoul(matches.str(1));
            totalCooks = std::stoul(matches.str(2));
            orderQueueCapacity = std::stoul(matches.str(3));

            format = "^ *([A-z0-9_-]+) +([0-5]) +([01]) *";
            std::string buffer = matches.suffix();
            while (buffer.size()) {
                if (!std::regex_search(buffer, matches, format))
                    return (false);
                orderQueue.emplace_back(matches.str(1), pizzaSize_t(std::stoi(matches.str(2))), std::stoi(matches.str(3)));
                buffer = matches.suffix();
            }
        } catch (...) {
            return (false);
        }
        return (true);
    }

    // parses a string vector formatted the way of `dump()`'s output
    // ie: {<activeCooks>, <totalCooks>, <orderQueueCapacity>, [<pizza>, <size>, <status>,]...}
    bool deserialize(const std::vector<std::string> &words, size_t offset = 0)
    {
        try {
            activeCooks = std::stoul(words[offset + 0]);
            totalCooks = std::stoul(words[offset + 1]);
            orderQueueCapacity = std::stoul(words[offset + 2]);

            const auto end = words.cend();
            for (auto it = words.cbegin() + offset + 3; it != end; it += 3) {
                orderQueue.emplace_back(*it, pizzaSize_t(std::stoi(*(it + 1))), std::stoi(*(it + 2)));
            }
        } catch (...) {
            return (false);
        }
        return (true);
    }

    // dumps infos in a human-readable format
    void dump(std::ostream &output) const noexcept
    {
        size_t ongoing = 0;

        for (const auto &it : orderQueue) {
            if (std::get<2>(it))
                ++ongoing;
        }
        output << "Cooks: " << totalCooks << " (" << activeCooks << " working)" << std::endl
               << "Orders: " << ongoing << "/" << orderQueue.size() << " ongoing (queue capacity: " << orderQueueCapacity << ")" << std::endl;
        for (const auto &it : orderQueue) {
            output << "\t" << std::get<0>(it) << " " << pizzaSizeStr.at(std::get<1>(it)) << ": ";
            if (std::get<2>(it))
                output << "ongoing";
            else
                output << "awaiting";
            output << std::endl;
        }
    }
};

std::string &operator<<(std::string &lhs, const KitchenStatus &rhs)
{
    lhs = rhs.serialize();
    return (lhs);
}

std::ostream &operator<<(std::ostream &lhs, const KitchenStatus &rhs)
{
    lhs << rhs.serialize();
    return (lhs);
}

KitchenStatus &operator>>(const std::vector<std::string> &lhs, KitchenStatus &rhs)
{
    rhs.deserialize(lhs);
    return (rhs);
}

KitchenStatus &operator>>(const std::string &lhs, KitchenStatus &rhs)
{
    rhs.deserialize(lhs);
    return (rhs);
}
