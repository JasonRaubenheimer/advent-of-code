#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// https://adventofcode.com/2024/day/7

// clang++ -Wall -std=c++20 day7.cpp -o day7 && ./day7

long to_long(const std::vector<bool> &bin_vec)
{
    long value{0};
    for (const bool bit : bin_vec)
    {
        value = (value << 1) | bit; // left shift and add the bit
    }
    return value;
}

void to_bin(long &val, std::vector<bool> &bin_vec)
{
    std::fill(bin_vec.begin(), bin_vec.end(), false); // clear and fill with zeros
    for (size_t bin_vec_idx{bin_vec.size()}; val > 0 && bin_vec_idx > 0;)
    {
        // decrement index first, since we're filling from the end
        bin_vec.at(--bin_vec_idx) = val % 2;
        val /= 2;
    }
}

bool is_true(const std::vector<long> &vals, const long &test_val)
{
    if (vals.size() == 1)
    {
        return vals.at(0) == test_val;
    }

    // need to test every combination of + and *...

    // use binary counter... where 0 is + and 1 is *

    std::vector<bool> bin{};

    // init with number of operators
    for (size_t i{0}; i < vals.size() - 1; ++i)
    {
        bin.push_back(0);
    }

    for (size_t n{0}; n < (std::pow(2, bin.size())); ++n)
    {
        long total{vals.at(0)};
        for (size_t n{0}; n < bin.size(); ++n)
        // for (const bool &v : bin)
        {
            if (bin.at(n) == 0)
            {
                total += vals.at(n + 1);
            }
            else // else v must be 1...
            {
                total *= vals.at(n + 1);
            }
        }

        if (total == test_val)
        {
            return true;
        }

        // 'increment' counter...
        long val = to_long(bin);
        ++val;
        to_bin(val, bin);
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