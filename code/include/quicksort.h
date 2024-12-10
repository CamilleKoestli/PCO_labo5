#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <queue>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>
#include "multithreadedsort.h"
#include "utils.h"

/**
 * @brief Implémente l'algorithme de tri rapide multi-threadé.
 */
template<typename T>
class Quicksort : public MultithreadedSort<T> {
public:
    Quicksort(unsigned int nbThreads) : MultithreadedSort<T>(nbThreads), stop(false) {
        for (unsigned int i = 0; i < nbThreads; ++i) {
            threads.emplace_back(&Quicksort::worker, this);
        }
    }

    ~Quicksort() {
        mutex.lock();
        stop = true;
        mutex.unlock();

        condVar.notifyAll();

        for (auto& thread : threads) {
            thread.join();
        }
    }

    /**
     * @brief Trie un tableau en utilisant le tri rapide.
     * @param array Tableau à trier
     */
    void sort(std::vector<T>& array) override {
        if (array.empty()) {
            return; 
        }

        this->array = &array; 
        mutex.lock();
        tasks.emplace(0, array.size() - 1); 
        mutex.unlock();

        condVar.notifyOne();

        mutex.lock();
        while (!tasks.empty() || activeThreads > 0) {
            finished.wait(&mutex);
        }
        mutex.unlock();

        if (!isSorted(array)) {
            throw std::runtime_error("Array is not sorted correctly");
        }
    }

private:
    struct Task {
        unsigned int lo, hi;

        Task(unsigned int low, unsigned int high) : lo(low), hi(high) {}
    };

    PcoMutex mutex;                       // Accès à la section critique
    PcoConditionVariable condVar;         // Gestion des threads
    PcoConditionVariable finished;        // Signaler la fin des tâches
    std::queue<Task> tasks;               // File de tâches
    std::vector<PcoThread> threads;       // Threads de travail
    unsigned int activeThreads = 0;       // Nombre de threads actifs
    bool stop = false;                    // Indique si les threads doivent s'arrêter
    std::vector<T>* array;                // Tableau partagé à trier

    /**
     * @brief Exécutée par chaque thread pour faire le tri.
     */
    void worker() {
        mutex.lock();

        while (true) {
            Task task(0, 0);

            while (!stop && tasks.empty()) {
                condVar.wait(&mutex);
            }

            if (stop) {
                break;
            }

            task = tasks.front();
            tasks.pop();
            activeThreads++;
            mutex.unlock();

            quicksort(*array, task.lo, task.hi);

            mutex.lock();
            activeThreads--;
            if (tasks.empty() && activeThreads == 0) {
                finished.notifyAll();
            }
        }
        mutex.unlock();
    }

    /**
     * @brief Trie un tableau en utilisant le tri rapide.
     * @param array Tableau à trier
     * @param lo L'indice inférieur du sous-tableau
     * @param hi L'indice supérieur du sous-tableau
     */
    void quicksort(std::vector<T>& array, int lo, int hi) {
        if (lo >= hi || lo < 0) {
            return;
        }

        if (hi - lo < 1000) {
            std::sort(array.begin() + lo, array.begin() + hi + 1);
            return;
        }

        int pivotIndex = partition(array, lo, hi);

        mutex.lock();
        if (lo != pivotIndex-1) {
            tasks.emplace(lo, pivotIndex - 1);
        }
        if (hi != pivotIndex+1) {
            tasks.emplace(pivotIndex + 1, hi); 
        }
        mutex.unlock();

        condVar.notifyOne();
    }

    /**
     * @brief Divise le tableau en deux partitions.
     * @param array Tableau à partitionner
     * @param lo L'indice inférieur du sous-tableau
     * @param hi L'indice supérieur du sous-tableau
     * @return L'indice du pivot après la partition
     */
    int partition(std::vector<T>& array, int lo, int hi) {
        T pivot = array[hi];
        int i = lo;

        for (int j = lo; j < hi; ++j) {
            if (array[j] <= pivot) {
                std::swap(array[i], array[j]);
                ++i;
            }
        }
        std::swap(array[i], array[hi]);
        return i;
    }
};

#endif // QUICKSORT_H
