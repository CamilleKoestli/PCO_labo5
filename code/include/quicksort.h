#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "multithreadedsort.h"
#include "utils.h"
#include <algorithm>
#include <pcosynchro/pcoconditionvariable.h>
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcothread.h>
#include <queue>
#include <stdexcept>
#include <vector>

/**
 * @brief Implémente l'algorithme de tri rapide multithreadé.
 */
template<typename T>
class Quicksort : public MultithreadedSort<T> {
public:
    Quicksort(int nbThreads) : MultithreadedSort<T>(nbThreads), stop(false) {
        for (int i = 0; i < nbThreads; ++i) {
            threads.emplace_back(&Quicksort::worker, this);
        }
    }

    ~Quicksort() {
        // Les threads doivent s'arrêter
        mutex.lock();
        stop = true;
        mutex.unlock();

        condVar.notifyAll();

        // Attendre que les threads se terminent
        for (auto &thread: threads) {
            thread.join();
        }
    }

    /**
   * @brief Trie un tableau en utilisant le tri rapide.
   * @param array Tableau à trier
   */
    void sort(std::vector<T> &array) override {
        // Si le tableau est vide, il est déjà trié
        if (array.empty()) {
            return;
        }

        this->array = &array;

        // Ajoute une première tâche pour trier tout le tableau
        mutex.lock();
        tasks.emplace(0, array.size() - 1);
        mutex.unlock();

        condVar.notifyOne();

        // Attend que les threads terminent le tri
        mutex.lock();
        while (!tasks.empty() || activeThreads > 0) {
            finished.wait(&mutex);
        }
        mutex.unlock();

        // Vérifie que le tableau est trié
        if (!isSorted(array)) {
            throw std::runtime_error("Array is not sorted correctly");
        }
    }

private:
    struct Task {
        unsigned lo, hi;

        Task(unsigned low, unsigned high) : lo(low), hi(high) {}
    };

    PcoMutex mutex;                // Accès à la section critique
    PcoConditionVariable condVar;  // Gestion des threads
    PcoConditionVariable finished; // Signaler la fin des tâches
    std::queue<Task> tasks;        // File de tâches
    std::vector<PcoThread> threads;// Threads de travail
    unsigned activeThreads = 0;    // Nombre de threads actifs
    bool stop = false;             // Indique si les threads doivent s'arrêter
    std::vector<T> *array;         // Tableau partagé à trier

    /**
   * @brief Exécutée par chaque thread pour faire le tri.
   */
    void worker() {
        mutex.lock();

        while (true) {
            Task task(0, 0);

            // Attend une tâche ou l'arrêt du thread
            while (!stop && tasks.empty()) {
                condVar.wait(&mutex);
            }

            // Quitte la boucle si le tri doit s'arrêter
            if (stop) {
                break;
            }

            // Récupère une tâche à effectuer
            task = tasks.front();
            tasks.pop();
            activeThreads++;
            mutex.unlock();

            // Tri rapide sur le sous-tableau
            int pivotIndex = quicksort(*array, task.lo, task.hi);

            // Ajoute les nouvelles tâches à la file
            mutex.lock();
            if (pivotIndex != -1) {

                if (task.lo != pivotIndex - 1) {
                    tasks.emplace(task.lo, pivotIndex - 1);
                }
                if (task.hi != pivotIndex + 1) {
                    tasks.emplace(pivotIndex + 1, task.hi);
                }
                // Réveille un autre thread pour effectuer une tâche
                condVar.notifyOne();
            }

            activeThreads--;

            // Vérifie si tous les threads et les tâches ont terminé
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
   * @return L'indice du pivot après la partition
   */
    int quicksort(std::vector<T> &array, const unsigned lo, const unsigned hi) {
        // sous tableau de taille 0 ou 1
        if (lo >= hi || lo < 0) {
            return -1;
        }

        // sous tableau assez petit pour être trié avec std::sort
        if (hi - lo < 1000) {
            std::sort(array.begin() + lo, array.begin() + hi + 1);
            return -1;
        }

        return partition(array, lo, hi);
    }

    /**
   * @brief Divise le tableau en deux partitions.
   * @param array Tableau à partitionner
   * @param lo L'indice inférieur du sous-tableau
   * @param hi L'indice supérieur du sous-tableau
   * @return L'indice du pivot après la partition
   */
    unsigned partition(std::vector<T> &array, unsigned lo, unsigned hi) {
        // Choix du pivot
        T pivot = array[hi];
        unsigned i = lo;

        // Partitionnement
        for (unsigned j = lo; j < hi; ++j) {
            if (array[j] <= pivot) {
                std::swap(array[i], array[j]);
                ++i;
            }
        }
        std::swap(array[i], array[hi]);
        return i;
    }
};

#endif// QUICKSORT_H
