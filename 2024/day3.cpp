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

    // read in the reports
    std::ifstream file("input/day3");
    // std::ifstream file("input/day3_test");
    // std::ifstream file("input/day3_testp2");
    std::string line;

    const std::string mul_const_str{"mul("};
    const std::string enable_const_str{"do()"};
    const std::string disable_const_str{"don't()"};

    size_t search_pos{0};
    size_t mul_pos{0};
    size_t comma_pos{0};
    size_t close_pos{0}; // pos of the closing bracket

    std::string num_str_1{};
    std::string num_str_2{};
    int num_1{0};
    int num_2{0};

    int total_p1{0};
    int total_p2{0};

    bool enable{true};

    while (std::getline(file, line))
    {
        search_pos = 0;
        // find the substrings...
        for (;;)
        {
            mul_pos = line.find(mul_const_str, search_pos);
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

            num_str_1 = line.substr(mul_pos + mul_const_str.length(), comma_pos - (mul_pos + mul_const_str.length()));

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

            total_p1 += num_1 * num_2;

            // part 2 ...
            // check what the most recent do or don't was...
            std::string enable_str{line.substr(0, close_pos)}; // search whole string up to the close point...
            // and find the most recent between do() and don't()
            std::size_t enable_pos = enable_str.rfind(enable_const_str);
            std::size_t disable_pos = enable_str.rfind(disable_const_str);
            if (enable_pos > enable_str.length())
            {
                enable_pos = 0;
            }
            if (disable_pos > enable_str.length())
            {
                disable_pos = 0;
            }

            if (enable_pos > disable_pos)
            {
                enable = true;
            }
            else if (disable_pos > enable_pos)
            {
                enable = false;
            }

            if (enable)
            {
                total_p2 += num_1 * num_2;
            }

            // ... end part 2

            // set search pos for next time
            search_pos = close_pos + 1;
        }
    }

    std::cout << "total_p1: " << total_p1 << std::endl;
    std::cout << "total_p2: " << total_p2 << std::endl;

    return 0;
}