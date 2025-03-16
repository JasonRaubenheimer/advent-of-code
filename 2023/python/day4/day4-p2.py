"""
https://adventofcode.com/2023/day/4

correct answer: Total cards: 5744979

Takes long to run...
"""

import pathlib
from typing import List, Tuple

TESTING = False

# path from cwd (cwd=/advent-of-code-2023)
INPUT_SOURCE = "python/day4/day4_input.txt"
INPUT_SOURCE_TESTING = "python/day4/day4_input_testing.txt"

FILE_INPUT_PATH = INPUT_SOURCE_TESTING if TESTING else INPUT_SOURCE


def str_2_int(str_list: List[str]) -> Tuple[int]:
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
    return tuple(int_list)


def read_and_parse_cards() -> Tuple[Tuple[int, Tuple[int], Tuple[int]]]:
    """
    Read cards from file and parse into winning nums and card nums.

    Returns a tuple where each element is the info for a card,
        the "info" is another tuple of 3 elements,
        the first element is the card number,
        the second element is a tuple of winning nums, and
        the third element is a tuple of card nums.
    E.g. for the following input data:
        Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
        Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
    The return tuple will be:
        (
            (1, (41, 48, 83, 86, 17), (83, 86, 6, 31, 17, 9, 48, 53)),
            (2, (13, 32, 20, 16, 61), (61, 30, 68, 82, 17, 32, 24, 19)),
        )
    """
    cards = list()
    path = f"{pathlib.Path().resolve()}/{FILE_INPUT_PATH}"
    with open(path, "r") as f:
        # assume line num == card num
        for card_num, line in enumerate(f, start=1):
            # split and strip into lists of string nums
            winning_nums = line.split("|")[0].split(":")[1].strip().split(" ")
            card_nums = line.split("|")[1].strip().split(" ")
            # convert string nums into ints to remove '' elements (probably unnecessary?)
            winning_nums = str_2_int(winning_nums)
            card_nums = str_2_int(card_nums)
            cards.append((card_num, winning_nums, card_nums))
            # print(winning_nums)
            # print(card_nums)
    return tuple(cards)


def num_winning_nums(winning_nums: Tuple[int], card_nums: Tuple[int]) -> int:
    """return the number of winning nums found in card nums"""
    total = 0
    for w_num in winning_nums:
        if w_num in card_nums:
            total += 1
    return total


def get_cards_points(input_cards: Tuple[Tuple[Tuple[int], Tuple[int]]]) -> int:
    """
    What this function does:

    Iterates over each card
        checks if there are winning numbers
            if there are winning numbers,
                add the additional cards to the list of cards to iterate over

    Once there are no more cards to iterate over, check
    how long the total list is and return that len()


    Input is a tuple where each element is the info for a card,
        the "info" is another tuple of two element, the first
        element is a tuple of winning nums, and the second
        element is a tuple of card nums.
    E.g. for the following input data:
        Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
        Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
    The input tuple will be:
        (
            ((41, 48, 83, 86, 17), (83, 86, 6, 31, 17, 9, 48, 53)),
            ((13, 32, 20, 16, 61), (61, 30, 68, 82, 17, 32, 24, 19)),
        )
    """
    cards = list(input_cards)

    for idx, card in enumerate(cards):
        w_nums = num_winning_nums(card[1], card[2])
        # print(f"card: {idx + 1}", w_nums)

        # need to insert the next w_nums cards into the cards list
        # at this position
        card_num = card[0]
        for i in range(1, w_nums + 1):
            # just insert all cards at the next position, order doesn't matter
            idx_insert = idx + 1
            # this is the index of the actual card to insert
            card_index_insert = (card_num - 1) + i
            cards.insert(idx_insert, input_cards[card_index_insert])

    return len(cards)


def main():
    cards = read_and_parse_cards()
    total = get_cards_points(cards)
    print(f"Total cards: {total}")


if __name__ == "__main__":
    main()
