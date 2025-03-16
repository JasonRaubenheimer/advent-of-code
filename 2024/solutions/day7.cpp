#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// https://adventofcode.com/2024/day/7

// clang++ -Wall -std=c++20 day7.cpp -o day7 && ./day7

long to_long(const std::vector<int> &base_three_vec)
{
    long value = 0;
    for (const int digit : base_three_vec)
    {
        if (digit > 2)
        {
            throw std::invalid_argument("Invalid digit in base-3 vector");
        }
        value = value * 3 + digit; // Multiply by 3 and add the current digit
    }
    return value;
}

void to_base_three(long &val, std::vector<int> &bin_vec)
{
    static constexpr int base{3};
    std::fill(bin_vec.begin(), bin_vec.end(), 0); // clear and fill with zeros
    for (size_t bin_vec_idx{bin_vec.size()}; val > 0 && bin_vec_idx > 0;)
    {
        // decrement index first, since we're filling from the end
        bin_vec.at(--bin_vec_idx) = val % base;
        val /= base;
    }
}

long concat_nums(const long &lhs_num, const long &rhs_num)
{
    // concatenate the two nums...
    return std::stol(std::to_string(lhs_num) + std::to_string(rhs_num));
}

bool is_true(const std::vector<long> &vals, const long &test_val)
{
    if (vals.size() == 1)
    {
        return vals.at(0) == test_val;
    }

    // need to test every combination of + and *...

    // use binary counter... where 0 is + and 1 is *

    std::vector<int> bin{};

    // init with number of operators
    for (size_t i{0}; i < vals.size() - 1; ++i)
    {
        bin.push_back(0);
    }

    std::vector<long> vals_cpy{};
    for (size_t t{0}; t < (std::pow(3, bin.size())); ++t) // 'base 3'...
    {
        vals_cpy = vals; // 'reset'
        for (size_t n{0}; n < bin.size(); ++n)
        {
            // idx 0 of vals_cpy is the current 'total'
            // idx 1 is the value to the right of the operator
            if (bin.at(n) == 0)
            {
                vals_cpy.at(0) = vals_cpy.at(0) + vals_cpy.at(1);
            }
            else if (bin.at(n) == 1)
            {
                vals_cpy.at(0) = vals_cpy.at(0) * vals_cpy.at(1);
            }
            else // must be 2
            {
                /* // working...
                6   8    6   15
                6 * 8 || 6 * 15
                48 || 6 * 15
                486 * 15
                */
                vals_cpy.at(0) = concat_nums(vals_cpy.at(0), vals_cpy.at(1));
            }

            // shift to the left (just remove element at idx 1)
            vals_cpy.erase(vals_cpy.begin() + 1);
        }

        if (vals_cpy.at(0) == test_val)
        {
            return true;
        }

        // 'increment' counter...
        long val = to_long(bin);
        ++val;
        to_base_three(val, bin);
    }

    return false;
}

int main()
{
    std::cout << "day 7" << std::endl;

    std::ifstream file("input/day7");
    // std::ifstream file("input/day7_test");
    std::string line;

    long test_val{0};

    static constexpr char test_val_delimiter{':'};
    static constexpr char vals_delimiter{' '};

    size_t del_pos{0};

    std::vector<long> vals{};

    long sum_true{0};

    while (std::getline(file, line))
    {
        // first split and get number before the colon, this is the 'test value'
        del_pos = line.find(test_val_delimiter);
        test_val = std::stol(line.substr(0, del_pos));

        // then create a vector of the values after the colon
        std::stringstream ss{line.substr(del_pos + 2)}; // +2 because ': '

        while (ss.good())
        {
            std::string substr{};
            std::getline(ss, substr, vals_delimiter);
            vals.push_back(std::stol(substr));
        }

        // then check if the equation can be made true
        if (is_true(vals, test_val))
        {
            sum_true += test_val;
        }

        vals.clear();
    }

    std::cout << "sum_true: " << sum_true << std::endl;
}