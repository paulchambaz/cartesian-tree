#import "@preview/cetz:0.3.0"

#let report(
  title: none,
  course: none,
  authors: (),
  university: none,
  reference: none,
  doc
) = {
  set text(size: 11pt, lang: "fr", font: "New Computer Modern")

  set page(
    numbering: "1",
    margin: (x: 2cm, y: 3cm),
    header: [
      #set text(weight: 400, size: 10pt)
      #stack(dir: ttb, 
        stack(dir: ltr,
          course,
          h(1fr),
          [ #authors.join(" ", last: " & ") ],
        ),
        v(.1cm),
        line(length: 100%, stroke: .4pt)
      )
    ],
    footer: [
      #set text(weight: 400, size: 10pt)
      #stack(dir: ltr,
          university,
          h(1fr),
          [ #context { counter(page).display("1") } ],
          h(1fr),
          reference,
      )
    ],
  )

  set par(justify: true)

  show heading.where(
    level: 2
  ): it => block(width: 100%)[
    #v(0.2cm)
    #set align(center)
    #set text(13pt, weight: 500)
    #smallcaps(it.body)
    #v(0.2cm)
  ]

  show heading.where(
    level: 3
  ): it => text(
    size: 11pt,
    weight: "regular",
    style: "italic",
    it.body + [.],
  )

  align(center)[
    #v(.5cm)
    #rect(inset: .4cm, stroke: .4pt)[
      = #title
    ]
    #v(1cm)
  ]

  show: rest => columns(2, rest)

  doc
}

#let calc-width(left: none, right: none) = {
  import cetz.draw: *
  if left == none and right == none { return 1.0 }

  let left-width = if left != none { calc-width(
    left: left.at("left", default: none),
    right: left.at("right", default: none)
  )} else { 0.0 }

  let right-width = if right != none { calc-width(
    left: right.at("left", default: none),
    right: right.at("right", default: none)
  )} else { 0.0 }

  return calc.max(left-width + right-width, 1.0)
}
    
#let min-separation(depth) = {
  import cetz.draw: *
  return calc.max(2.0 * calc.pow(0.8, depth), 1.0)
}
    
#let get-offset(left-width, right-width, depth, spread) = {
  import cetz.draw: *
  let min-sep = min-separation(depth)

  let total-width = left-width + right-width
  let base-offset = calc.max(total-width * 0.5, min-sep) * spread

  return base-offset * calc.pow(0.8, depth)
}
    
#let bintree(position, label, left: none, right: none, depth: 0, spread: 0.6) = {
  import cetz.draw: *
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
  content((position.at(0), position.at(1) + 0.0), label)
}
