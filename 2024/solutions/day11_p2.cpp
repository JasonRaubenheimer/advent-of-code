#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cmath>

// https://adventofcode.com/2024/day/11

// DAY=11 && clang++ -Wall -std=c++20 day${DAY}_p2.cpp -o day${DAY}_p2 && ./day${DAY}_p2

uint64_t num_digits(uint64_t num)
{
    uint64_t digits{0};
    while (num)
    {
        num /= 10;
        ++digits;
    }
    return digits;
}

/**
 * @brief for part 2 we can't store all the stones in a list... there are too many
 * - but each stone and all their splits are independent at each iteration...
 * - so use a recursive function to go down each path and keep splitting until we have done that many iterations
 *
 * Rules:
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
uint64_t num_stones(uint64_t stone_num, size_t iteration, uint64_t total_num_stones)
{
    if (iteration == 0)
    {
        return total_num_stones;
    }
    --iteration;

    // rule 1
    if (stone_num == 0)
    {
        // make stone_num = 1
        return num_stones(1, iteration, total_num_stones);
    }

    // rule 2
    uint64_t digit_count{num_digits(stone_num)};
    if (digit_count % 2 == 0)
    {
        // need to split into 2 stones
        // find the place to split
        uint64_t place{static_cast<uint64_t>(std::pow(10, (digit_count / 2)))};
        uint64_t new_stone_num{stone_num / place};
        stone_num = stone_num % place; // original stone num

        // now we split and go down 2 different paths
        total_num_stones = num_stones(stone_num, iteration, total_num_stones) + num_stones(new_stone_num, iteration, total_num_stones);
        return total_num_stones;
    }

    // rule 3
    stone_num *= 2024;
    total_num_stones = num_stones(stone_num, iteration, total_num_stones);
    return total_num_stones;
}

int main()
{
    static constexpr std::string day{"11"};

    std::cout << "day " << day << std::endl;

    // std::ifstream infile("../input/day" + day + "_test");
    std::ifstream infile("../input/day" + day);

    std::vector<uint64_t> stone_line_p2{};

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
                stone_line_p2.push_back(std::stoi(stone_val));
                stone_val = "";
            }
        }
    }

    // part 2
    size_t n_blink_p2{75};
    uint64_t total_stones{0};
    for (auto &num : stone_line_p2)
    {
        total_stones += num_stones(num, n_blink_p2, 1); // start with 1 stone (call function with 1 stone for each stone in the first iteration)
    }

    std::cout << "part 2 --> total_stones: " << total_stones << std::endl;
    static constexpr int p2_answer{203609};
    if (total_stones != p2_answer)
    {
        std::cout << "part 1 error!!  -> " << total_stones << " != " << p2_answer << std::endl;
        std::exit(1);
    }
    return 0;
}
