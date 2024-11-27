#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <random>

/**
 * @brief Vérifie si un vecteur est trié dans l'ordre ascendant
 * @param vec le vecteur à vérifier
 * @return true si trié, false sinon
 */
template<typename T>
bool isSorted(const std::vector<T>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i - 1] > vec[i]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Cette fonction génère aléatoirement une séquence de nombres de la taille requise
 * @param seqLen la taille de la séquence à trier
 * @param seed utilisé pour seeder la génération aléatoire de la séquence à trier
 * @return séquence à trier
 */
std::vector<int> generateSequence(unsigned int seqLen, unsigned int seed)
{
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(0, seqLen); // [0,seqLen]

    std::vector<int> randomSequence;
    randomSequence.reserve(seqLen);

    for (unsigned int i = 0; i < seqLen; ++i) {
        int randomValue = distribution(generator);
        randomSequence.push_back(randomValue);
    }

    return randomSequence;
}

#endif // UTILS_H
