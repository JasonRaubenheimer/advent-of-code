#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

// https://adventofcode.com/2024/day/13

// DAY=13 && clang++ -Wall -std=c++20 day${DAY}.cpp -o day${DAY} && ./day${DAY}

struct Position
{
    size_t x;
    size_t y;
};

class Button
{
    Button(std::string raw_button, size_t cost) : m_cost{cost}
    {
    }

    // Button(size_t x_inc, size_t y_inc, size_t cost) : m_x_inc{x_inc}, m_y_inc{y_inc}, m_cost{cost} {}

    size_t m_x_inc;
    size_t m_y_inc;
    size_t m_cost; // 3 for A and 1 for B
};

class ClawMachine
{
    ClawMachine(std::string raw_button_A, std::string raw_button_B, std::string raw_prize) : m_A{raw_button_A, 3}, m_B{raw_button_B, 1}
    {
        // construct ClawMachine from strings
        // buttonA: Button A: X+69, Y+23
        // buttonB: Button B: X+27, Y+71
        // prize: Prize: X=18641, Y=10279
    }

    ClawMachine(Button A, Button B, Position prize) : m_A{A}, m_B{B}, m_prize{prize} {}

    Button m_A;
    Button m_B;
    Position m_prize; // position of the prize
};

int main()
{
    static constexpr std::string day{"13"};

    std::cout << "day " << day << std::endl;

    std::ifstream infile("../input/day" + day + "_test");
    // std::ifstream infile("../input/day" + day);

    std::vector<ClawMachine> claw_machines{};

    std::string line;
    size_t y{0};
    while (std::getline(infile, line))
    {
        // line is button A
        std::string buttonB;
        std::string prize;
        std::getline(infile, buttonB);
        std::getline(infile, prize);

        std::cout << "buttonA: " << line << std::endl;
        std::cout << "buttonB: " << buttonB << std::endl;
        std::cout << "prize: " << prize << std::endl;

        // area.push_back(std::vector<Point>{});
        // for (size_t i{0}; i < line.size(); ++i)
        // {
        //     area.at(y).emplace_back(line.at(i), i, y);
        // }
        // ++y;
    }

#define PART_1 false
#define PART_2 false

#if PART_1 == true
    size_t total_price_p1{farm.get_total_price()};
    std::cout << "part 1 --> total_price_p1: " << total_price_p1 << std::endl;
    static constexpr int p1_answer{1452678};
    if (total_price_p1 != p1_answer)
    {
        std::cout << "part 1 error!!  -> " << total_price_p1 << " != " << p1_answer << std::endl;
        std::exit(1);
    }
#endif

#if PART_2 == true
    size_t total_price_p2{farm.get_total_price_p2()};
    std::cout << "part 2 --> total_price_p2: " << total_price_p2 << std::endl;
    static constexpr int p2_answer{873584};
    if (total_price_p2 != p2_answer)
    {
        std::cout << "part 2 error!!  -> " << total_price_p2 << " != " << p2_answer << std::endl;
        std::exit(1);
    }
#endif

    return 0;
}
