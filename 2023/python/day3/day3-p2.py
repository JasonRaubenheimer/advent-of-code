"""
https://adventofcode.com/2023/day/3

tried:
79221762 - too low
1033484470281 - too high

CORRECT! 81709807
"""

import pathlib
from typing import List

# path from cwd (cwd=/advent-of-code-2023)
INPUT_SOURCE = "python/day3/day3_input.txt"


def validate_data(data: List[List]):
    # check that there are the correct number of rows
    assert len(data) == 140 + 2  # +2 for padding
    # check that all rows are correct length
    for i, row in enumerate(data):
        assert len(row) == 140 + 2  # +2 for padding


class Star:
    ID = 0

    def __init__(self):
        self.id = Star.ID
        Star.ID += 1

    def get_id(self) -> int:
        return self.id

    def __str__(self) -> str:
        return "-1"


def check_special_star_char(data: List[List], row_idx: int, col_idx: int):
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

    special_stars = list()

    for check in checks:
        c = data[check[0]][check[1]]
        if isinstance(c, Star):
            special_stars.append(c.get_id())

    return special_stars


def get_gear_ratios(test=False) -> list:
    """
    e.g. 467 and 35

    467..114..
    ...*......
    ..35..633.

    123.456  ..123..  ......
    ...*...  ...*...  123*456

    if number next to *, add number to a dict and store the star id
    - then if there are two numbers with the same start id, they are
      valid gears

    1 2 3
    4 . 5
    6 7 8

    data = [[1,2,3],[4,.,5],[6,7,8]]

    """
    data = list()
    with open(f"{pathlib.Path().resolve()}/{INPUT_SOURCE}", "r") as f:
        for line in f:
            # replace * with object that has ID
            line = [Star() if e == "*" else e for e in line]
            # remove '\n' from end and add '.'
            data.append(["."] + list(line[0:-1]) + ["."])
    # add padding at idx 0 and idx -1
    padding = ["." for _ in range(len(data[0]))]
    data.insert(0, padding)
    data.append(padding)
    # print(data)

    if not test:
        validate_data(data)

    values = dict()

    for row_num, row in enumerate(data):
        # dont check first or last row (padding)
        if row_num < 1 or row_num >= len(data) - 1:
            continue
        c: str
        current_num = []
        valid = False
        star_ids = []
        for col_num, c in enumerate(row):
            if col_num >= len(data[0]) - 1:
                # end of row, check if we have a number
                if valid:
                    for star in set(star_ids):
                        values[star].append(int("".join(current_num)))
            # dont check first or last col (padding)
            if col_num < 1 or col_num >= len(data[0]) - 1:
                continue

            # for debugging
            if current_num == ["9", "1", "9"]:
                x = 1

            if str(c) in "0123456789":
                current_num.append(c)
                special_stars = check_special_star_char(data, row_num, col_num)
                if special_stars:
                    valid = True
                    star_ids = star_ids + special_stars
                    for star in special_stars:
                        if star not in values:
                            values[star] = list()
            elif current_num:
                if valid:
                    for star in set(star_ids):
                        values[star].append(int("".join(current_num)))
                current_num.clear()
                valid = False
                star_ids = []

    valid_ratios = []

    for gears in values.values():
        if len(gears) == 2:
            valid_ratios.append(gears[0] * gears[1])

    return valid_ratios


def main():
    valid_ratios = get_gear_ratios()
    total = sum(valid_ratios)
    print(f"sum of all ratios: {total}")


if __name__ == "__main__":
    main()
