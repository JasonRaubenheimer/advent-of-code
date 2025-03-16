#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// clang++ -Wall -std=c++20 day5.cpp -o day5 && ./day5

bool rules_ok(const std::vector<std::vector<int>> &rules, const std::vector<int> &update)
{
    // for each rule, if the update contains both numbers X and Y,
    // X must come before Y
    size_t xpos{0};
    size_t ypos{0};
    for (const auto &pages : rules)
    {
        xpos = std::distance(update.begin(), find(update.begin(), update.end(), pages.at(0)));
        ypos = std::distance(update.begin(), find(update.begin(), update.end(), pages.at(1)));

        if (xpos < update.size() && ypos < update.size())
        {
            // it includes both numbers... now check the order
            if (ypos < xpos)
            {
                return false;
            }
        }
    }

    return true;
}

void fix_order(const std::vector<std::vector<int>> &rules, std::vector<int> &update)
{
    // update is fixed in place
    size_t xpos{0};
    size_t ypos{0};
    for (const auto &pages : rules)
    {
        xpos = std::distance(update.begin(), find(update.begin(), update.end(), pages.at(0)));
        ypos = std::distance(update.begin(), find(update.begin(), update.end(), pages.at(1)));

        if (xpos < update.size() && ypos < update.size())
        {
            // it includes both numbers... now check the order
            if (ypos < xpos)
            {
                // can we just swop the numbers..?
                // this will probably break other rules..?
                int xval{update.at(xpos)};
                update.at(xpos) = update.at(ypos);
                update.at(ypos) = xval;
            }
        }
    }
}

int main()
{
    std::cout << "day 5" << std::endl;

    std::ifstream file("../input/day5");
    // std::ifstream file("../input/day5_test");
    std::string line;

    std::vector<std::vector<int>> rules{};

    char rule_delimiter{'|'};

    int rule_x{0};
    int rule_y{0};

    int total{0};
    int total_p2{0};

    bool get_rules{true};
    std::vector<int> update{};
    while (std::getline(file, line))
    {
        // get rules
        if (get_rules)
        {
            if (line == "")
            {
                get_rules = false;
                continue;
            }

            rule_x = std::stoi(line.substr(0, line.find(rule_delimiter, 0)));
            rule_y = std::stoi(line.substr(line.find(rule_delimiter, 0) + 1));
            rules.push_back(std::vector{rule_x, rule_y});
        }
        else
        {
            // get updates...
            std::stringstream ss{line};
            while (ss.good())
            {
                std::string substr;
                std::getline(ss, substr, ',');
                update.push_back(std::stoi(substr));
            }

            if (rules_ok(rules, update))
            {
                // find middle page number and add it to the total
                size_t middle_idx{(update.size() - 1) / 2};
                total += update.at(middle_idx);
            }
            else
            {
                do
                {
                    // part 2 - correctly order the ones that break the rules...
                    fix_order(rules, update);
                } while (!rules_ok(rules, update));
                // find middle page number and add it to the total
                size_t middle_idx{(update.size() - 1) / 2};
                total_p2 += update.at(middle_idx);
            }

            update.clear();
        }
    }

    std::cout << "\npart1: " << total << std::endl;
    std::cout << "\npart2: " << total_p2 << std::endl;

    return 0;
}