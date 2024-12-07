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
    Quicksort<int> sorter(2);
    std::vector<int> array = {42}; 
    sorter.sort(array);
    EXPECT_EQ(array.size(), 1); 
    EXPECT_EQ(array[0], 42); 
}

/**
 * @brief Tri d'un tableau de 10 éléments avec un seul thread.
 */
TEST(SortingTest, SingleThreadSmallArray) {
    Quicksort<int> sorter(1);
    std::vector<int> array = generateSequence(10, 42); 
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array)); 
}

/**
 * @brief Tri d'un tableau de 100000 avec un seul thread.
 */
TEST(SortingTest, SingleThreadLargeArray) {
    Quicksort<int> sorter(1);
    std::vector<int> array = generateSequence(100000, 42); 
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array));
}

/**
 * @brief Tri d'un tableau de 1000000 avec plusieurs threads.
 */
TEST(SortingTest, ManyThreadsLargeArray) {
    Quicksort<int> sorter(8);
    std::vector<int> array = generateSequence(1000000, 123); 
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array)); 
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
