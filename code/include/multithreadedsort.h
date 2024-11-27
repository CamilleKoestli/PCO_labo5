#ifndef MULTITHREADEDSORT_H
#define MULTITHREADEDSORT_H

#include <vector>

/**
 * @brief The Sort class is abstract and defines what a Sorting class should implement.
 */
template<typename T>
class Sort
{
public:
    virtual void sort(std::vector<T>& array) = 0;
};


/**
 * @brief The MultithreadedSort class is abstract and defines what a Multithreaded Sort class should implement.
 */
template<typename T>
class MultithreadedSort : public Sort<T>
{
public:
    MultithreadedSort(unsigned int nbThreads) : nbThreads(nbThreads) {}

protected:
    unsigned int nbThreads;
};


#endif // MULTITHREADEDSORT_H
