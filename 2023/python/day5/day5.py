"""
https://adventofcode.com/2023/day/5

tried:

"""
import pathlib

TESTING = False

# path from cwd (cwd=/advent-of-code-2023)
INPUT_SOURCE = "python/day5/day5_input.txt"
INPUT_SOURCE_TESTING = "python/day5/day5_input_testing.txt"

FILE_INPUT_PATH = INPUT_SOURCE_TESTING if TESTING else INPUT_SOURCE


def solve_day():
    """"""
    path = f"{pathlib.Path().resolve()}/{FILE_INPUT_PATH}"
    with open(path, "r") as f:
        for line in f:
            print(line)


def main():
    solve_day()
    print(f"the solution")


if __name__ == "__main__":
    main()
