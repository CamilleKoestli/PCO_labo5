#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <iostream>
#include <queue>

#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>
#include <pcosynchro/pcosemaphore.h>
#include "multithreadedsort.h"
#include "utils.h"
#include <algorithm>

/**
 * @brief implémente l'algorithme de tri rapide multi-threadé.
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
        // Arrêter les threads
        mutex.lock();
        stop = true;
        condVar.notifyAll();
        mutex.unlock();

        for (auto& thread : threads) {
            thread.join();
        }
    }

    /**
     * @brief trie un tableau en utilisant le tri rapide.
     * @param array tableau à trier
     */
    void sort(std::vector<T>& array) override {
        if (array.empty()) {
        // Rien à trier pour un tableau vide
        return;
        }

        mutex.lock();
        tasks.emplace(&array, 0, array.size() - 1);
        condVar.notifyOne();
        mutex.unlock();

        // Attendre que toutes les tâches soient terminées
        mutex.lock();
        while (!tasks.empty() || activeThreads > 0) {
            finished.wait(&mutex);
        }
        mutex.unlock();

        // Validation du tri
        if (!isSorted(array)) {
            throw std::runtime_error("Array is not sorted correctly");
        }
    }

    std::vector<T> generateRandomSequence(unsigned int size, unsigned int seed) {
        return generateSequence(size, seed);
    }


private:
    struct Task {
        std::vector<T>* array;
        int lo, hi;

        Task(std::vector<T>* arr, int low, int high) : array(arr), lo(low), hi(high) {}
    };

    PcoMutex mutex; // Accès à la SC
    PcoConditionVariable condVar; // Gestion des threads
    PcoConditionVariable finished; // Signaler la fin des tâches
    std::queue<Task> tasks; // File de tâches
    std::vector<PcoThread> threads; // Threads
    unsigned int activeThreads = 0; // Nombre de threads actifs
    bool stop; // Threads doivent s'arrêter

    /**
     * @brief Est exécutée par chaque thread pour traiter les tâches de tri.
     */
    void worker() {
        while (true) {
            Task task(nullptr, 0, 0);

            mutex.lock();
            while (!stop && tasks.empty()) {
                condVar.wait(&mutex);
            }

            if (stop && tasks.empty()) {
                mutex.unlock();
                break;
            }

            task = tasks.front();
            tasks.pop();
            activeThreads++;
            mutex.unlock();

            quicksort(*(task.array), task.lo, task.hi);

            mutex.lock();
            activeThreads--;
            if (tasks.empty() && activeThreads == 0) {
                finished.notifyAll();
            }
            mutex.unlock();
        }
    }

    /**
     * @brief quicksort est une méthode récursive qui trie un tableau en utilisant le tri rapide.
     * @param array tableau à trier
     * @param lo L'indice inférieur du sous-tableau.
     * @param hi L'indice supérieur du sous-tableau.
     */
    void quicksort(std::vector<T>& array, int lo, int hi) {
        // Si les indices sont invalides ou si la partition est petite
        if (lo >= hi) {
            return;
        }

        if (hi - lo < 1000) {
            std::sort(array.begin() + lo, array.begin() + hi + 1);
            return;
        }

        int pivotIndex = partition(array, lo, hi);

        mutex.lock();
        tasks.emplace(&array, lo, pivotIndex - 1);
        tasks.emplace(&array, pivotIndex + 1, hi);
        condVar.notifyOne();
        mutex.unlock();
    }

    /**
     * @brief Divise le tableau en deux partitions
     * @param array tableau à partitionner
     * @param lo L'indice inférieur du sous-tableau.
     * @param hi L'indice supérieur du sous-tableau.
     * @return l'indice du pivot après la partition.
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
