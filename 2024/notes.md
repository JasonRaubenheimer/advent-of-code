https://adventofcode.com/2024

## compiling and running

### using clang++

Use clang++ rather than clang because clang++ does linking automatically.

```sh
DAY=1 && clang++ -Wall -std=c++20 day${DAY}.cpp -o day${DAY} && ./day${DAY}
```
