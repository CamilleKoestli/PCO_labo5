#include <gtest/gtest.h>

#include "quicksort.h"
#include "utils.h"

/**
 * @brief test Generates a random sequence of specified size and sorts it with
 * Quicksort using N threads.
 * @param nbThreads number of threads to use to sort the sequence
 * @param size of the sequence to sort
 * @param seed to use for the random generation of the sequence
 */
void test(int nbThreads, int size, int seed) {
    Quicksort<int> sorter(nbThreads);
    std::vector<int> array = generateSequence(size, seed);
    sorter.sort(array);
    EXPECT_FALSE(array.empty());  // check that the result is not empty
    EXPECT_EQ(array.size(), size); // check that the result has the same size
    EXPECT_TRUE(isSorted(array)); // check that result is sorted
}

// TEST(SortingTest, Test1) {
//     // TODO: Modify this test, and add others
//     int size = 0;
//     int nbThreads = 0;
//     int seed = 0;

//     test(nbThreads, size, seed);
// }

/**
 * @brief Tri d'un tableau vide.
 */
TEST(SortingTest, EmptyArray) {
    Quicksort<int> sorter(2);
    std::vector<int> array; 
    sorter.sort(array);
    EXPECT_TRUE(array.empty()); 
}

/**
 * @brief Tri d'un tableau avec un seul élément.
 */
TEST(SortingTest, SingleElement) {
    int size = 1;
    int nbThreads = 2;
    int seed = 42;

    test(nbThreads, size, seed);
}

/**
 * @brief Tri d'un tableau de 10 éléments avec un seul thread.
 */
TEST(SortingTest, SingleThreadSmallArray) {
    int size = 10;
    int nbThreads = 1;
    int seed = 42;

    test(nbThreads, size, seed);
}

/**
 * @brief Tri d'un tableau de 100000 avec un seul thread.
 */
TEST(SortingTest, SingleThreadLargeArray) {
    int size = 100000;
    int nbThreads = 1;
    int seed = 42;

    test(nbThreads, size, seed);
}

/**
 * @brief Tri d'un tableau de 1000000 avec plusieurs threads.
 */
TEST(SortingTest, ManyThreadsLargeArray) {
    int size = 1000000;
    int nbThreads = 8;
    int seed = 123;

    test(nbThreads, size, seed);
}

/**
 * @brief Tri d'un tableau de 1000000 avec beaucoup de threads.
 */
TEST(SortingTest, LotOfThreadsLargeArray) {
    int size = 1000000;
    int nbThreads = 25;
    int seed = 123;

    test(nbThreads, size, seed);
}

/**
 * @brief Tri d'un tableau avec des éléments identiques.
 */
TEST(SortingTest, IdenticalElementsArray) {
    Quicksort<int> sorter(4);
    std::vector<int> array(1000, 42); 
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array));
    EXPECT_EQ(array[0], 42); 
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
