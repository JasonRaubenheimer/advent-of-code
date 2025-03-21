#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

// https://adventofcode.com/2024/day/11

// DAY=11 && clang++ -Wall -std=c++20 day${DAY}.cpp -o day${DAY} && ./day${DAY}

struct Stone
{
    Stone(uint32_t num) : m_num(num) {}

    Stone(std::string num) : m_num(std::stoi(num)) {}

    uint32_t m_num;
};

uint32_t num_digits(uint32_t num)
{
    uint32_t digits{0};
    while (num)
    {
        num /= 10;
        ++digits;
    }
    return digits;
}

/**
 *
 * - If the stone is engraved with the number 0, it is replaced by a stone
 *   engraved with the number 1.
 *
 * - If the stone is engraved with a number that has an even number of digits,
 *   it is replaced by two stones. The left half of the digits are engraved on
 *   the new left stone, and the right half of the digits are engraved on the
 *   new right stone. (The new numbers don't keep extra leading zeroes: 1000
 *   would become stones 10 and 0.)
 *
 * - If none of the other rules apply, the stone is replaced by a new stone; the
 *   old stone's number multiplied by 2024 is engraved on the new stone.
 *
 */
void blink(std::vector<Stone> &stone_line)
{
    for (size_t i{0}; i < stone_line.size(); ++i)
    {
        uint32_t stone_num{stone_line.at(i).m_num};
        // rule 1
        if (stone_num == 0)
        {
            stone_line.at(i).m_num = 1;
            continue;
        }

        // rule 2
        uint32_t digit_count{num_digits(stone_num)};
        if (digit_count % 2 == 0)
        {
            // need to split into 2 stones
            std::string str_num{std::to_string(stone_num)};
            stone_line.at(i) = Stone(str_num.substr(0, digit_count / 2));
            Stone tmp{str_num.substr((digit_count / 2) - 1, digit_count / 2)};
            stone_line.insert(stone_line.begin() + i, tmp);
            continue;
        }

        // rule 3
        stone_line.at(i).m_num = stone_num * 2024;
    }
}

int main()
{
    static constexpr std::string day{"11"};

    std::cout << "day " << day << std::endl;

    std::ifstream infile("../input/day" + day + "_test");
    // std::ifstream infile("../input/day" + day);

    std::vector<Stone> stone_line{};
    std::string line;
    while (std::getline(infile, line))
    {
        std::string stone_val{""};
        for (size_t i{0}; i < line.size(); ++i)
        {
            char c{line.at(i)};
            bool is_digit{std::isdigit(c) > 0};
            if (is_digit)
            {
                stone_val += c;
            }
            if (!is_digit || i == line.size() - 1)
            {
                // need to make a stone and add it to the vector
                stone_line.emplace_back(std::stoi(stone_val));
                stone_val = "";
            }
        }
    }

    size_t n_blink{15};
    int num{0};
    while (n_blink)
    {
        std::cout << num << std::endl;
        ++num;
        blink(stone_line);
        --n_blink;
    }

#define PART_1 true
#define PART_2 false

#if PART_1 == true
    size_t num_stones{stone_line.size()};
    static constexpr int p1_answer{698};
    if (0 != p1_answer)
    {
        std::cout << "part 1 error!!  -> " << num_stones << " != " << p1_answer << std::endl;
        std::exit(1);
    }
    std::cout << "part 1 --> num_stones: " << num_stones << std::endl;
#endif

#if PART_2 == true
#endif
    return 0;
}
