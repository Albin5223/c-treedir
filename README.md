# C-projet--TREEDIR

Ce projet consiste à simuler un shell en C.

# Comment lancer le shell ?

Avant de commencer, assurez-vous d'être dans le repértoire "src".

## Depuis un terminal Windows :

```
gcc -c *c ; gcc -o prog *.o ; ./prog
```

## Depuis un terminal Linux :

```
gcc -c *c && gcc -o prog *.o && ./prog
```

## Si les commandes simultanées ne fonctionnent pas :

```
gcc -c *c
gcc -o prog *.o
./prog
```

## `Makefile`
```
> make clean (pour supprimer les résidus)
> make prog (pour créer l'executable)
```

# Binôme

### PARIS Albin
### YAZICI Servan