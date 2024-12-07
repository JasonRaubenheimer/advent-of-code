#include <iostream>
#include <fstream>

bool is_str_valid_num(const std::string &str)
{
    const std::string digits{"0123456789"};
    for (const char &c : str)
    {
        if (digits.find(c) == std::string::npos)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    // read in each line of the instruction

    // look for the substrings "mul(", this is the 'trigger' for a mul instruction
    // can we have this situation... mul(6,mul(...), do we need to look for that or will the algorithm in mind just handle it...?

    std::cout << "day 3" << std::endl;
    std::cout << "\n-- part 1 --" << std::endl;

    // read in the reports
    std::ifstream file("input/day3");
    // std::ifstream file("input/day3_test");
    std::string line;

    const std::string mul{"mul("};

    size_t search_pos{0};
    size_t mul_pos{0};
    size_t comma_pos{0};
    size_t close_pos{0}; // pos of the closing bracket

    std::string num_str_1{};
    std::string num_str_2{};
    int num_1{0};
    int num_2{0};

    int total{0};

    while (std::getline(file, line))
    {
        search_pos = 0;
        // find the substrings...
        for (;;)
        {
            mul_pos = line.find(mul, search_pos);
            if (mul_pos == std::string::npos)
            {
                break;
            }

            // now find the next ','... everything in between should be the first number...
            comma_pos = line.find(',', mul_pos);
            if (comma_pos == std::string::npos)
            {
                break;
            }

            num_str_1 = line.substr(mul_pos + mul.length(), comma_pos - (mul_pos + mul.length()));

            // try get an int from the num_str
            if (!is_str_valid_num(num_str_1))
            {
                // first num isn't valid, abort this mul
                // set search pos for next time
                search_pos = mul_pos + 1;
                continue;
            }
            num_1 = std::stoi(num_str_1);

            // have num_1 at this point... try get num 2
            // look for closing bracket
            close_pos = line.find(')', comma_pos);
            if (close_pos == std::string::npos)
            {
                break;
            }

            num_str_2 = line.substr(comma_pos + 1, close_pos - (comma_pos + 1));

            // try get the int
            if (!is_str_valid_num(num_str_2))
            {
                // second num isn't valid, abort this mul
                // set search pos for next time
                search_pos = mul_pos + 1;
                continue;
            }
            num_2 = std::stoi(num_str_2);

            // nums are valid!

            total += num_1 * num_2;

            // set search pos for next time
            search_pos = close_pos + 1;
        }
    }

    std::cout << "total: " << total << std::endl;

    return 0;
}