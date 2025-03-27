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
    Point(char letter, size_t x, size_t y) : m_position{x, y}, m_letter{letter}, m_accounted{false}, m_adjacent_x{false}, m_adjacent_y{false} {}

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

    void set_adjacent_x()
    {
        m_adjacent_x = true;
    }

    bool get_adjacent_x() const
    {
        return m_adjacent_x;
    }

    void set_adjacent_y()
    {
        m_adjacent_y = true;
    }

    bool get_adjacent_y() const
    {
        return m_adjacent_y;
    }

private:
    Position m_position;
    char m_letter;
    bool m_accounted;
    bool m_adjacent_x;
    bool m_adjacent_y;
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
    Plot(char letter, std::vector<Point> points) : m_letter{letter}, m_points{points}, m_corner_point_count{0}, m_perimeter{calculate_perimeter()}, m_area{points.size()}, m_price{calculate_price()}, m_price_p2{calculate_price_p2()}
    {
        (void)m_letter; // thought I'd need this but doesn't seem like it... maybe part 2?
    }

    uint64_t get_price() const
    {
        return m_price;
    }

    uint64_t get_price_p2() const
    {
        return m_price_p2;
    }

private:
    /**
     * @brief given the input point location, and all the points in this plot, find whether the input
     * point is part of this plot.
     *
     * Ok to pass in negative numbers
     * - this function checks for that and returns false if x < 0 || y < 0
     *
     * @param[in] search_x int64_t is the x location to search for
     * @param[in] search_y int64_t is the y location to search for
     */
    bool point_in_plot(int64_t search_x, int64_t search_y)
    {
        if (search_x < 0 || search_y < 0)
        {
            return false;
        }

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
            if (!point_in_plot(point_x, point_y - 1))
            {
                ++perimeter;
            }
            // down (don't need to check for a size too big... not accessing a vector or anything)
            if (!point_in_plot(point_x, point_y + 1))
            {
                ++perimeter;
            }
            // left
            if (!point_in_plot(point_x - 1, point_y))
            {
                ++perimeter;
            }
            // right
            if (!point_in_plot(point_x + 1, point_y))
            {
                ++perimeter;
            }

            // for part 2
            // check the 'outer' corners
            /*
                A A A
                A A A A
                A A A
            */
            // top right
            if (!point_in_plot(point_x, point_y - 1) && !point_in_plot(point_x + 1, point_y))
            {
                ++m_corner_point_count;
            }
            // bottom right
            if (!point_in_plot(point_x, point_y + 1) && !point_in_plot(point_x + 1, point_y))
            {
                ++m_corner_point_count;
            }
            // bottom left
            if (!point_in_plot(point_x, point_y + 1) && !point_in_plot(point_x - 1, point_y))
            {
                ++m_corner_point_count;
            }
            // top left
            if (!point_in_plot(point_x, point_y - 1) && !point_in_plot(point_x - 1, point_y))
            {
                ++m_corner_point_count;
            }
            // now need to find the 'inner' corners like the + below
            /*
               A A
               A A
               A + A A
               A A A A
            */
            // not top right, but top and right are
            if (!point_in_plot(point_x + 1, point_y - 1) && point_in_plot(point_x, point_y - 1) && point_in_plot(point_x + 1, point_y))
            {
                ++m_corner_point_count;
            }
            // not bottom right, but bottom and right are
            if (!point_in_plot(point_x + 1, point_y + 1) && point_in_plot(point_x, point_y + 1) && point_in_plot(point_x + 1, point_y))
            {
                ++m_corner_point_count;
            }
            // not bottom left, but bottom and left are
            if (!point_in_plot(point_x - 1, point_y + 1) && point_in_plot(point_x, point_y + 1) && point_in_plot(point_x - 1, point_y))
            {
                ++m_corner_point_count;
            }
            // not top left, but top and left are
            if (!point_in_plot(point_x - 1, point_y - 1) && point_in_plot(point_x, point_y - 1) && point_in_plot(point_x - 1, point_y))
            {
                ++m_corner_point_count;
            }
        }

        return perimeter;
    }

    /**
     * @brief "The price of fence required for a region is found by multiplying that region's
     * area by its perimeter."
     */
    uint64_t calculate_price()
    {
        return m_area * m_perimeter;
    }

    /**
     * @brief "multiplying the region's area by its number of sides"
     */
    uint64_t calculate_price_p2()
    {
        return m_area * m_corner_point_count;
    }

    char m_letter;
    std::vector<Point> m_points;
    size_t m_corner_point_count;
    size_t m_perimeter;
    size_t m_area;
    uint64_t m_price;
    uint64_t m_price_p2;
};

/**
 * @brief the whole area that includes all the Plots
 */
class Farm
{
public:
    using MapT = std::vector<std::vector<Point>>;
    using PlotT = std::vector<Plot>;

    Farm(MapT area) : m_area{area}, m_total_price{0}, m_total_price_p2{0} {}

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

    /**
     * @brief "The total price of fencing all regions on a map is found by adding together
     * the price of fence for every region on the map."
     */
    void calculate_total_price()
    {
        for (const auto &plot : m_plots)
        {
            m_total_price += plot.get_price();
        }
    }

    /**
     * @brief return the total price of all the plots
     */
    uint64_t get_total_price() const
    {
        return m_total_price;
    }

    void calculate_total_price_p2()
    {
        for (const auto &plot : m_plots)
        {
            m_total_price_p2 += plot.get_price_p2();
        }
    }

    /**
     * @brief return the total price of all the plots (for part 2)
     */
    uint64_t get_total_price_p2() const
    {
        return m_total_price_p2;
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
    uint64_t m_total_price;
    uint64_t m_total_price_p2;
};

int main()
{
    static constexpr std::string day{"12"};

    std::cout << "day " << day << std::endl;

    // std::ifstream infile("../input/day" + day + "_test1");
    // std::ifstream infile("../input/day" + day + "_test2");
    std::ifstream infile("../input/day" + day);

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
    farm.calculate_total_price();

#define PART_1 true
#define PART_2 true

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
    farm.calculate_total_price_p2();
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
