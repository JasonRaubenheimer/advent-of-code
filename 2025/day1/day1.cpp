#include <fstream>
#include <iostream>
#include <string>

// DAY=1 && clang++ -Wall -std=c++20 day${DAY}.cpp -o day${DAY} && ./day${DAY}

int main() {

  // 0 - 99
  // starts at 50
  // The actual password is the number of times the dial is left pointing at 0
  // after any rotation in the sequence.

  const bool testing{false};

  std::ifstream file{testing ? "test_input" : "input"};

  const int starting{50};
  const int match_num{0};
  const int min_num{0};
  const int max_num{99};

  int count{0};
  int current_val{starting};

  // start at 50
  // read each line
  // if current == 0, ++ count

  std::string rotation;
  while (std::getline(file, rotation)) {

    char dir{rotation.at(0)};
    int val{std::stoi(rotation.substr(1, rotation.length()))};

    const char right{'R'};

    // std::cout << rotation << " -- dir: " << dir << ", val: " << val
    //           << std::endl;

    if (dir == right) {
      //   std::cout << "right" << std::endl;
      current_val += val;
      while (current_val > max_num) {
        current_val -= (max_num + 1);
      }
    } else {
      //   std::cout << "left" << std::endl;
      current_val -= val;
      while (current_val < min_num) {
        current_val += (max_num + 1);
      }
    }

    // std::cout << "current_val: " << current_val << std::endl;

    if (current_val == match_num) {
      ++count;
    }
  }

  std::cout << "count: " << count << std::endl;

  return 0;
}
