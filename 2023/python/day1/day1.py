"""
https://adventofcode.com/2023/day/1
"""

import pathlib

TEST = True

# path from cwd (cwd=/advent-of-code-2023)
INPUT_SOURCE = "python/day1/day1_input.txt"


def get_num_from_line(line: str) -> int:
    """
    combine the first and last digit to get the two digit number
    """
    num = list()
    last_digit = ""
    for index, c in enumerate(line):
        if c.isdigit():
            last_digit = c
        if last_digit != "" and (len(num) == 0) or (index == len(line) - 1):
            num.append(last_digit)

    # we now have a list of len 2 that we need to combine into an int
    return int("".join(num))


def main():
    total = 0
    with open(f"{pathlib.Path().resolve()}/{INPUT_SOURCE}", "r") as f:
        for line in f:
            total += get_num_from_line(line)

    print(f"sum of all calibration values: {total}")


def test_get_num_from_line():
    # (input, output)
    test_cases = (
        ("lddxjsczqkd26g5jpvdlfour", 25),
        ("1twotwofcl", 11),
        ("sltfz55knrfvhhbbeightsevenninesevenfour", 55),
        ("27155bvv", 25),
    )

    for test in test_cases:
        assert get_num_from_line(test[0]) == test[1]


if __name__ == "__main__":
    if TEST:
        test_get_num_from_line()
    main()
