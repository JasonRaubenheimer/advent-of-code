#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

int main()
{
    std::cout << "day 1" << std::endl;

    // two lists
    std::vector<int> list1;
    std::vector<int> list2;

    // read in the two lists
    std::ifstream file("input/day1");
    std::string line;
    std::string delimiter = "   "; // nums separated by 3 spaces
    size_t pos;                    // first position of the delimiter
    while (std::getline(file, line))
    {
        pos = line.find(delimiter);
        list1.push_back(std::stoi(line.substr(0, pos)));
        list2.push_back(std::stoi(line.substr(pos + delimiter.length(), line.length())));
    }

    // sort the lists smallest to biggest
    std::sort(list1.begin(), list1.end()); // default is ascending
    std::sort(list2.begin(), list2.end());

    // calculate distances between pairs and add results to third list
    // or just reuse list 1...
    for (size_t i{0}; i < list1.size(); ++i)
    {
        /*
        2  7 -> 2 - 7 = -5
        7  2 -> 7 - 2 = 5
        */
        list1.at(i) = std::abs(list1.at(i) - list2.at(i));
    }

    // add up third list to get answer
    // int res = std::reduce(list1.begin(), list1.end()); // need c++17
    int part1_res{0};
    for (auto &n : list1)
    {
        part1_res += n;
    }

    std::cout << "part1_res: " << part1_res << std::endl;

    return 0;
}
