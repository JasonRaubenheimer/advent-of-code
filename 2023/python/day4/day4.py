"""
https://adventofcode.com/2023/day/4

tried:

"""

import pathlib
from typing import List

TESTING = False

# path from cwd (cwd=/advent-of-code-2023)
INPUT_SOURCE = "python/day4/day4_input.txt"
INPUT_SOURCE_TESTING = "python/day4/day4_input_testing.txt"

FILE_INPUT_PATH = INPUT_SOURCE_TESTING if TESTING else INPUT_SOURCE


def str_2_int(str_list: List[str]) -> List[int]:
    int_list = list()
    for element in str_list:
        try:
            int_list.append(int(element))
        except ValueError:
            # print("ValueError OK")
            pass
        except Exception as e:
            print(f"what is this? --> {e}")
            print(f"str_list: {str_list}")
            print(f"element: {element}")
    return int_list


def get_cards_points() -> List:
    """"""
    cards_points = list()
    path = f"{pathlib.Path().resolve()}/{FILE_INPUT_PATH}"
    with open(path, "r") as f:
        for line in f:
            # split and strip into lists of string nums
            winning_nums = line.split("|")[0].split(":")[1].strip().split(" ")
            card_nums = line.split("|")[1].strip().split(" ")
            # convert string nums into ints to remove '' elements (probably unnecessary?)
            winning_nums = str_2_int(winning_nums)
            card_nums = str_2_int(card_nums)
            # see what winning nums are in the card nums
            total = 0
            for w_num in winning_nums:
                if w_num in card_nums:
                    if total == 0:
                        total = 1
                    else:
                        total = total * 2
            cards_points.append(total)
            # print(winning_nums)
            # print(card_nums)
    return cards_points


def main():
    cards_points = get_cards_points()
    total = sum(cards_points)
    print(f"sum of all cards points: {total}")


if __name__ == "__main__":
    main()
