# Quicksort multithreadé

Auteurs: Camille Koestli

## Description des fonctionnalités du logiciel

Le logiciel implémente un algorithme de tri rapide (QuickSort) capable d’utiliser plusieurs threads pour améliorer les performances sur des tableaux de grande taille. L'objectif est de paralléliser l'exécution du tri en divisant le tableau en sous-partitions, chacune étant traitée par un thread différent.

## Choix d'implémentation

L'algorithme utilise un modèle de type producteur-consommateur. La partition initiale du tableau est traitée comme une "tâche", et chaque tâche est insérée dans une file partagée entre les threads. Les threads consomment ces tâches et effectuent le tri récursif en ajoutant de nouvelles sous-partitions dans la file.

### Décomposition

La structure `Task` représente une tâche de tri. Elle contient les indices de début et de fin de la partition à trier. Une queue FIFO de tâches est utilisée pour partager les tâches entre les threads. Enfin, le tableau est partagé entre les threads, chaque thread ayant accès à la même instance du tableau, ce qui évite une copie dans chaque tâche.

### Concurrence

Pour éviter les problèmes de concurrence, les threads utilisent des mutex pour accéder à la file de tâches. Je ne protège pas l'accès au tableau, car il n'y a pas de redimensionnement du tableau. De plus, comme chaque thread traite une partition différente, il n'y a pas de problème de concurrence sur les éléments du tableau.

- `mutex` : mutex qui protège les accès à la file de tâches et aux variables partagées.
- `condVar` et `finished`: ces variables de conditions gèrent l'attente des threads sur la file de tâches et les notifie lorsque des tâches sont disponibles ou que le tri est terminé.

### Optimisations

Les partitions de taille inférieure à 1000 éléments sont triées directement avec un `std::sort` pour éviter la surcharge de gestion des threads.

## Tests effectués

### Description des tests

Les tests ont été effectués sur des tableaux de différentes tailles, avec un nombre de threads variable. Ces tests ont été réalisés afin de valider que le tri est correct et donc que notre algorithme est correctement implémenté. De plus, afin de pouvoir vérifier qu'aucun élément n'est supprimé, j'ai rajouté un check dans la fonction `test()` afin de vérifier que la taille de l'array a la même taille. Voici donc les différents tests effectués:

| Tableaux                                             | Résultats |
| ---------------------------------------------------- | --------- |
| Vide avec plusieurs threads                          | OK        |
| 1 élément avec plusieurs threads                     | OK        |
| 10 éléments différents avec un thread                | OK        |
| 100000 éléments différents avec un thread            | OK        |
| 1000000 éléments différents avec plusieurs threads   | OK        |
| 1000000 éléments différents avec beaucoup de threads | OK        |
| 1000 éléments identiques avec plusieurs threads      | OK        |

### Résultats des benchmarks

Les benchmarks ont été effectués sur un tableau de 5000000 éléments aléatoires, avec un nombre de threads variable. Les résultats sont les suivants:

| Nombre de threads | Temps réel (ns) | CPU time (ns) | Itérations |
| ----------------- | --------------- | ------------- | ---------- |
| 1                 | 1385544882      | 143328195     | 1          |
| 2                 | 779300395       | 126960258     | 1          |
| 4                 | 489572542       | 120802969     | 2          |
| 8                 | 359329641       | 121373324     | 2          |
| 16                | 318055731       | 122372508     | 2          |

Le but était de vérifier que le temps d'exécution diminue avec le nombre de threads.

On observe que le temps d'exécution diminue de moitié quand on passe à 2 threads par rapport à 1, puis diminue de manière moins significative avec 4, 8 et 16 threads. Cela est dû au fait que le nombre de coeurs disponibles sur la machine est limité, et que le gain de performance est donc limité par le nombre de coeurs. Concernant le temps du CPU, une note nous dit que le CPU scaling est activé. Ca peut introduire du bruit dans les mesures, car les fréquences peuvent varier.

En conclusion, l'algorithme de tri rapide multithreadé est correctement implémenté et permet d'améliorer les performances en terme de temps d'exécution sur des tableaux de grande taille.
