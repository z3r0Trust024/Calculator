# Calculatrice Scientifique avec C++
La calculatrice est gere par la classe `Calculator`.

La classe **ne possede pas de methode statique** alors pour l'utiliser il faut d'abord *l'instancier*.
Apres, on peut l'utiliser.

## Evaluation d'une expression
Pour evaluer une expression, on utilise la methode `Calculate` de l'objet. Elle prend un **std::string** comme expression a evaluer et retourne un **std::string** qui peut etre le _resultat_ ou un _message d'erreur_ par rapport a l'evaluation de la chaine ou de l'expression.
L'utilisation de cette methode change la valeur de l'attribut `state` de l'objet qui contient l'etat de la derniere evaluation. On utilise l'accesseur `getState` pour recuperer cette valeur.
### Les differents operateurs:
- #### Operateurs de base:
    - \+ : addition
    - \- : soustraction
    - / : division
    - \* : multiplication
    - % : modulo
    - ! : factorielle (la factorielle passe apres la valeur et peut prendre tout valeur dans R<sup>+ </sup> comme elle utilise la fonction `gamma`)

- #### Logarithmes et exponentielle :
    - ln : logarithme neperien
    - log : logarithme de base 10
    - exp : exponentielle
    - ^ : puissance (operateur)

- #### Fonctions trigonometriques:
    - sin : sinus
    - cos : cosinus
    - tan : tangente
    - asin : arcsin
    - acos : arccos
    - atan : arctan

- #### Fonctions hyperboliques:
    - sh : sinus hyperbolique
    - ch : cosinus hyperbolique
    - th : tangente hyperbolique
    - ash : arg sinus hyperbolique 
    - ach : arg cosinus hyperbolique
    - ath : arg tangente hyperbolique 

- #### Arrondissement:
    - ceil : arrondissement vers le haut 
    - floor : arrondissement vers le bas
    - round : arrondissement 

- ### Autres:
    - abs : valeur absolue
    - $ : racine 

_Ps_: 
- Les fonctions doivent etre suivi de parentheses.
- La racine agit comme un operateur et une fonction en meme temps. Cad, elle agit sur 2 valeurs mais oblige une parenthese apres.
Ex:
```
    racine carree de 3 : 2$(3)
    racine cubique de 5 : 3$(6)
```


### Reussite :
__Resultat__ : `std::string` correspondant au resultat de l'evaluation de l'expression
```
    state = 0;
```
## Erreurs :
__Resultat__ : `std::string` correspondant au message d'erreur.
- __Erreur de syntaxe__
    **Cas**:
        - _Erreur sur les parentheses_: les parentheses ne sont pas fermees, les parentheses ouvrantes et fermantes ne se concordent,parentheses vides,etc.
        - Des operateurs qui se suivent sauf des + et - qui se suivent et les operateurs qui se suivent mais acceptables comme /- ou *+.
        - Des operateurs n'ayant aucune valeur a etre utilisees.
        - Les fonctions qui n'ont pas de parentheses.
```
    state = 1;
```

-  __Erreur de calcul__:
    **Cas**: 
    - Division par zero
    - Les fonctions recoivent des valeurs en dehors de leurs domaines de definition.

```
    state = 2;
```

- __Nombre trop grand__:
    **Cas**: Le nombre recu est trop grand ou alors un cas d'overflow.
```
    state = 3;
```

## Modes:
On peut aussi definir differents modes sur la calculatrice.
On change le mode avec la methode `setMode` de l'objet; et aussi, on peut savoir le mode utiliser en utilisant l'accesseur `getMode`.<br>
*ps: Le type traite en question est le `char`.*
__Les differents modes :__
- `'N'` : Le mode **normale**
- `'F'` : le mode **F-E** ou mode **scientifique**
- `'D'` : le mode **dms** (degres,minutes,secondes)

*ps: Le mode par defaut est le mode normale.*

## Angles
La calculatrice prend en charge aussi differents angles.
On peut definir cette angle avec la methode `setAngle` et obtenir l'angle utilise avec `getAngle`. (Le type traite est le char). <br>
__Les angles pris en charge :__
- `'D'` : degree
- `'R'` : radian
- `'G'` : grades

*ps: Par defaut, les angles traites sont en degrees."

## Memoires
La calculatrice gere aussi une memoire pour stocker une valeur.
Toutes les methodes 
On peut le traiter avec les methodes publiques suivantes:
- **MS** : Stocke une valeur (ou l'evaluation d'une expression) dans la memoire 
(Ca peut etre une expression et est evalue avant).
- **MC** : efface la memoire
- **MP** : additionne la valeur a la memoire
- **MM** : soutrait la memoire par la valeur
- **MR** : rappelle la memoire

*Rq* : Toutes ces methodes attendent une string a evaluer (sauf *MR* et *MC*) et retourne la valeur (sauf *MC*).
