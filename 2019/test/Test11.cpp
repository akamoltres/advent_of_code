
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_11){};

TEST(DAY_11, TEST_SOL)
{
    CHECK_EQUAL(1771, solve_2019_11_1("input/11.txt"));

    const char *part2_sol =
        ".#..#..##..####.#..#...##.#..#.#..#.####...\n"
        ".#..#.#..#.#....#..#....#.#..#.#..#....#...\n"
        ".####.#....###..####....#.####.#..#...#....\n"
        ".#..#.#.##.#....#..#....#.#..#.#..#..#.....\n"
        ".#..#.#..#.#....#..#.#..#.#..#.#..#.#......\n"
        ".#..#..###.####.#..#..##..#..#..##..####...\n";
    char *part2_retval = solve_2019_11_2("input/11.txt");
    STRCMP_EQUAL(part2_sol, part2_retval);
    free(part2_retval);
};
