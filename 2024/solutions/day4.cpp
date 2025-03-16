#include <iostream>
#include <fstream>
#include <vector>

static const std::string word{"XMAS"}; // lazy global...

bool search_n(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos) // north
{
    // bottom to top
    if (y_pos < word.length() - 1)
    {
        return false; // not enough space
    }
    for (size_t i{0}; i < word.length(); ++i)
    {
        if (grid.at(y_pos - i).at(x_pos) != word.at(i))
        {
            return false;
        }
    }
    return true;
}

bool search_ne(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos) // north east
{
    // search word bottom left to top right
    if (y_pos < word.length() - 1 || x_pos + word.length() > grid.at(y_pos).length())
    {
        return false; // not enough space
    }
    for (size_t i{0}; i < word.length(); ++i)
    {
        if (grid.at(y_pos - i).at(x_pos + i) != word.at(i))
        {
            return false;
        }
    }
    return true;
}

bool search_e(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos) // east
{
    // first check if there's enough space to search east
    if (x_pos + word.length() > grid.at(y_pos).length())
    {
        return false; // not enough space
    }
    for (size_t i{0}; i < word.length(); ++i)
    {
        if (grid.at(y_pos).at(x_pos + i) != word.at(i))
        {
            return false;
        }
    }
    return true;
}

bool search_se(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos) // south east
{
    // search top left to bottom right
    if (y_pos + word.length() > grid.size() || x_pos + word.length() > grid.at(y_pos).length())
    {
        return false; // not enough space
    }
    for (size_t i{0}; i < word.length(); ++i)
    {
        if (grid.at(y_pos + i).at(x_pos + i) != word.at(i))
        {
            return false;
        }
    }
    return true;
}

bool search_s(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos) // south
{
    // search top to bottom
    if (y_pos + word.length() > grid.size())
    {
        return false; // not enough space
    }
    for (size_t i{0}; i < word.length(); ++i)
    {
        if (grid.at(y_pos + i).at(x_pos) != word.at(i))
        {
            return false;
        }
    }
    return true;
}

bool search_sw(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos) // south west
{
    // search top right to bottom left
    if (y_pos + word.length() > grid.size() || x_pos < word.length() - 1)
    {
        return false; // not enough space
    }
    for (size_t i{0}; i < word.length(); ++i)
    {
        if (grid.at(y_pos + i).at(x_pos - i) != word.at(i))
        {
            return false;
        }
    }
    return true;
}

bool search_w(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos) // west
{
    // right to left
    if (x_pos < word.length() - 1)
    {
        return false; // not enough space
    }
    for (size_t i{0}; i < word.length(); ++i)
    {
        if (grid.at(y_pos).at(x_pos - i) != word.at(i))
        {
            return false;
        }
    }
    return true;
}

bool search_nw(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos) // north west
{
    // bottom right to top left
    if (y_pos < word.length() - 1 || x_pos < word.length() - 1)
    {
        return false; // not enough space
    }
    for (size_t i{0}; i < word.length(); ++i)
    {
        if (grid.at(y_pos - i).at(x_pos - i) != word.at(i))
        {
            return false;
        }
    }
    return true;
}

void print(const size_t &x_pos, const size_t &y_pos, const std::string &dir)
{
    // std::cout << x_pos << "," << y_pos << "  " << dir << std::endl;
}

bool is_x_mas(const std::vector<std::string> &grid, const size_t &x_pos, const size_t &y_pos)
{
    // check if possible... needs to be one in from each edge
    if (x_pos < 1 || x_pos > grid.at(y_pos).length() - 2)
    {
        // 0123   len = 4    len-2 = 2   xpos_max = 2
        return false;
    }
    if (y_pos < 1 || y_pos > grid.size() - 2)
    {
        return false;
    }

    if (grid.at(y_pos).at(x_pos) != 'A')
    {
        return false;
    }

    // top left 'M' then bottom right 'S'
    // top right 'M' then bottom left 'S'
    // bottom right 'M' then top left 'S'
    // bottom left 'M' then top right 'S'

    char top_left{grid.at(y_pos - 1).at(x_pos - 1)};
    char top_right{grid.at(y_pos - 1).at(x_pos + 1)};
    char bottom_right{grid.at(y_pos + 1).at(x_pos + 1)};
    char bottom_left{grid.at(y_pos + 1).at(x_pos - 1)};

    size_t xmas_count{0};
    if ((top_left == 'M' && bottom_right == 'S') || (top_left == 'S' && bottom_right == 'M'))
    {
        ++xmas_count;
    }
    if ((top_right == 'M' && bottom_left == 'S') || (top_right == 'S' && bottom_left == 'M'))
    {
        ++xmas_count;
    }

    if (xmas_count == 2)
    {
        return true;
    }

    return false;
}

int main()
{
    std::cout << "day 4" << std::endl;

    std::ifstream file("input/day4");
    // std::ifstream file("input/day4_test");
    std::string line;

    std::vector<std::string> grid{};

    while (std::getline(file, line))
    {
        grid.push_back(line);
    }

    size_t xmas_count{0};
    size_t xmas_count_p2{0};

    for (size_t y{0}; y < grid.size(); ++y)
    {
        for (size_t x{0}; x < grid.at(y).length(); ++x)
        {
            if (search_e(grid, x, y))
            {
                ++xmas_count;
                print(x, y, "e");
            }
            if (search_w(grid, x, y))
            {
                ++xmas_count;
                print(x, y, "w");
            }
            if (search_s(grid, x, y))
            {
                ++xmas_count;
                print(x, y, "s");
            }
            if (search_n(grid, x, y))
            {
                ++xmas_count;
                print(x, y, "n");
            }
            if (search_se(grid, x, y))
            {
                ++xmas_count;
                print(x, y, "se");
            }
            if (search_ne(grid, x, y))
            {
                ++xmas_count;
                print(x, y, "ne");
            }
            if (search_sw(grid, x, y))
            {
                ++xmas_count;
                print(x, y, "sw");
            }
            if (search_nw(grid, x, y))
            {
                ++xmas_count;
                print(x, y, "nw");
            }

            // part 2
            if (is_x_mas(grid, x, y))
            {
                ++xmas_count_p2;
            }
        }
    }

    std::cout << "\nxmas_count_p1: " << xmas_count << std::endl;
    std::cout << "\nxmas_count_p2: " << xmas_count_p2 << std::endl;

    return 0;
}