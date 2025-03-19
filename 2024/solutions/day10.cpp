#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>

// https://adventofcode.com/2024/day/10

// DAY=10 && clang++ -Wall -std=c++20 day${DAY}.cpp -o day${DAY} && ./day${DAY}

/*

- if 0 -> possible start of trail
    - search up/down/left/right for a 1
        - if found -> continue down that path
            --> could follow multiple paths - recursion?


- a trailhead's (0 starting position) score is the number of 9-height
  positions that are reachable

things to handle:
- multiple routes from 0 to 9 -> should only count as 1

*/

enum Direction
{
    up,
    down,
    left,
    right
};

/**
 * A single position on the map
 */
class TrailPoint
{
public:
    TrailPoint(uint32_t val) : m_value{val}, m_score{0}, m_been_here{false} {}

    /**
     * @brief returns true if the value == 0 (i.e. this is the start of a trail)
     */
    bool is_trailhead() const
    {
        return m_value == 0;
    }

    /**
     * @brief returns true if the value == 9 (i.e. this is the end of a trail)
     */
    bool is_peak() const
    {
        return m_value == 9;
    }

    /**
     * @brief returns true if we have been here before
     */
    bool been_here_before() const
    {
        return m_been_here;
    }

    /**
     * @brief set's the 'been here' bool to true
     */
    void set_been_here()
    {
        m_been_here = true;
    }

    /**
     * @brief set the trailhead score
     */
    void set_trailhead_score(const uint32_t &score)
    {
        // must be a trailhead otherwise what are you doing?
        if (m_value != 0)
        {
            std::terminate();
        }
        m_score = score;
    }

    uint32_t get_point_value() const
    {
        return m_value;
    }

    uint32_t get_score() const
    {
        return m_score;
    }

private:
    uint32_t m_value;
    uint32_t m_score; // only relevant for trailhead
    bool m_been_here;
};

/**
 * @brief the 'hiker' that tries to find a trail
 */
class Hiker
{

public:
    using MapT = std::vector<std::vector<TrailPoint>>;

    Hiker(MapT map) : m_map{map} {}

    uint32_t find_trailhead_scores()
    {
        // loop over each trail point
        //     if trail point == trailhead
        //         search for a route to a number 9
        //             at each position, see if a number exists that's > current number
        //                 if more than one exists... need to search both paths

        uint32_t total_score{0U};

        for (size_t y{0}; y < m_map.size(); ++y)
        {
            for (size_t x{0}; x < m_map.at(y).size(); ++x)
            {
                if (m_map.at(y).at(x).is_trailhead())
                {
                    // need to search for a route to a 9
                    // but can be a split in the trail
                    // only increasing numbers (by 1)
                    // "as long as possible" <- not sure how important this is
                    m_map.at(y).at(x).set_trailhead_score(get_trailhead_score(x, y, m_map));
                    total_score += m_map.at(y).at(x).get_score();
                }
            }
        }

        print_trailhead_scores_in_reading_order();

        return total_score;
    }

private:
    void print_trailhead_scores_in_reading_order()
    {
        return;
        for (const auto &row : m_map)
        {
            for (const auto &point : row)
            {
                if (point.is_trailhead())
                {
                    std::cout << point.get_score() << std::endl;
                }
            }
        }
    }

    void print_explored_map(const MapT &map) const
    {
        return;
        for (const auto &row : map)
        {
            for (const auto &point : row)
            {
                std::string to_print{point.been_here_before() ? "+" : "-"};
                std::cout << to_print << " ";
            }
            std::cout << std::endl;
        }
    }

    struct MoveResult
    {
        enum Result
        {
            stuck, // no more moves left (no adjacent positions that haven't been explored that are +1 in value)
            ok,    // have options for places to move
            found  // found a peak
        };

        MoveResult(Result res = stuck, uint32_t num_found = 0U) : res(res), num_found(num_found) {}

        Result res;         // the result of the move
        uint32_t num_found; // we could possibly find multiple peaks from the same position, this is the number of peaks we found
    };

    /**
     * @brief recursive method to explore all the routes
     */
    MoveResult found_peak(const size_t &x, const size_t &y, MapT &map, const size_t &trailhead_x, const size_t &trailhead_y)
    {
        if (map.at(y).at(x).is_peak() && !map.at(y).at(x).been_here_before())
        {
            // mark our current position as 'been here'
            map.at(y).at(x).set_been_here();
            return {MoveResult::Result::found, 1U};
        }

        // find all possible routes from that point
        std::vector<Direction> possible_directions = get_possible_directions(x, y, map);

        if (possible_directions.size() == 0)
        {
            // nowhere else to move, we've exhausted this route
            return {MoveResult::Result::stuck, 0U};
        }

        MoveResult mov_res;
        for (const auto &dir : possible_directions)
        {
            size_t new_x{x};
            size_t new_y{y};
            switch (dir)
            {
            case up:
                new_y = y - 1;
                break;
            case down:
                new_y = y + 1;
                break;
            case left:
                new_x = x - 1;
                break;
            case right:
                new_x = x + 1;
                break;
            }
            MoveResult tmp_mov_res = found_peak(new_x, new_y, map, trailhead_x, trailhead_y);

            if (tmp_mov_res.res == MoveResult::Result::found)
            {
                mov_res.res = MoveResult::Result::found;
            }
            mov_res.num_found += tmp_mov_res.num_found;
        }
        return mov_res;
    }

    /**
     * get the score of a trailhead by searching if it links to a 9
     *
     */
    uint32_t get_trailhead_score(const size_t &x, const size_t &y, const MapT &const_map)
    {
        MapT map{const_map}; // make a copy of the map for each trailhead
        MoveResult m_res = found_peak(x, y, map, x, y);

        if (m_res.res == MoveResult::Result::found)
        {
            return m_res.num_found;
        }

        return 0U; // no peaks found
    }

    /**
     * @brief: Checks if there are any points up, down, left, or right that are +1 in value
     * compared to the current point.
     * - also modifies the map copy so we know where we've explored - TODO: can we trap ourselves? Corner ourselves in so we don't explore everywhere?
     */
    std::vector<Direction> get_possible_directions(const size_t &x, const size_t &y, const MapT &map)
    {
        std::vector<Direction> possible_directions{};
        uint32_t next_point_val{map.at(y).at(x).get_point_value() + 1U}; // next point must be +1

        // TODO: ternary operators??
        // up
        if (y > 0 && map.at(y - 1).at(x).get_point_value() == next_point_val)
        {
            possible_directions.push_back(up);
        }
        // down
        if (y < map.size() - 1 && map.at(y + 1).at(x).get_point_value() == next_point_val)
        {
            possible_directions.push_back(down);
        }
        // left
        if (x > 0 && map.at(y).at(x - 1).get_point_value() == next_point_val)
        {
            possible_directions.push_back(left);
        }
        // right
        if (x < map.at(y).size() - 1 && map.at(y).at(x + 1).get_point_value() == next_point_val)
        {
            possible_directions.push_back(right);
        }
        return possible_directions;
    }

    MapT m_map;
};

int main()
{
    static constexpr std::string day{"10"};

    std::cout << "day " << day << std::endl;

    // std::ifstream infile("../input/day" + day + "_test");
    // std::ifstream infile("../input/day" + day + "_test_2");
    std::ifstream infile("../input/day" + day);

    Hiker::MapT map;
    std::string line;
    size_t y{0};
    while (std::getline(infile, line))
    {
        map.emplace_back();
        for (const auto val : line)
        {
            // test_2 uses dots... make it 10 so we don't explore there
            // the val - '0' is to convert the char digit to an int
            int i{val == '.' ? 10 : val - '0'};
            map.at(y).emplace_back(i);
        }
        ++y;
    }

#define PART_1 true
#define PART_2 false
#if PART_1 == true
    Hiker hiker{map};

    auto trailhead_scores{hiker.find_trailhead_scores()};

    // answer: 698
    std::cout << "part 1 --> trailhead score: " << trailhead_scores << std::endl;
#endif

#if PART_2 == true
    //
    std::cout << "part 2 --> : " << "" << std::endl;
#endif
    return 0;
}
