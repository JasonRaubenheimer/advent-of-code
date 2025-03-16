#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

int main()
{
    std::cout << "day 1" << std::endl;
    std::cout << "\n-- part 1 --" << std::endl;

    // two lists
    std::vector<int> list1;
    std::vector<int> list2;

    // read in the two lists
    std::ifstream file("../input/day1");
    // std::ifstream file("../input/day1_test");
    std::string line;
    std::string delimiter = "   "; // nums separated by 3 spaces
    size_t pos;                    // first position of the delimiter
    while (std::getline(file, line))
    {
        pos = line.find(delimiter);
        list1.push_back(std::stoi(line.substr(0, pos)));
        list2.push_back(std::stoi(line.substr(pos + delimiter.length(), line.length())));
    }

    // just get part2 lists here...
    std::vector<int> list1_p2{list1};
    std::vector<int> list2_p2{list2};

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

    std::cout << "\n-- part 2 --" << std::endl;

    /* going to rather use a map and try_emplace
    // get a set of left list numbers (so no duplicates)
    std::vector<int> no_dups{list1_p2};
    // need to sort first so unique does what we want
    std::sort(no_dups.begin(), no_dups.end());
    no_dups.erase(std::unique(no_dups.begin(), no_dups.end()), no_dups.end());
    */

    // using map rather
    std::map<int, int> counts{};
    // iterate over list1 and add to map
    // using try_emplace to only add to map if it doesn't exist, therefore we'll
    // have a map and removed duplicates at the same time (rather than getting
    // a unique vector first then creating a map)
    std::for_each(list1_p2.begin(), list1_p2.end(), [&counts](const int &n)
                  { counts.try_emplace(n, 0); });

    // count the occurrences of each left list number in the right list
    std::for_each(counts.begin(), counts.end(),
                  [&list2_p2, &counts](const std::pair<int, int> &element)
                  {
                      counts.at(element.first) = std::count(list2_p2.begin(), list2_p2.end(), element.first); // add count to map
                  });

    // now we have a map where each key is a value that exists in the left list, and the
    // value is the number of times that key occurs in the right list

    // loop over complete left list and multiply each number by the occurrences
    std::vector<int> res_vec_p2{}; // vector to store the multiplication result
    std::for_each(list1_p2.begin(), list1_p2.end(), [&res_vec_p2, &counts](const int &el)
                  { res_vec_p2.push_back(el * counts.at(el)); });

    // add left list (that has been multiplied) up
    int part2_res{0};
    for (const auto &el : res_vec_p2)
    {
        part2_res += el;
    }

    std::cout << "part2_res: " << part2_res << std::endl;

    return 0;
}
