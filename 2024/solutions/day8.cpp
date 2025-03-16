#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>

// DAY=8 && clang++ -Wall -std=c++20 day${DAY}.cpp -o day${DAY} && ./day${DAY}

/**
 * finds the (valid) antinode locations and adds them to the containers
 *
 * an antinode occurs at any point that is perfectly in line with
 * two antennas of the same frequency - but only when one of the
 * antennas is twice as far away as the other.
 *
 */
void add_antinodes(const std::vector<std::vector<char>> &the_map, const size_t &x_pos, const size_t &y_pos, std::set<std::string> &antinodes, std::vector<std::vector<char>> &antinode_map)
{
    // find distance between two antennas of the same frequency
    char ant{the_map.at(y_pos).at(x_pos)};
    for (size_t y{0}; y < the_map.size(); ++y)
    {
        for (size_t x{0}; x < the_map.at(y).size(); ++x)
        {
            if (x == x_pos && y == y_pos)
            {
                continue;
            }

            if (ant != the_map.at(y).at(x))
            {
                continue;
            }

            // this x and y contains an antenna of the same frequency

            // now add the antinodes for these two antennas...
            // need to look in the same line, but twice the distance, either side of the two antennas
            // also need to make sure the locations are on the map...

            /*
            . . . . x .
            . . . . . .
            . . . 0 . .
            . . . . . .
            . . 0 . . .
            . . . . . .
            . x . . . .
            */

            // there should be 2 antinodes (if they're not off the map...)
            // one closer to the 'master' x_pos, y_pos point, and one closer to the found x, y point

            // find the distance
            int x_dist{std::abs(int(x_pos) - int(x))};
            int y_dist{std::abs(int(y_pos) - int(y))};

            // antinode locations
            int x1{x_pos > x ? int(x_pos) + x_dist : int(x_pos) - x_dist};
            int x2{x_pos > x ? int(x) - x_dist : int(x) + x_dist};

            int y1{y_pos > y ? int(y_pos) + y_dist : int(y_pos) - y_dist};
            int y2{y_pos > y ? int(y) - y_dist : int(y) + y_dist};

            // add antinode locations to containers (if they're valid locations)
            static constexpr char anti_char{'#'};
            if (x1 >= 0 && x1 < the_map.at(y).size() && y1 >= 0 && y1 < the_map.size())
            {
                antinodes.insert(std::to_string(x1) + "_" + std::to_string(y1));
                antinode_map.at(y1).at(x1) = anti_char;
            }
            if (x2 >= 0 && x2 < the_map.at(y).size() && y2 >= 0 && y2 < the_map.size())
            {
                antinodes.insert(std::to_string(x2) + "_" + std::to_string(y2));
                antinode_map.at(y2).at(x2) = anti_char;
            }
        }
    }
}

bool are_collinear(const int x_test, const int y_test, const int x_pos, const int y_pos, const int x_pos2, const int y_pos2)
{
    // https://math.stackexchange.com/questions/405966/if-i-have-three-points-is-there-an-easy-way-to-tell-if-they-are-collinear
    // (n-b)(x-m)=(y-n)(m-a)
    /*
    x,y -> x_pos, y_pos
    a,b -> x_pos2, y_pos2
    m,n -> x_test, y_test
    */

    return (y_test - y_pos2) * (x_pos - x_test) == (y_pos - y_test) * (x_test - x_pos2);
}

/**
 * finds the (valid) antinode locations and adds them to the containers
 *
 * an antinode occurs at any point that is perfectly in line with
 * two antennas of the same frequency - but only when one of the
 * antennas is twice as far away as the other.
 *
 */
void add_antinodes_part_2(const std::vector<std::vector<char>> &the_map, const size_t &x_pos, const size_t &y_pos, std::set<std::string> &antinodes, std::vector<std::vector<char>> &antinode_map)
{
    // find distance between two antennas of the same frequency
    char ant{the_map.at(y_pos).at(x_pos)};

    for (size_t y{0}; y < the_map.size(); ++y)
    {
        for (size_t x{0}; x < the_map.at(y).size(); ++x)
        {
            if (x == x_pos && y == y_pos)
            {
                continue; // this is the 'main' one...
            }

            if (ant != the_map.at(y).at(x))
            {
                continue; // not the same frequency
            }

            // this x and y contains an antenna of the same frequency

            // now add the antinodes for these two antennas...
            // need to look in the same line, but twice the distance, either side of the two antennas... but for p2 they can be all along the line...
            // also need to make sure the locations are on the map...

            /*
            xdist: 1
            ydist: 2

            valid antinode if x%xdist == 0 && y%ydist == 0 && must be in line... ?

            . . . . . x .   y:0, x:5
            . . . . . . .
            . . . . 0 . .   y:2, x:4
            . . . . . . .
            . . . 0 . . .   y:4, x:3
            . . . . . . .
            . . x . . . .
            . . . . . . .
            . x . . . . .

            */

            // there should be 2 antinodes (if they're not off the map...)
            // one closer to the 'master' x_pos, y_pos point, and one closer to the found x, y point

            // find the distance
            int x_dist{std::abs(int(x_pos) - int(x))};
            int y_dist{std::abs(int(y_pos) - int(y))};

            // lazy way... loop over every point...
            for (size_t y_test{0}; y_test < the_map.size(); ++y_test)
            {
                for (size_t x_test{0}; x_test < the_map.at(y_test).size(); ++x_test)
                {
                    // shift over to 0, then check if it's a multiple of the dist...
                    if ((x_test - std::min(x, x_test)) % x_dist == 0 && (y_test - std::min(y, y_test)) % y_dist == 0 && are_collinear(x_test, y_test, x_pos, y_pos, x, y))
                    {
                        // found an antinode! add it to the set...
                        // add antinode locations to containers (if they're valid locations)
                        static constexpr char anti_char{'#'};
                        antinodes.insert(std::to_string(x_test) + "_" + std::to_string(y_test));
                        antinode_map.at(y_test).at(x_test) = anti_char;
                    }
                }
            }
        }
    }
}

void print_map(std::vector<std::vector<char>> &the_map)
{
    for (const auto &line : the_map)
    {
        for (const char c : line)
        {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void test_are_collinear()
{
    /*
    . . . . . x .   y:0, x:5
    . . . . . . .
    . . . . 0 . .   y:2, x:4
    . . . . . . .
    . . . 0 . . .   y:4, x:3
    . . . . . . .
    . . x . . . .
    . . . . . . .
    . x . . . . .
    */
    int x_test{0};
    int y_test{0};
    int x_pos{10};
    int y_pos{10};
    int x_pos2{100};
    int y_pos2{10};

    std::cout << are_collinear(x_test, y_test, x_pos, y_pos, x_pos2, y_pos2) << std::endl;
}

int main()
{
    // test_are_collinear();
    // return 0;

    std::cout << std::boolalpha << "day 8" << std::endl;

    // std::ifstream infile("../input/day8_test");
    std::ifstream infile("../input/day8");

    std::vector<std::vector<char>> the_map{};

    std::string line;
    size_t line_num{0};
    while (std::getline(infile, line))
    {
        the_map.push_back(std::vector<char>{});
        std::stringstream stream{line};
        char c;
        while (stream >> c)
        {
            the_map.at(line_num).push_back(c);
        }
        ++line_num;
    }

    std::set<std::string> antinodes{};    // set of antinode locations in the format x_y
    std::set<std::string> antinodes_p2{}; // set of antinode locations in the format x_y

    std::vector<std::vector<char>> antinode_map{the_map};
    std::vector<std::vector<char>> antinode_map_p2{the_map};

    for (size_t y_main{0}; y_main < the_map.size(); ++y_main)
    {
        for (size_t x_main{0}; x_main < the_map.at(y_main).size(); ++x_main)
        {
            if (the_map.at(y_main).at(x_main) == '.')
            {
                continue;
            }
            // add_antinodes(the_map, x_main, y_main, antinodes, antinode_map);
            add_antinodes_part_2(the_map, x_main, y_main, antinodes_p2, antinode_map_p2);
        }
    }

    // print_map(antinode_map);
    print_map(antinode_map_p2);

    // std::cout << "number of antinode locations (p1): " << antinodes.size() << std::endl;
    std::cout << "number of antinode locations (p2): " << antinodes_p2.size() << std::endl;

    return 0;
}
