"""
https://adventofcode.com/2023/day/2
"""

import pathlib
from types import MappingProxyType

TEST = True

# path from cwd (cwd=/advent-of-code-2023)
INPUT_SOURCE = "python/day2/day2_input.txt"


def is_game_possible(line: str) -> int:
    """
    example input: Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green

    Rules: only 12 red cubes, 13 green cubes, and 14 blue cubes

    Plan:
    - parse data, need map of colour:number
    - make sure the colour exists (not sure if this is a possibility but why not check)
    - make sure the number does not exceed the max for the colour

    Returns: game number if game is possible, otherwise it returns -1
    """
    # immutable dict
    truth = MappingProxyType(
        {
            "red": 12,
            "green": 13,
            "blue": 14,
        }
    )

    # first split to get "Game 10", then split at the " " to get the ID
    game_id = line.split(":")[0].split(" ")[1]

    for round in line.split(":")[1].split(";"):
        # round eg: "12 red, 2 green, 5 blue"
        for cubes in round.split(","):
            num = int(cubes.strip().split(" ")[0])
            colour = cubes.strip().split(" ")[1]

            if colour not in truth:
                return -1

            if num > truth[colour]:
                return -1

    return int(game_id)


def calculate_power(line: str) -> int:
    """
    fewest number of cubes of each color that could have been in the bag to make the game possible
    -> i.e. max number of each colour

    power = red * blue * green
    """
    cube_nums = {"red": 0, "blue": 0, "green": 0}
    for round in line.split(":")[1].split(";"):
        # round eg: "12 red, 2 green, 5 blue"
        for cubes in round.split(","):
            num = int(cubes.strip().split(" ")[0])
            colour = cubes.strip().split(" ")[1]

            if cube_nums[colour] < num:
                cube_nums[colour] = num

    return cube_nums["red"] * cube_nums["blue"] * cube_nums["green"]


def main():
    total = 0
    power_total = 0
    with open(f"{pathlib.Path().resolve()}/{INPUT_SOURCE}", "r") as f:
        for line in f:
            res = is_game_possible(line)
            if res != -1:
                total += res

            # part 2
            power_total += calculate_power(line)

    print(f"sum of all possible game IDs: {total}")
    print(f"sum of all game powers: {power_total}")


def test_is_game_possible():
    # Rules: only 12 red cubes, 13 green cubes, and 14 blue cubes
    # (input, output)
    test_cases = (
        (
            "Game 1: 12 red, 2 green, 5 blue; 9 red, 6 green, 4 blue; 10 red, 2 green, 5 blue; 8 blue, 9 red",
            1,
        ),
        (
            "Game 2: 3 green, 7 red; 3 blue, 5 red; 2 green, 1 blue, 6 red; 3 green, 2 red, 3 blue",
            2,
        ),
        (
            "Game 3: 12 red, 18 blue, 3 green; 14 red, 4 blue, 2 green; 4 green, 15 red",
            -1,
        ),
        (
            "Game 3: 12 red, 1 blue, 3 green; 14 red, 4 blue, 2 green; 4 green, 15 red",
            -1,
        ),
    )

    for test in test_cases:
        assert is_game_possible(test[0]) == test[1]


if __name__ == "__main__":
    if TEST:
        test_is_game_possible()
    main()
