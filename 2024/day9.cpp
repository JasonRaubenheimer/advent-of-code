#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>

// https://adventofcode.com/2024/day/9

// DAY=9 && clang++ -Wall -std=c++20 day${DAY}.cpp -o day${DAY} && ./day${DAY}

struct Block
{
    Block(int id) : id{id} {}

    int id;
    static const int space{-1};

    friend std::ostream &operator<<(std::ostream &os, const Block &bl);
};

std::ostream &operator<<(std::ostream &os, const Block &bl)
{
    os << bl.id;
    return os;
}

std::vector<Block> expand_filesystem(const std::string &disk_map)
{
    std::vector<Block> expanded_disk_map{};
    std::stringstream stream{disk_map};
    char c;
    size_t idx{0};
    int id{0};
    while (stream >> c)
    {
        int val{c - '0'};
        int id_tmp = (idx % 2 == 0) ? id : Block::space; // odd indexes are free space
        for (size_t i{0}; i < val; ++i)
        {
            expanded_disk_map.emplace_back(id_tmp);
        }
        if (id_tmp != Block::space)
        {
            ++id;
        }
        ++idx;
    }
    return expanded_disk_map;
}

std::vector<Block> compress_filesystem(const std::vector<Block> &disk_map)
{
    std::vector<Block> compressed_disk_map{disk_map};

    // keep looping until there are no blocks with a 'space' id
    bool still_spaces{true};
    do
    {
        // find first space
        auto first_space = std::find_if(compressed_disk_map.begin(), compressed_disk_map.end(), [](Block bl)
                                        { return bl.id == Block::space; });

        // find last block
        auto last_block = compressed_disk_map.end(); // always end because we are popping the last val

        if (first_space == last_block) // i.e. we are at the end
        {
            // we are done, return
            return compressed_disk_map;
        }

        // move last block to first space pos, remove the space
        // just update the id - the object doesn't have to move...
        auto first_pos{first_space - compressed_disk_map.begin()};
        auto last_pos{last_block - compressed_disk_map.begin() - 1}; // why -1? out of range but why?
        compressed_disk_map.at(first_pos).id = compressed_disk_map.at(last_pos).id;
        // now remove the last pos
        compressed_disk_map.pop_back();
        still_spaces = std::find_if(compressed_disk_map.begin(), compressed_disk_map.end(), [](Block bl)
                                    { return bl.id == Block::space; }) != compressed_disk_map.end();
    } while (still_spaces);

    return compressed_disk_map;
}

void print_diskmap(const std::vector<Block> &disk_map)
{
    for (const auto &bl : disk_map)
    {
        std::cout << bl << ' ';
    }
    std::cout << std::endl;
}

/**
 * To calculate the checksum, add up the result of multiplying each of these blocks' position
 * with the file ID number it contains. The leftmost block is in position 0. If a block
 * contains free space, skip it instead.
 *
 * THIS ONLY WORKS WITH LONG! (Overflow otherwise...)
 */
long calculate_checksum(const std::vector<Block> &disk_map)
{
    long checksum{0};
    for (int i{0}; i < disk_map.size(); ++i)
    {
        checksum += i * disk_map.at(i).id;
    }
    return checksum;
}

int main()
{
    static constexpr std::string day{"9"};

    std::cout << "day " << day << std::endl;

    // std::ifstream infile("input/day" + day + "_test");
    std::ifstream infile("input/day" + day);

    std::string disk_map{};
    std::string line;
    while (std::getline(infile, line))
    {
        disk_map += line; // should only be one line
    }

    // std::cout << disk_map << std::endl;

    std::vector<Block> expanded_disk_map = expand_filesystem(disk_map);
    // print_diskmap(expanded_disk_map);

    // compress filesystem
    std::vector<Block> compressed_disk_map = compress_filesystem(expanded_disk_map);
    // print_diskmap(compressed_disk_map);

    // calculate checksum --> answer: 6421128769094
    std::cout << "checksum: " << calculate_checksum(compressed_disk_map) << std::endl;

    return 0;
}
