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
    std::vector<int> array; // Tableau vide
    sorter.sort(array);
    EXPECT_TRUE(array.empty()); // Le tableau reste vide
}

/**
 * @brief Tri d'un tableau avec un seul élément.
 */
TEST(SortingTest, SingleElement) {
    Quicksort<int> sorter(2);
    std::vector<int> array = {42}; // Tableau avec un seul élément
    sorter.sort(array);
    EXPECT_EQ(array.size(), 1); // Taille doit rester 1
    EXPECT_EQ(array[0], 42); // L'élément doit rester inchangé
}

/**
 * @brief Tri d'un petit tableau avec un seul thread.
 */
TEST(SortingTest, SingleThreadSmallArray) {
    Quicksort<int> sorter(1);
    std::vector<int> array = generateSequence(10, 42); // Tableau de 10 éléments
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array)); // Vérifie si le tableau est trié
}

/**
 * @brief Tri d'un grand tableau avec un seul thread.
 */
TEST(SortingTest, SingleThreadLargeArray) {
    Quicksort<int> sorter(1);
    std::vector<int> array = generateSequence(100000, 42); // Tableau de 100k éléments
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array)); // Vérifie si le tableau est trié
}

/**
 * @brief Tri d'un grand tableau avec plusieurs threads.
 */
TEST(SortingTest, MultiThreadLargeArray) {
    Quicksort<int> sorter(4);
    std::vector<int> array = generateSequence(100000, 42); // Tableau de 100k éléments
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array)); // Vérifie si le tableau est trié
}

/**
 * @brief Tri d'un tableau avec des éléments identiques.
 */
TEST(SortingTest, IdenticalElementsArray) {
    Quicksort<int> sorter(4);
    std::vector<int> array(1000, 42); // Tableau de 1000 éléments identiques
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array)); // Vérifie si le tableau est trié
    EXPECT_EQ(array[0], 42); // Vérifie que les valeurs n'ont pas changé
}

/**
 * @brief Tri d'un très grand tableau avec plusieurs threads.
 */
TEST(SortingTest, ManyThreadsLargeArray) {
    Quicksort<int> sorter(8);
    std::vector<int> array = generateSequence(1000000, 123); // Tableau de 1M éléments
    sorter.sort(array);
    EXPECT_TRUE(isSorted(array)); // Vérifie si le tableau est trié
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
