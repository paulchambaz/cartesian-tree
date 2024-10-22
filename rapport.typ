#import "@preview/cetz:0.3.0"

// #set text(size: 11pt, font: "New Computer Modern")
#set text(size: 11pt)

#set page(
  numbering: "1",
  margin: (x: 2cm, y: 3cm),
  header: [
    #set text(weight: 400, size: 10pt)
    #stack(dir: ttb, 
      stack(dir: ltr,
        [ Complexité, algorithmes randomisés et approchés ],
        h(1fr),
        [ Paul Chambaz & Damien Vergnaud ],
      ),
      v(.1cm),
      line(length: 100%, stroke: .4pt)
    )
  ],
  footer: [
    #set text(weight: 400, size: 10pt)
    #stack(dir: ltr,
        [ Sorbonne Université ],
        h(1fr),
        [#counter(page).display(
          "1",
        )],
        h(1fr),
        [ Master #smallcaps[Ai2d] & #smallcaps[Cca] M1 -- 2024 ],
    )
  ],
)


#let calc-width(left: none, right: none) = {
  import cetz.draw: *
  // Base case: leaf node needs width of 1
  if left == none and right == none { return 1.0 }

  // Calculate width needed for left and right subtrees
  let left-width = if left != none { calc-width(
    left: left.at("left", default: none),
    right: left.at("right", default: none)
  )} else { 0.0 }

  let right-width = if right != none { calc-width(
    left: right.at("left", default: none),
    right: right.at("right", default: none)
  )} else { 0.0 }

  // Return sum of children's widths plus spacing
  // This ensures enough space for both subtrees
  return calc.max(left-width + right-width, 1.0)
}
    
#let min-separation(depth) = {
  import cetz.draw: *
  // Minimum separation decreases with depth but never below 1
  return calc.max(2.0 * calc.pow(0.8, depth), 1.0)
}
    
#let get-offset(left-width, right-width, depth, spread) = {
  import cetz.draw: *
  // Get minimum separation needed at this depth
  let min-sep = min-separation(depth)

  // Calculate offset based on children widths and minimum separation
  let total-width = left-width + right-width
  let base-offset = calc.max(total-width * 0.5, min-sep) * spread

  // Scale down with depth
  return base-offset * calc.pow(0.8, depth)
}
    
#let bintree(position, label, left: none, right: none, depth: 0, spread: 0.6) = {
  import cetz.draw: *
  // Calculate subtree widths
  let left-width = if left != none { 
    calc-width(
      left: left.at("left", default: none),
      right: left.at("right", default: none)
    )
  } else { 0.0 }

  let right-width = if right != none {
    calc-width(
      left: right.at("left", default: none),
      right: right.at("right", default: none)
    )
  } else { 0.0 }

  let x-offset = get-offset(left-width, right-width, depth, spread)

  // Draw children first
  if left != none {
  let left-pos = (position.at(0) - x-offset, position.at(1) - 1)
  line(position, left-pos)
  bintree(
    left-pos,
    left.label,
    left: left.at("left", default: none),
    right: left.at("right", default: none),
    depth: depth + 1,
    spread: spread
  )
}

  if right != none {
  let right-pos = (position.at(0) + x-offset, position.at(1) - 1)
  line(position, right-pos)
  bintree(
    right-pos,
    right.label,
    left: right.at("left", default: none),
    right: right.at("right", default: none),
    depth: depth + 1,
    spread: spread
  )
}

  circle(position, radius: 0.4cm, fill: rgb("#ccccff"))
  content((position.at(0), position.at(1) + 0.1), label)
}

#set par(justify: true)

#show heading.where(
  level: 2
): it => block(width: 100%)[
  #v(0.2cm)
  #set align(center)
  #set text(13pt, weight: 500)
  #smallcaps(it.body)
]

#show heading.where(
  level: 3
): it => text(
  size: 11pt,
  weight: "regular",
  style: "italic",
  it.body + [.],
)

#align(center)[
  #v(.5cm)
  #rect(inset: .4cm, stroke: .4pt)[
    = Projet #smallcaps[Complex] \ Arbre Cartésiens
  ]
  #v(1cm)
]


#show: rest => columns(2, rest)

== Exercice 1

#figure(caption: [ Arbre cartésien $cal(A)$ ])[
  #v(.5cm)
  #cetz.canvas({
    bintree(
      (0, 0),
      [ $H_1$ ],
      left: (
        label: [ $D_2$ ], 
        left: (
          label: [ $B_3$ ],
          left: (
            label: [ $A_5$ ],
          ),
          right: (
            label: [ $C_8$ ],
          ),
        ),
        right: (
          label: [ $E_6$ ],
          right: (
            label: [ $F_7$ ],
            right: (
              label: [ $G_9$ ],
            )
          ),
        ),
      ),
      right: (
        label: [ $I_10$ ], 
        right: (
          label: [ $J_12$ ],
        ),
      ),
    )
})
  #v(.5cm)
]

=== a
Il n'existe qu'une unique solution pour cette liste. En effet, toutes les priorités sont différentes, il n'y a donc pas de _choix_ disponible au moment de la construction. Si toutes les priorités n'étaient pas différentes, alors on aurait pas un ordre totale. On pourrait ajouter un critère de sélection, comme la position dans la liste (la clé du nœud).

=== b
On observe que l'arbre produit à partir des clés suivantes est le même que celui obtenu dans la question 1. On observe que les clés étaient triés selon l'ordre croissant de priorité. On emet donc l'hypothèse suivante : trier les nœuds en suivant l'ordre des priorités, puis construire l'arbre binaire en suivant l'ordre des nœuds résulte en la création d'un arbre cartésien général.

Un arbre cartésien suit deux propriétés, d'une il est un arbre binaire de recherche, c'est à dire que les nœuds sont organisés de manière à ce que, pour tout nœud, les clés de son sous-arbre de gauche soient inférieures à sa clé, et celles de son sous-arbre droit soient supérieure. Cette propriété est satisfaite par la construction de notre arbre. Il doit satisfaire la priorité que les nœuds également organisés selon la priorité, de sorte qu'un parent ait toujours un priorité inférieure à celle de ses enfants. Cette propriété est satisfaite par le fait que la liste est triée par ordre croissant de priorité. Ainsi, un nœud fils aura toujours une priorité supérieure à celle de son parent.

=== c
_Voir `Node.cpp` et `Node.h` pour l'implémentation du nœud._

=== d
_Voir `CartesianTree.cpp` et `CartesianTree.h` pour l'implémentation de l'arbre cartésien._

=== e
_Voir la fonction `exercice_1` pour la construction "manuelle" de l'arbre cartésien de la figure 1._

== Exercice 2

=== a
_Voir la fonction `CartesianTree::find` pour l'implémentation de la recherche d'un nœud et `exercice_2` pour un exemple de recherche minimal._

=== b
Dans le cas d'une recherche fructueuse, soit $k$ la profondeur du nœud. On aura $1$ comparaison par échec (tant que l'on est pas encore au nœud) et $1$ comparaison pour valider que la clé est bien la bonne. On aura donc bien $k$ comparaisons, c'est à dire $k$ nœuds parcourus. Dans le cas d'une recherche infructueuse, on note $k_p$ et $k_s$ la profondeur de son prédecesseur et successeur respectivement. On devra alors aller jusqu'à $max{k_p, k_s}$.

== Exercice 3

=== a
On reprend l'arbre construit en question 1.a (et 1.b). On cherche à ajouter un nouveau nœud $K_4$ dans cet arbre. Le nœud sera ajouté, suivant la construction d'un arbre binaire, comme le fils droit de $J_12$. Cependant, soit $cal(P)(n)$ la priorité du nœud $n$, $cal(P)(K_4) = 4$ et $cal(P)(J_12) = 12$ et $J_12$ est père de $K_4$, ce qui contredit la propriété du tas.

#figure(caption: [ L'ajout de $K_4$ à $cal(A)$ est faux ])[
  #v(.5cm)
  #cetz.canvas({
    bintree(
      (0, 0),
      [ $H_1$ ],
      left: (
        label: [ $D_2$ ], 
        left: (
          label: [ $B_3$ ],
          left: (
            label: [ $A_5$ ],
          ),
          right: (
            label: [ $C_8$ ],
          ),
        ),
        right: (
          label: [ $E_6$ ],
          right: (
            label: [ $F_7$ ],
            right: (
              label: [ $G_9$ ],
            )
          ),
        ),
      ),
      right: (
        label: [ $I_10$ ], 
        right: (
          label: [ $J_12$ ],
          right: (
            label: [ #text(fill: black, [ $bold(K_4)$ ]) ],
          ),
        ),
      ),
    )
})
  #v(.5cm)
]

=== b
La complexité de l'ajout dans un arbre binaire est de $O(k)$ (2.b) et la complexité de la rotation est en temps constant. On a donc une complexité totale pour l'insertion dans un arbre cartésien en suivant cette méthode de $O(k)$.

=== c
_Voir la fonction `CartesianTree::insert` pour l'implémentation de l'insertion d'un nouveau nœud dans un arbre cartésien._

=== d
_Voir la fonction `exercice_3` pour un la construction de l'arbre de la figure 1 avec différents ordres pour les nœuds._

== Exercice 4

=== a
#lorem(100)

=== b
#lorem(100)

=== c
_#lorem(20)_

=== d
_#lorem(20)_

== Exercice 5

=== a
_#lorem(20)_
#lorem(100)

== Exercice 6

=== a
#lorem(100)

=== b
#lorem(100)

=== c
#lorem(100)
