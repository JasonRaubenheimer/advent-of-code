#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

// https://adventofcode.com/2024/day/6

// clang++ -Wall -std=c++20 day6.cpp -o day6 && ./day6

// If there is something directly in front of you, turn right 90 degrees.
// Otherwise, take a step forward.

class GuardMap
{
private:
    char up_m{'^'};
    char down_m{'v'};
    char right_m{'>'};
    char left_m{'<'};

    char obs_m{'#'}; // obstacle

    enum Orientation
    {
        UP,
        DOWN,
        RIGHT,
        LEFT
    };

    std::vector<std::vector<char>> map_m{};
    size_t num_grid_pos_m{0};

    size_t gx_starting_m{0};
    size_t gy_starting_m{0};
    Orientation go_starting_m{};

    size_t gx_m{0};
    size_t gy_m{0};
    Orientation go_m{};

    std::set<std::string> positions_m{}; // record the positions in the format x_y

    bool will_move_off_map()
    {
        // returns true if the guard will leave the map with the next move
        if (go_m == Orientation::UP && gy_m == 0)
        {
            return true;
        }
        if (go_m == Orientation::DOWN && gy_m == map_m.size() - 1)
        {
            return true;
        }
        if (go_m == Orientation::RIGHT && gx_m == map_m.at(gy_m).size() - 1)
        {
            return true;
        }
        if (go_m == Orientation::LEFT && gx_m == 0)
        {
            return true;
        }
        return false;
    }

    bool is_blocked()
    {
        // returns true if there is something in front of the guard, false otherwise

        // get the orientation, then we know where to look (edges must be obstacle as well..?)
        // if(go == Orientation::UP)
        switch (go_m)
        {
        case Orientation::UP:
            if (gy_m == 0)
            {
                return true; // can't go up
            }
            if (map_m.at(gy_m - 1).at(gx_m) == obs_m)
            {
                return true;
            }
            return false;
            break;

        case Orientation::DOWN:
            if (gy_m == map_m.size() - 1)
            {
                return true; // can't go down
            }
            if (map_m.at(gy_m + 1).at(gx_m) == obs_m)
            {
                return true;
            }
            return false;
            break;

        case Orientation::LEFT:
            if (gx_m == 0)
            {
                return true; // can't go left
            }
            if (map_m.at(gy_m).at(gx_m - 1) == obs_m)
            {
                return true;
            }
            return false;
            break;

        case Orientation::RIGHT:
            if (gx_m == map_m.at(gy_m).size() - 1)
            {
                return true; // can't go right
            }
            if (map_m.at(gy_m).at(gx_m + 1) == obs_m)
            {
                return true;
            }
            return false;
            break;

        default:
            return true;
        }
    }

    void turn_right()
    {
        if (go_m == Orientation::UP)
        {
            go_m = Orientation::RIGHT;
        }
        else if (go_m == Orientation::DOWN)
        {
            go_m = Orientation::LEFT;
        }
        else if (go_m == Orientation::RIGHT)
        {
            go_m = Orientation::DOWN;
        }
        else if (go_m == Orientation::LEFT)
        {
            go_m = Orientation::UP;
        }
    }

    void step_forward()
    {
        if (go_m == Orientation::UP)
        {
            --gy_m;
        }
        else if (go_m == Orientation::DOWN)
        {
            ++gy_m;
        }
        else if (go_m == Orientation::RIGHT)
        {
            ++gx_m;
        }
        else if (go_m == Orientation::LEFT)
        {
            --gx_m;
        }
    }

    void record_position()
    {
        // record the current position
        positions_m.insert(std::to_string(gx_m) + "_" + std::to_string(gy_m));
    }

public:
    GuardMap(const std::vector<std::vector<char>> &map) : map_m(map)
    {
    }

    void init_guard()
    {
        // init the location (x,y) and orientation of the guard
        bool found{false};
        char c{};
        num_grid_pos_m = map_m.size() * map_m.at(0).size();
        for (size_t y{0}; y < map_m.size(); ++y)
        {
            for (size_t x{0}; x < map_m.at(y).size(); ++x)
            {
                c = map_m.at(y).at(x);
                if (c == up_m)
                {
                    go_m = Orientation::UP;
                    found = true;
                }
                else if (c == down_m)
                {
                    go_m = Orientation::DOWN;
                    found = true;
                }
                else if (c == right_m)
                {
                    go_m = Orientation::RIGHT;
                    found = true;
                }
                else if (c == left_m)
                {
                    go_m = Orientation::LEFT;
                    found = true;
                }

                if (found)
                {
                    gx_m = x;
                    gy_m = y;
                    gx_starting_m = x;
                    gy_starting_m = y;
                    go_starting_m = go_m;
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
    }

    bool move(bool record_position_switch = true)
    {
        // record position
        if (record_position_switch)
        {
            record_position();
        }

        // returns true if next move is on the map, false otherwise
        if (will_move_off_map())
        {
            // guard leaving the map, we are done...
            return false;
        }
        if (is_blocked())
        {
            turn_right(); // if there is something in front, turn right 90 deg
        }
        else
        {
            step_forward(); // else, step forward
        }

        return true;
    }

    size_t get_distinct_positions()
    {
        return positions_m.size();
    }

    bool is_starting_pos(size_t x, size_t y)
    {
        if (x == gx_starting_m && x == gy_starting_m)
        {
            return true;
        }
        return false;
    }

    void add_obstacle(size_t x, size_t y)
    {
        map_m.at(y).at(x) = obs_m;
    }

    void reset(size_t x, size_t y, char c)
    {
        // reset starting pos, orientation, and obstacle

        map_m.at(y).at(x) = c; // obstacle restoration

        gx_m = gx_starting_m;
        gy_m = gy_starting_m;

        go_m = go_starting_m;
    }

    bool created_loop()
    {
        // returns true if we are in a loop, false otherwise
        size_t move_count{0};
        while (move(false))
        {
            // keep moving...
            ++move_count;
            if (move_count > num_grid_pos_m)
            {
                return true;
            }
        }

        return false;

        // TODO: check if we are catching the edge cases... might be off by one errors here...
    }
};

int main()
{
    std::cout << "day 6" << std::endl;

    std::ifstream file("../input/day6");
    // std::ifstream file("../input/day6_test");
    std::string line;

    std::vector<std::vector<char>> map{};

    size_t line_num{0};

    while (std::getline(file, line))
    {
        map.push_back(std::vector<char>{});
        for (const auto &c : line)
        {
            map.at(line_num).push_back(c);
        }
        ++line_num;
    }

    GuardMap guard_map{map};
    guard_map.init_guard();

    while (guard_map.move())
    {
        // keep moving...
    }

    std::cout << "\npositions: " << guard_map.get_distinct_positions() << std::endl;

    // part 2

    /*
    - in a loop if the guard doesn't leave the map...
    - the limit is the number of positions (could subtract obstacles... probably not necessary)
    - so if the guard has moved the same number of positions as there are positions on the map, then we're in a loop

    - iterate over each grid position and add an obstacle
        - NB -> EXCEPT for where the guard currently is (just the starting position)
    */

    size_t loop_count{0};

    for (size_t y{0}; y < map.size(); ++y)
    {
        for (size_t x{0}; x < map.at(y).size(); ++x)
        {
            if (guard_map.is_starting_pos(x, y))
            {
                continue;
            }

            // add an obstacle to this position
            guard_map.add_obstacle(x, y);

            // check if we created a loop
            if (guard_map.created_loop())
            {
                ++loop_count;
            }

            // reset back to starting config
            guard_map.reset(x, y, map.at(y).at(x));
        }
    }

    std::cout << "\nloop_count: " << loop_count << std::endl;

    return 0;
}