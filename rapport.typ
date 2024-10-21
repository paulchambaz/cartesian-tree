#import "@preview/cetz:0.3.0": canvas, draw, tree

= Projet TME -- Arbre Cartésiens

// damien.vergnaud@lip6.fr

== Exercice 1

#figure(caption: [ Arbre cartésien $cal(A)$ ])[
]

a. Il n'existe qu'une unique solution pour cette liste. En effet, toutes les priorités sont différentes, il n'y a donc pas de _choix_ disponible au moment de la construction. Si toutes les priorités n'étaient pas différentes, alors on aurait pas un ordre totale. On pourrait ajouter un critère de sélection, comme la position dans la liste (la clé du nœud).

b. On observe que l'arbre produit à partir des clés suivantes est le même que celui obtenu dans la question 1. On observe que les clés étaient triés selon l'ordre croissant de priorité. On emet donc l'hypothèse suivante : trier les nœuds en suivant l'ordre des priorités, puis construire l'arbre binaire en suivant l'ordre des nœuds résulte en la création d'un arbre cartésien général.

Un arbre cartésien suit deux propriétés, d'une il est un arbre binaire de recherche, c'est à dire que les nœuds sont organisés de manière à ce que, pour tout nœud, les clés de son sous-arbre de gauche soient inférieures à sa clé, et celles de son sous-arbre droit soient supérieure. Cette propriété est satisfaite par la construction de notre arbre. Il doit satisfaire la priorité que les nœuds également organisés selon la priorité, de sorte qu'un parent ait toujours un priorité inférieure à celle de ses enfants. Cette propriété est satisfaite par le fait que la liste est triée par ordre croissant de priorité. Ainsi, un nœud fils aura toujours une priorité supérieure à celle de son parent.

== Exercice 2

b. Dans le cas d'une recherche fructueuse, soit $k$ la profondeur du nœud. On aura $1$ comparaison par échec (tant que l'on est pas encore au nœud) et $1$ comparaison pour valider que la clé est bien la bonne. On aura donc bien $k$ comparaisons, c'est à dire $k$ nœuds parcourus. Dans le cas d'une recherche infructueuse, on note $k_p$ et $k_s$ la profondeur de son prédecesseur et successeur respectivement. On devra alors aller jusqu'à $max{k_p, k_s}$.

== Exercice 3

a. On reprend l'arbre construit en question 1.a (et 1.b). On cherche à ajouter un nouveau nœud $"K4"$ dans cet arbre. Le nœud sera ajouté, suivant la construction d'un arbre binaire, comme le fils droit de $"J12"$. Cependant, soit $cal(P)(n)$ la priorité du nœud $n$, $cal(P)("K4") = 4$ et $cal(P)("J12") = 12$ et $"J12"$ est père de $"K4"$, ce qui contredit la propriété du tas.

b. La complexité de l'ajout dans un arbre binaire est de $O(k)$ (2.b) et la complexité de la rotation est en temps constant. On a donc une complexité totale pour l'insertion dans un arbre cartésien en suivant cette méthode de $O(k)$.

== Exercice 4

== Exercice 5

== Exercice 6
