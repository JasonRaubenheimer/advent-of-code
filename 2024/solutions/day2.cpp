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

    // for part 2
    std::vector<std::vector<int>> reports{}; // hold all the reports for part 2 processing

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

        // add report to vector for part 2 processing
        reports.push_back(report);

        if (is_safe(report))
        {
            ++safe_count;
        }
    }

    std::cout << "safe_count: " << safe_count << std::endl;

    std::cout << "\n-- part 2 --" << std::endl;

    // problem dampener...
    // do this in the brute force way...
    // if a report isn't safe, remove each element and re-run the check

    // loop over each report
    size_t safe_count_p2{0};
    for (const std::vector<int> &report_p2 : reports) // can use auto...
    {
        if (is_safe(report_p2))
        {
            ++safe_count_p2;
            continue; // nothing else to do here...
        }

        // now keep removing different elements to see if it makes it safe
        std::vector<int> report_copy{report_p2}; // so we can modify it
        int removed_val{0};

        // loop over each element, because we want to test removing each element
        for (size_t i{0}; i < report_p2.size(); ++i)
        {
            // store and remove val
            removed_val = report_p2.at(i);
            std::vector<int>::const_iterator it = report_copy.begin() + i;
            report_copy.erase(it);

            // check if safe
            if (is_safe(report_copy))
            {
                ++safe_count_p2;
                break; // it's safe! check the next report...
            }

            // not safe... add value back before checking the next one
            report_copy.insert(it, removed_val);
        }
    }

    std::cout << "safe_count_p2: " << safe_count_p2 << std::endl;

    return 0;
}