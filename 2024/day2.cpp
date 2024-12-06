#include <iostream>
#include <fstream>
#include <vector>

bool is_safe(const std::vector<int> &report)
{
    // safe if:
    // 1. The levels are either all increasing or all decreasing.
    // 2. Any two adjacent levels differ by at least one and at most three.

    bool is_increasing{true};
    bool is_decreasing{true};
    bool diff_ok{true};
    int diff{0};
    for (size_t i{0}; i < report.size() - 1; ++i)
    {
        // check 1: increasing
        if (report.at(i) > report.at(i + 1))
        {
            is_increasing = false;
        }

        // check 1: decreasing
        if (report.at(i) < report.at(i + 1))
        {
            is_decreasing = false;
        }

        // check 2: differ
        diff = std::abs(report.at(i) - report.at(i + 1));
        if (diff < 1 || diff > 3)
        {
            diff_ok = false;
        }

        // exit early if unsafe
        if ((!is_increasing && !is_decreasing) || !diff_ok)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    std::cout << "day 2" << std::endl;
    std::cout << "\n-- part 1 --" << std::endl;

    // read in the reports
    std::ifstream file("input/day2");
    // std::ifstream file("input/day2_test");
    std::string line;
    size_t safe_count{0}; // a count of how many reports are safe
    std::vector<int> report{};
    std::string num{}; // construct a num string from the chars around the spaces
    char c{};
    while (std::getline(file, line))
    {
        report.clear(); // clear reusable report container

        // each line is a report
        // convert each string into a vector of numbers
        // each number is separated by a space
        for (size_t i{0}; i < line.length(); ++i)
        {
            c = line.at(i);
            num += c;
            if (c == ' ' || i == line.length() - 1)
            {
                if (num.length() > 0)
                {
                    // we have created a num, store it in the vector as an int
                    report.push_back(std::stoi(num)); // num might have a ' ' but stoi handles that
                    num.clear();
                }
                continue;
            }
        }

        if (is_safe(report))
        {
            ++safe_count;
        }
    }

    std::cout << "safe_count: " << safe_count << std::endl;

    return 0;
}