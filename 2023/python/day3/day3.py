"""
https://adventofcode.com/2023/day/3

tried:
537066 - too low

"""

import pathlib
from typing import List

TEST = True

# path from cwd (cwd=/advent-of-code-2023)
INPUT_SOURCE = "python/day3/day3_input.txt"
INPUT_SOURCE_TESTING = "python/day3/day3_input_testing.txt"


def validate_data(data: List[List]):
    # check that there are the correct number of rows
    assert len(data) == 140 + 2  # +2 for padding
    # check that all rows are correct length
    for i, row in enumerate(data):
        assert len(row) == 140 + 2  # +2 for padding


def check_special_char(data: List[List], row_idx: int, col_idx: int):
    """
    check:
    1,2,3
    4,c,5
    6,7,8
    """
    checks = [
        [row_idx - 1, col_idx - 1],  # check num: 1
        [row_idx - 1, col_idx],  # check num: 2
        [row_idx - 1, col_idx + 1],  # check num: 3
        [row_idx, col_idx - 1],  # check num: 4
        [row_idx, col_idx + 1],  # check num: 5
        [row_idx + 1, col_idx - 1],  # check num: 6
        [row_idx + 1, col_idx],  # check num: 7
        [row_idx + 1, col_idx + 1],  # check num: 8
    ]

    for check in checks:
        c: str = data[check[0]][check[1]]
        if (c != ".") and (not c.isdigit()):
            # must be a special char
            return True
    return False


def get_valid_numbers(test=False) -> list:
    """
    number is valid if it has a symbol next to it (including diagonal),
    e.g. 35 and 633

    ...*......
    ..35..633.
    ......#...

    need to store prev line, current line, and next line
    iterate over current? line and check surrounding chars
    if char is digit, check surrounding chars
        if next char is digit, it is part of the same number...

    1 2 3
    4 . 5
    6 7 8

    for each line
        for each char in line
            if char isdigit
                check if any pos (above, 1 - 8) is char (will
                need to check if pos 'available' (e.g. if we
                are on the first line, 1 2 3 is not available)

                need to make sure if numbers are continuous (i.e.
                if pos 5 is also number) then they form the same number

    read in the data as a matrix (list of lists of chars)
    - "scan" over each char?

    data = [[1,2,3],[4,.,5],[6,7,8]]



    returns a list of valid numbers
    """
    data = list()
    path = f"{pathlib.Path().resolve()}/{INPUT_SOURCE if not test else INPUT_SOURCE_TESTING}"
    with open(path, "r") as f:
        for line in f:
            # remove '\n' from end and add '.'
            data.append(["."] + list(line[0:-1]) + ["."])
    # add padding at idx 0 and idx -1
    padding = ["." for _ in range(len(data[0]))]
    data.insert(0, padding)
    data.append(padding)
    # print(data)

    if not test:
        validate_data(data)

    values = []

    for row_num, row in enumerate(data):
        # dont check first or last row (padding)
        if row_num < 1 or row_num >= len(data) - 1:
            continue
        c: str
        current_num = []
        valid = False
        for col_num, c in enumerate(row):
            if col_num >= len(data[0]) - 1:
                # end of row, check if we have a number
                if valid:
                    values.append(int("".join(current_num)))
            # dont check first or last col (padding)
            if col_num < 1 or col_num >= len(data[0]) - 1:
                continue

            # if current_num == ["1", "2", "3"]:
            #     print()

            if c.isdigit():
                current_num.append(c)
                if check_special_char(data, row_num, col_num):
                    valid = True
            elif current_num:
                if valid:
                    values.append(int("".join(current_num)))
                current_num.clear()
                valid = False

            # print(c, row_num, col_num, values)

    return values


def main():
    valid_nums = get_valid_numbers()
    total = sum(valid_nums)

    print(f"sum of all part nums: {total}")


def test_get_valid_numbers():
    # not: 114 (top right) and 58
    valid_numbers = (123, 111, 467, 35, 633, 617, 592, 755, 664, 598)
    not_valid = (114, 58)

    res = get_valid_numbers(test=True)
    for num in res:
        assert num in valid_numbers

    for not_v in not_valid:
        assert not_v not in res

    assert len(res) == len(valid_numbers)

    total = sum(res)

    assert total == 4361 + 123 + 111

    print("Test OK")


if __name__ == "__main__":
    if TEST:
        test_get_valid_numbers()
    main()
