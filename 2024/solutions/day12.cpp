#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

// https://adventofcode.com/2024/day/12

// DAY=12 && clang++ -Wall -std=c++20 day${DAY}.cpp -o day${DAY} && ./day${DAY}

/**
 * @brief a single letter
 */
class Point
{
public:
    Point(char letter, size_t x, size_t y) : m_position{x, y}, m_letter{letter}, m_accounted{false} {}

    struct Position
    {
        size_t x;
        size_t y;
    };

    char get_letter() const
    {
        return m_letter;
    }

    bool is_accounted_for() const
    {
        return m_accounted;
    }

    void set_accounted_for()
    {
        m_accounted = true;
    }

    Position get_position() const
    {
        return m_position;
    }

private:
    Position m_position;
    char m_letter;
    bool m_accounted;
};

/**
 * @brief all the points of the same letter that are connected horizontally and/or vertically.
 *
 * - "The area of a region is simply the number of garden plots the region contains"
 *
 * - "The perimeter of a region is the number of sides of garden plots in the region that do
 *   not touch another garden plot in the same region."
 *
 * @param[in] points is a vector of Point objects (a single point on the map) that all share
 * the same letter and are connected horizontally and/or vertically.
 */
class Plot
{
public:
    Plot(char letter, std::vector<Point> points) : m_letter{letter}, m_points{points}, m_perimeter{calculate_perimeter()}, m_area{points.size()} {}

private:
    /**
     * @brief given the input point location, and all the points in this plot, find whether the input
     * point is part of this plot.
     *
     * @param[in] search_x is the x location to search for
     * @param[in] search_y is the y location to search for
     */
    bool point_in_plot(size_t search_x, size_t search_y)
    {
        for (const auto &point : m_points)
        {
            if (point.get_position().x == search_x && point.get_position().y == search_y)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * "The perimeter of a region is the number of sides of garden plots in the region that do
     * not touch another garden plot in the same region."
     */
    size_t calculate_perimeter()
    {
        size_t perimeter{0};

        // loop over each point and see which of the sides touch another garden plot in the same region
        for (const auto &point : m_points)
        {
            size_t point_x{point.get_position().x};
            size_t point_y{point.get_position().y};

            // check up
            if (point_y > 0) // this check is to make sure size_t doesn't go negative
            {
                if (!point_in_plot(point_x, point_y - 1))
                {
                    ++perimeter;
                }
            }
            else // if we can't go up then it must be a perimeter
            {
                ++perimeter;
            }
            // down (don't need to check for a size too big... not accessing a vector or anything)
            if (!point_in_plot(point_x, point_y + 1))
            {
                ++perimeter;
            }
            // left
            if (point_x > 0) // this check is to make sure size_t doesn't go negative
            {
                if (!point_in_plot(point_x - 1, point_y))
                {
                    ++perimeter;
                }
            }
            else // if we can't go left then it must be a perimeter
            {
                ++perimeter;
            }
            // right
            if (!point_in_plot(point_x + 1, point_y))
            {
                ++perimeter;
            }
        }

        return perimeter;
    }

    char m_letter;
    std::vector<Point> m_points;
    size_t m_perimeter;
    size_t m_area;
};

/**
 * @brief the whole area that includes all the Plots
 */
class Farm
{
public:
    using MapT = std::vector<std::vector<Point>>;
    using PlotT = std::vector<Plot>;

    Farm(MapT area) : m_area{area} {}

    /**
     * @brief loop over each Point and start a search for the plot
     * - a lot of the Points will have an 'accounted for' bool and we will skip those
     */
    void find_plots()
    {
        for (size_t y{0}; y < m_area.size(); ++y)
        {
            for (size_t x{0}; x < m_area.at(y).size(); ++x)
            {
                if (m_area.at(y).at(x).is_accounted_for())
                {
                    continue;
                }
                m_plots.push_back(find_plot(x, y));
            }
        }
    }

private:
    /**
     * @brief recursive function to find all the neighboring points with the same letter
     * - searches horizontally and vertically
     *
     */
    void find_all_points(std::vector<Point> &plot_points, const char letter, size_t search_x, size_t search_y)
    {
        // first check if x and y are on the map
        if (search_x >= m_area.at(search_y).size() || search_y >= m_area.size())
        {
            return;
        }

        // next check if this search pos has the same letter
        if (letter != m_area.at(search_y).at(search_x).get_letter())
        {
            return;
        }

        // then check if we have already been here or not
        if (m_area.at(search_y).at(search_x).is_accounted_for())
        {
            return;
        }

        // this point is a successful find, so add it to the plot_points vector
        plot_points.emplace_back(letter, search_x, search_y);
        // and mark it accounted for
        m_area.at(search_y).at(search_x).set_accounted_for();

        // now continue the search

        // can we go up?
        if (search_y > 0)
        {
            find_all_points(plot_points, letter, search_x, search_y - 1);
        }
        // down?
        if (search_y < m_area.size() - 1)
        {
            find_all_points(plot_points, letter, search_x, search_y + 1);
        }
        // left?
        if (search_x > 0)
        {
            find_all_points(plot_points, letter, search_x - 1, search_y);
        }
        // right?
        if (search_x < m_area.at(search_y).size() - 1)
        {
            find_all_points(plot_points, letter, search_x + 1, search_y);
        }
    }

    /**
     * @brief search all the neighboring Points that have the same letter
     *
     * @param x starting x (x to start searching at)
     * @param y starting y (y to start searching at)
     */
    Plot find_plot(size_t start_x, size_t start_y)
    {
        char letter{m_area.at(start_y).at(start_x).get_letter()};
        std::vector<Point> plot_points{};
        find_all_points(plot_points, letter, start_x, start_y);
        return Plot{letter, plot_points};
    }

    std::vector<Plot> m_plots;

    MapT m_area;
};

int main()
{
    static constexpr std::string day{"12"};

    std::cout << "day " << day << std::endl;

    std::ifstream infile("../input/day" + day + "_test1");
    // std::ifstream infile("../input/day" + day);

    Farm::MapT area;
    std::string line;
    size_t y{0};
    while (std::getline(infile, line))
    {
        area.push_back(std::vector<Point>{});
        for (size_t i{0}; i < line.size(); ++i)
        {
            area.at(y).emplace_back(line.at(i), i, y);
        }
        ++y;
    }

    Farm farm{area};

    farm.find_plots();

    std::cout << std::endl;

#define PART_1 true
#define PART_2 false

#if PART_1 == true
    // size_t num_stones{stone_line.size()};
    // std::cout << "part 1 --> num_stones: " << num_stones << std::endl;
    // static constexpr int p1_answer{203609};
    // if (num_stones != p1_answer)
    // {
    //     std::cout << "part 1 error!!  -> " << num_stones << " != " << p1_answer << std::endl;
    //     std::exit(1);
    // }
#endif

#if PART_2 == true
#endif

    return 0;
}
