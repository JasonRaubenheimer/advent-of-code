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
        if (disk_map.at(i).id == Block::space)
        {
            continue;
        }
        checksum += i * disk_map.at(i).id;
    }
    return checksum;
}

struct WholeFile
{
    WholeFile(int id, size_t size, bool attempted_move = false) : id{id}, attempted_move{attempted_move}
    {
        for (size_t i{0}; i < size; ++i)
        {
            file.emplace_back(id);
        }
    }

    void add_block(int id)
    {
        file.emplace_back(id);
    }

    size_t get_size() const
    {
        return file.size();
    }

    bool still_space()
    {
        for (const auto &b : file)
        {
            if (b.id == Block::space)
            {
                return true;
            }
        }
        return false;
    }

    void print() const
    {
        std::cout << "< ";
        for (const auto &b : file)
        {
            std::cout << b << " ";
        }
        std::cout << ">";
    }

    void print_3() const
    {
        for (const auto &b : file)
        {
            std::cout << b << " ";
        }
    }

    std::vector<Block> file;
    int id;
    bool attempted_move;

    friend std::ostream &operator<<(std::ostream &os, const WholeFile &f);
};

std::ostream &operator<<(std::ostream &os, const WholeFile &f)
{
    os << "<" << f.id << ", " << f.get_size() << ">";
    return os;
}

std::vector<WholeFile> create_WholeFiles(const std::vector<Block> &disk_map)
{
    std::vector<WholeFile> wholeFiles;

    int current_id;
    for (size_t i{0}; i < disk_map.size();) // no ++
    {
        current_id = disk_map.at(i).id;

        // see how big the size is...
        size_t test_idx{i};
        size_t size{0};
        while (test_idx < disk_map.size() && current_id == disk_map.at(test_idx).id)
        {
            ++test_idx;
            ++size;
        }

        // now create WholeFile
        wholeFiles.emplace_back(current_id, size);

        // std::cout << "id: " << current_id << ", size: " << size << std::endl;
        i = test_idx;
    }

    return wholeFiles;
}

void print_wholeFiles_3(const std::vector<WholeFile> &disk_map)
{
    for (const auto &f : disk_map)
    {
        f.print_3();
        std::cout << ' ';
    }
    std::cout << std::endl;
}

void print_wholeFiles_2(const std::vector<WholeFile> &disk_map)
{
    for (const auto &f : disk_map)
    {
        f.print();
        std::cout << ' ';
    }
    std::cout << std::endl;
}

void print_wholeFiles(const std::vector<WholeFile> &disk_map)
{
    for (const auto &f : disk_map)
    {
        std::cout << f << ' ';
    }
    std::cout << std::endl;
}

std::vector<WholeFile> compress_wholefiles(const std::vector<WholeFile> &wholefiles)
{
    std::vector<WholeFile> compressed{wholefiles};

    while (true)
    {
        // first find block that hasn't been moved yet
        auto last_block = std::find_if(compressed.rbegin(), compressed.rend(), [](WholeFile f)
                                       { return (!f.attempted_move) && (f.id != Block::space); });

        if (last_block == compressed.rend())
        {
            // no blocks where attempted_move is false
            // we are done
            return compressed;
        }
        // set attempted move
        last_block->attempted_move = true;

        // now try find space that will fit
        auto first_space = std::find_if(compressed.begin(), compressed.end(), [&last_block](WholeFile f)
                                        { 
                                            if (f.id != Block::space) // must be space
                                            {
                                                return false;
                                            }
                                            if(f.get_size() < last_block->get_size()) // space must be big enough
                                            {
                                                return false;
                                            }
                                            return true; });

        if ((first_space - compressed.begin()) >= (compressed.rend() - last_block)) // no space for the block...
        {
            // std::cout << "done checking: ";
            // last_block->print();
            // std::cout << std::endl;
            // std::cout << "new state: ";
            // print_wholeFiles_3(compressed);
            // std::cout << std::endl;
            continue;
        }

        // now 'move' the block into the space

        for (size_t i{0}; i < last_block->get_size(); ++i)
        {
            first_space->file.at(i).id = last_block->id;
            last_block->file.at(i).id = Block::space;
        }
        // set last block to space TODO: method for this...
        first_space->id = last_block->id;
        last_block->id = Block::space;

        // now we have the issue that the new 'block' might be half block and half space...
        if (first_space->still_space())
        {
            // need to split into block and space
            // get copy of the space/block combo
            WholeFile block{first_space->file.at(0).id, last_block->get_size(), true};
            WholeFile space{Block::space, first_space->get_size() - block.get_size()};

            // remove old space/block combo
            compressed.erase(first_space);
            // then add block
            compressed.insert(first_space, block);
            // then add remaining space
            compressed.insert(first_space + 1, space);
        }

        // if there are spaces next to each other we need to merge them
        for (size_t i{0}; i < compressed.size(); ++i)
        {
            if (i + 1 < compressed.size())
            {
                if ((compressed.at(i).id == Block::space) && (compressed.at(i + 1).id == Block::space))
                {
                    // need to merge
                    WholeFile bigger_space{Block::space,
                                           (compressed.at(i).get_size() + compressed.at(i + 1).get_size())};
                    // remove old
                    compressed.erase(compressed.begin() + i);
                    compressed.erase(compressed.begin() + i); // was the i + 1
                    // then add new
                    compressed.insert(compressed.begin() + i, bigger_space);
                    --i; // the next one (i.e. (original i) + 2) might also be a space
                }
            }
        }
    }

    return compressed;
}

std::vector<Block> wholeFile_to_Block(const std::vector<WholeFile> &whole)
{
    std::vector<Block> blocks;
    for (const auto &w : whole)
    {
        for (size_t i{0}; i < w.get_size(); ++i)
        {
            blocks.emplace_back(w.id);
        }
    }
    return blocks;
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

#define PART_1 false
#define PART_2 true
#if PART_1 == true
    // compress filesystem
    std::vector<Block> compressed_disk_map = compress_filesystem(expanded_disk_map);
    // print_diskmap(compressed_disk_map);

    // calculate checksum --> answer: 6421128769094
    std::cout << "part 1 --> checksum: " << calculate_checksum(compressed_disk_map) << std::endl;
#endif

#if PART_2 == true
    // for part 2:
    // convert expanded disk map into vector of WholeFile objects
    std::vector<WholeFile> wholeFiles = create_WholeFiles(expanded_disk_map);

    // print_wholeFiles(wholeFiles);

    // compress filesystem with wholefiles
    std::vector<WholeFile> compressed_wholefiles = compress_wholefiles(wholeFiles);

    // convert vector of WholeFiles into a vector of Blocks (for checksum)
    std::vector<Block> blocks = wholeFile_to_Block(compressed_wholefiles);

    // print_diskmap(blocks);
    // calculate checksum --> answer: 6448168620520
    std::cout << "part 2 --> checksum: " << calculate_checksum(blocks) << std::endl;

    // 00992111777.44.333....5555.6666.....8888..
#endif
    return 0;
}
