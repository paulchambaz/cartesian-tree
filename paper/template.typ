#import "@preview/cetz:0.3.1": canvas, draw
#import "@preview/statastic:1.0.0": arrayLinearRegression, arrayMedian, arrayAvg

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
  import draw: *
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
  import draw: *
  return calc.max(2.0 * calc.pow(0.8, depth), 1.0)
}
    
#let get-offset(left-width, right-width, depth, spread) = {
  import draw: *
  let min-sep = min-separation(depth)

  let total-width = left-width + right-width
  let base-offset = calc.max(total-width * 0.5, min-sep) * spread

  return base-offset * calc.pow(0.8, depth)
}
    
#let bintree(position, label, left: none, right: none, depth: 0, spread: 0.6) = {
  import draw: *
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

#let plot-colors = (
  oklch(100%, 50%, 30deg, 100%),
  oklch(80%, 50%, 30deg, 100%),
  oklch(60%, 50%, 30deg, 100%),

  oklch(100%, 50%, 140deg, 100%),
  oklch(80%, 50%, 140deg, 100%),
  oklch(60%, 50%, 140deg, 100%),

  oklch(100%, 50%, 260deg, 100%),
  oklch(80%, 50%, 260deg, 100%),
  oklch(60%, 50%, 260deg, 100%),
)

#let to-screen(value, min-val, max-val) = {
  if min-val == max-val { return 0 }
  (value - min-val) / (max-val - min-val)
}

#let format-tick-value(value) = {
  let digits = if calc.abs(value) < 1 {
    2
  } else if calc.abs(value) < 10 {
    1
  } else {
    0
  }
  
  return calc.round(value, digits: digits)
}

#let generate-ticks(min-val, max-val, step) = {
  let start = calc.ceil(min-val / step) * step
  let end = calc.floor(max-val / step) * step
  let ticks = ()
  let current = start
  while current <= end {
    ticks.push(format-tick-value(current))
    current = current + step
  }
  return ticks
}

#let find-tick-step(min-val, max-val, target-ticks: 10) = {
  let range = max-val - min-val
  let magnitude = calc.pow(10, calc.floor(calc.log(range + 0.01) / calc.log(10)))
  let possible-steps = (0.1, 0.2, 0.3, 0.5, 1, 2, 5, 10, 20, 50)
  
  for step in possible-steps {
    let scaled-step = step * magnitude
    let num-ticks = calc.ceil(range / scaled-step)
    if num-ticks <= target-ticks {
      return scaled-step
    }
  }
  return possible-steps.last() * magnitude
}

#let plot-heap-operations(data) = {
  let data-push = data.filter(el => el.at("operation") == "push")
  let data-pop = data.filter(el => el.at("operation") == "pop")

  let ns = data.map(el => el.at("n")).dedup()

  let median-push = ()
  let median-pop = ()
  for n in ns {
    let n-push-data = data-push.filter(el => el.at("n") == n).map(el => el.at("time_us"))
    median-push.push(
      (x: n, y: arrayMedian(n-push-data))
    )
    let n-pop-data = data-pop.filter(el => el.at("n") == n).map(el => el.at("time_us"))
    median-pop.push(
      (x: n, y: arrayMedian(n-pop-data))
    )
  }

  let median-push-data = median-push.flatten().map(el => el.at("y"))
  let median-pop-data = median-pop.flatten().map(el => el.at("y"))

  let x-min = int(ns.first());
  let x-max = int(ns.last());

  let y-push-min = calc.min(..median-push-data)
  let y-push-max = calc.max(..median-push-data)
  let y-pop-min = calc.min(..median-pop-data)
  let y-pop-max = calc.max(..median-pop-data)

  let y-min = int(calc.min(y-push-min, y-pop-min))
  let y-max = int(calc.max(y-push-max, y-pop-max))

  let x-ticks = generate-ticks(x-min, x-max, find-tick-step(x-min, x-max))
  let y-ticks = generate-ticks(y-min, y-max, find-tick-step(y-min, y-max))

  canvas(length: 5cm, {
    import draw: *

    set-style(
      mark: (fill: black),
      stroke: 0.5pt,
    )

    line((0, 0), (1.1, 0), mark: (end: "stealth"))
    line((0, 0), (0, 1.1), mark: (end: "stealth"))

    for x in x-ticks {
      if x <= x-max {
        let x-pos = to-screen(x, x-min, x-max)
        line((x-pos, 0), (x-pos, 1), stroke: gray + 0.5pt)
        line((x-pos, 0), (x-pos, -0.02), stroke: black)
        content((x-pos, -0.05), angle: 45deg, anchor: "north", text(size: 0.5em)[#x])
      }
    }

    for y in y-ticks {
      if y <= y-max {
        let y-pos = to-screen(y, y-min, y-max)
        line((0, y-pos), (1, y-pos), stroke: gray + 0.5pt)
        line((0, y-pos), (-0.02, y-pos), stroke: black)
        content((-0.03, y-pos), anchor: "east", text(size: 0.5em)[#y])
      }
    }

    for point in median-push {
      let x = to-screen(int(point.x), x-min, x-max)
      let y = to-screen(int(point.y), y-min, y-max)
      circle((x, y), radius: 0.01, stroke: none, fill: plot-colors.at(1))
    }

    for i in range(1, median-push.len()) {
      let curr = median-push.at(i)
      let prev = median-push.at(i - 1)
      let curr-x = to-screen(int(curr.x), x-min, x-max)
      let curr-y = to-screen(int(curr.y), y-min, y-max)
      let prev-x = to-screen(int(prev.x), x-min, x-max)
      let prev-y = to-screen(int(prev.y), y-min, y-max)
      line((prev-x, prev-y), (curr-x, curr-y), stroke: plot-colors.at(1))
    }

    for point in median-pop {
      let x = to-screen(int(point.x), x-min, x-max)
      let y = to-screen(int(point.y), y-min, y-max)
      circle((x, y), radius: 0.01, stroke: none, fill: plot-colors.at(7))
    }

    for i in range(1, median-pop.len()) {
      let curr = median-pop.at(i)
      let prev = median-pop.at(i - 1)
      let curr-x = to-screen(int(curr.x), x-min, x-max)
      let curr-y = to-screen(int(curr.y), y-min, y-max)
      let prev-x = to-screen(int(prev.x), x-min, x-max)
      let prev-y = to-screen(int(prev.y), y-min, y-max)
      line((prev-x, prev-y), (curr-x, curr-y), stroke: plot-colors.at(7))
    }

    // Add legend
    let legend-start = 0.7
    let y-pos = legend-start + (0 * 0.06)
    circle((1.1, y-pos), radius: 0.01, stroke: none, fill: plot-colors.at(1))
    content((1.13, y-pos), text(size: 0.5em)[ push ], anchor: "west")

    let y-pos = legend-start + (1 * 0.06)
    circle((1.1, y-pos), radius: 0.01, stroke: none, fill: plot-colors.at(7))
    content((1.13, y-pos), text(size: 0.5em)[ pop ], anchor: "west")
    // }
  })
}
#let plot-bst-operations(data, operation) = {
  // Filter data for the specific operation
  let data-filtered = data.filter(el => el.at("operation") == operation)
  
  // Get unique patterns and n values
  let patterns = ("random", "ascending", "descending")
  let ns = data.map(el => el.at("n")).dedup()
  
  // Initialize medians for each pattern
  let medians = ()
  
  // Calculate medians for each pattern
  for pattern in patterns {
    let pattern-data = data-filtered.filter(el => el.at("pattern") == pattern)
    let pattern-medians = ()
    
    for n in ns {
      let n-data = pattern-data.filter(el => el.at("n") == n)
                              .map(el => int(el.at("time_us")))
      if n-data.len() > 0 {
        pattern-medians.push((
          x: n,
          y: arrayMedian(n-data)
        ))
      }
    }
    medians.push(pattern-medians)
  }
  
  // Calculate min/max values for axes
  let x-min = int(ns.first())
  let x-max = int(ns.last())
  
  let all-y-values = medians.flatten().map(el => el.at("y"))
  let y-min = 0  // Start from 0 for timing measurements
  let y-max = int(calc.max(..all-y-values))
  
  // Generate ticks
  let x-ticks = generate-ticks(x-min, x-max, find-tick-step(x-min, x-max))
  let y-ticks = generate-ticks(y-min, y-max, find-tick-step(y-min, y-max))
  
  canvas(length: 5cm, {
    import draw: *
    
    set-style(
      mark: (fill: black),
      stroke: 0.5pt,
    )
    
    // Draw axes
    line((0, 0), (1.1, 0), mark: (end: "stealth"))
    line((0, 0), (0, 1.1), mark: (end: "stealth"))
    
    // Draw x-axis ticks and grid
    for x in x-ticks {
      if x <= x-max {
        let x-pos = to-screen(x, x-min, x-max)
        line((x-pos, 0), (x-pos, 1), stroke: gray + 0.5pt)
        line((x-pos, 0), (x-pos, -0.02), stroke: black)
        content((x-pos, -0.05), angle: 45deg, anchor: "north", text(size: 0.5em)[#x])
      }
    }
    
    // Draw y-axis ticks and grid
    for y in y-ticks {
      if y <= y-max {
        let y-pos = to-screen(y, y-min, y-max)
        line((0, y-pos), (1, y-pos), stroke: gray + 0.5pt)
        line((0, y-pos), (-0.02, y-pos), stroke: black)
        content((-0.03, y-pos), anchor: "east", text(size: 0.5em)[#y])
      }
    }
    
    // Plot lines and points for each pattern
    for (pattern-idx, pattern-medians) in medians.enumerate() {
      let color = plot-colors.at(pattern-idx * 3 + 1)
      
      // Draw points
      for point in pattern-medians {
        let x = to-screen(int(point.x), x-min, x-max)
        let y = to-screen(int(point.y), y-min, y-max)
        circle((x, y), radius: 0.01, stroke: none, fill: color)
      }
      
      // Draw lines between points
      for i in range(1, pattern-medians.len()) {
        let curr = pattern-medians.at(i)
        let prev = pattern-medians.at(i - 1)
        let curr-x = to-screen(int(curr.x), x-min, x-max)
        let curr-y = to-screen(int(curr.y), y-min, y-max)
        let prev-x = to-screen(int(prev.x), x-min, x-max)
        let prev-y = to-screen(int(prev.y), y-min, y-max)
        line((prev-x, prev-y), (curr-x, curr-y), stroke: color)
      }
    }
    
    // Add legend
    let legend-start = 0.7
    for (i, pattern) in patterns.enumerate() {
      let color = plot-colors.at(i * 3 + 1)
      let y-pos = legend-start + (i * 0.06)
      circle((1.1, y-pos), radius: 0.01, stroke: none, fill: color)
      content((1.13, y-pos), text(size: 0.5em)[#pattern], anchor: "west")
    }
  })
}
#let plot-cartesian-operations(data, operation) = {
  // Filter data for the specific operation
  let data-filtered = data.filter(el => el.at("operation") == operation)
  
  // Get unique patterns, collision rates (alphas) and n values
  let patterns = ("random", "ascending", "descending")
  let alphas = (0, 0.01, 0.1)
  let ns = data.map(el => el.at("n")).dedup()
  
  // Initialize medians for each pattern and alpha combination
  let medians = ()
  
  // Calculate medians for each pattern and alpha
  for pattern in patterns {
    for alpha in alphas {
      let pattern-alpha-data = data-filtered.filter(el => 
        el.at("pattern") == pattern and 
        float(el.at("collision_rate")) == alpha
      )
      let pattern-alpha-medians = ()
      
      for n in ns {
        let n-data = pattern-alpha-data.filter(el => el.at("n") == n)
                                     .map(el => int(el.at("time_us")))
        if n-data.len() > 0 {
          pattern-alpha-medians.push((
            x: n,
            y: arrayMedian(n-data)
          ))
        }
      }
      medians.push((
        pattern: pattern,
        alpha: alpha,
        data: pattern-alpha-medians
      ))
    }
  }
  
  // Calculate min/max values for axes
  let x-min = int(ns.first())
  let x-max = int(ns.last())
  
  let all-y-values = medians.map(m => m.at("data"))
                           .flatten()
                           .map(el => el.at("y"))
  let y-min = 0  // Start from 0 for timing measurements
  let y-max = int(calc.max(..all-y-values))
  
  // Generate ticks
  let x-ticks = generate-ticks(x-min, x-max, find-tick-step(x-min, x-max))
  let y-ticks = generate-ticks(y-min, y-max, find-tick-step(y-min, y-max))
  
  canvas(length: 5cm, {
    import draw: *
    
    set-style(
      mark: (fill: black),
      stroke: 0.5pt,
    )
    
    // Draw axes
    line((0, 0), (1.1, 0), mark: (end: "stealth"))
    line((0, 0), (0, 1.1), mark: (end: "stealth"))
    
    // Draw x-axis ticks and grid
    for x in x-ticks {
      if x <= x-max {
        let x-pos = to-screen(x, x-min, x-max)
        line((x-pos, 0), (x-pos, 1), stroke: gray + 0.5pt)
        line((x-pos, 0), (x-pos, -0.02), stroke: black)
        content((x-pos, -0.05), angle: 45deg, anchor: "north", text(size: 0.5em)[#x])
      }
    }
    
    // Draw y-axis ticks and grid
    for y in y-ticks {
      if y <= y-max {
        let y-pos = to-screen(y, y-min, y-max)
        line((0, y-pos), (1, y-pos), stroke: gray + 0.5pt)
        line((0, y-pos), (-0.02, y-pos), stroke: black)
        content((-0.03, y-pos), anchor: "east", text(size: 0.5em)[#y])
      }
    }
    
    // Plot lines and points for each pattern and alpha
    for (idx, median-group) in medians.enumerate() {
      let pattern = median-group.at("pattern")
      let alpha = median-group.at("alpha")
      let pattern-data = median-group.at("data")
      
      // Calculate color index (3 colors per pattern)
      let pattern-idx = patterns.position(p => p == pattern)
      let alpha-idx = alphas.position(a => a == alpha)
      let color-idx = pattern-idx * 3 + alpha-idx
      let color = plot-colors.at(color-idx)
      
      // Draw points
      for point in pattern-data {
        let x = to-screen(int(point.x), x-min, x-max)
        let y = to-screen(int(point.y), y-min, y-max)
        circle((x, y), radius: 0.01, stroke: none, fill: color)
      }
      
      // Draw lines between points
      for i in range(1, pattern-data.len()) {
        let curr = pattern-data.at(i)
        let prev = pattern-data.at(i - 1)
        let curr-x = to-screen(int(curr.x), x-min, x-max)
        let curr-y = to-screen(int(curr.y), y-min, y-max)
        let prev-x = to-screen(int(prev.x), x-min, x-max)
        let prev-y = to-screen(int(prev.y), y-min, y-max)
        line((prev-x, prev-y), (curr-x, curr-y), stroke: color)
      }
    }
    
    // Add legend
    let legend-start = 0.3
    for (p-idx, pattern) in patterns.enumerate() {
      for (a-idx, alpha) in alphas.enumerate() {
        let color-idx = p-idx * 3 + a-idx
        let color = plot-colors.at(color-idx)
        let y-pos = legend-start + (p-idx * 0.18) + (a-idx * 0.06)
        circle((1.1, y-pos), radius: 0.01, stroke: none, fill: color)
        content((1.13, y-pos), 
          text(size: 0.5em)[#pattern (#alpha)], 
          anchor: "west"
        )
      }
    }
  })
}

#let plot-bst-structure(data, metrics) = {
  let patterns = ("random", "ascending", "descending")
  let ns = data.map(el => el.at("n")).dedup()
  
  let medians = ()
  
  // Calculate medians for each pattern
  for pattern in patterns {
    let pattern-medians = ()
    let pattern-data = data.filter(el => el.at("pattern") == pattern)
    
    for n in ns {
      let n-data = pattern-data.filter(el => el.at("n") == n)
      if n-data.len() > 0 {
        let values1 = n-data.map(el => float(el.at(metrics.at(0))))
        let values2 = n-data.map(el => float(el.at(metrics.at(1))))
        pattern-medians.push((
          x: n,
          y1: arrayMedian(values1),
          y2: arrayMedian(values2)
        ))
      }
    }
    medians.push(pattern-medians)
  }
  
  let x-min = int(ns.first())
  let x-max = int(ns.last())
  
  let all-y1-values = medians.flatten().map(el => el.at("y1"))
  let all-y2-values = medians.flatten().map(el => el.at("y2"))
  let y-min = calc.min(calc.min(..all-y1-values), calc.min(..all-y2-values))
  let y-max = calc.max(calc.max(..all-y1-values), calc.max(..all-y2-values))
  
  let x-ticks = generate-ticks(x-min, x-max, find-tick-step(x-min, x-max))
  let y-ticks = generate-ticks(y-min, y-max, find-tick-step(y-min, y-max))
  
  canvas(length: 5cm, {
    import draw: *
    set-style(mark: (fill: black), stroke: 0.5pt)
    
    line((0, 0), (1.1, 0), mark: (end: "stealth"))
    line((0, 0), (0, 1.1), mark: (end: "stealth"))
    
    for x in x-ticks {
      if x <= x-max {
        let x-pos = to-screen(x, x-min, x-max)
        line((x-pos, 0), (x-pos, 1), stroke: gray + 0.5pt)
        line((x-pos, 0), (x-pos, -0.02), stroke: black)
        content((x-pos, -0.05), angle: 45deg, anchor: "north", text(size: 0.5em)[#x])
      }
    }
    
    for y in y-ticks {
      if y <= y-max {
        let y-pos = to-screen(y, y-min, y-max)
        line((0, y-pos), (1, y-pos), stroke: gray + 0.5pt)
        line((0, y-pos), (-0.02, y-pos), stroke: black)
        content((-0.03, y-pos), anchor: "east", text(size: 0.5em)[#y])
      }
    }
    
    for (pattern-idx, pattern-medians) in medians.enumerate() {
      let color1 = plot-colors.at(pattern-idx * 3)
      let color2 = plot-colors.at(pattern-idx * 3 + 1)
      
      // First metric
      for point in pattern-medians {
        let x = to-screen(int(point.x), x-min, x-max)
        let y = to-screen(point.y1, y-min, y-max)
        circle((x, y), radius: 0.01, stroke: none, fill: color1)
      }
      
      for i in range(1, pattern-medians.len()) {
        let curr = pattern-medians.at(i)
        let prev = pattern-medians.at(i - 1)
        let curr-x = to-screen(int(curr.x), x-min, x-max)
        let curr-y = to-screen(curr.y1, y-min, y-max)
        let prev-x = to-screen(int(prev.x), x-min, x-max)
        let prev-y = to-screen(prev.y1, y-min, y-max)
        line((prev-x, prev-y), (curr-x, curr-y), stroke: color1)
      }
      
      // Second metric
      for point in pattern-medians {
        let x = to-screen(int(point.x), x-min, x-max)
        let y = to-screen(point.y2, y-min, y-max)
        circle((x, y), radius: 0.01, stroke: none, fill: color2)
      }
      
      for i in range(1, pattern-medians.len()) {
        let curr = pattern-medians.at(i)
        let prev = pattern-medians.at(i - 1)
        let curr-x = to-screen(int(curr.x), x-min, x-max)
        let curr-y = to-screen(curr.y2, y-min, y-max)
        let prev-x = to-screen(int(prev.x), x-min, x-max)
        let prev-y = to-screen(prev.y2, y-min, y-max)
        line((prev-x, prev-y), (curr-x, curr-y), stroke: color2)
      }
    }
    
    // Legend
    let legend-start = 0.7
    for (i, pattern) in patterns.enumerate() {
      let y-pos = legend-start + (i * 0.12)
      let color1 = plot-colors.at(i * 3)
      let color2 = plot-colors.at(i * 3 + 1)
      
      circle((1.1, y-pos), radius: 0.01, stroke: none, fill: color1)
      content((1.13, y-pos), text(size: 0.5em)[#pattern #metrics.at(0)], anchor: "west")
      
      circle((1.1, y-pos + 0.06), radius: 0.01, stroke: none, fill: color2)
      content((1.13, y-pos + 0.06), text(size: 0.5em)[#pattern #metrics.at(1)], anchor: "west")
    }
  })
}

#let plot-cartesian-structure(data, metrics) = {
  let patterns = ("random", "ascending", "descending")
  let alphas = (0, 0.01, 0.1)
  let ns = data.map(el => el.at("n")).dedup()
  
  let medians = ()
  
  for pattern in patterns {
    for alpha in alphas {
      let pattern-alpha-data = data.filter(el => 
        el.at("pattern") == pattern and 
        float(el.at("collision_rate")) == alpha
      )
      let pattern-medians = ()
      
      for n in ns {
        let n-data = pattern-alpha-data.filter(el => el.at("n") == n)
        if n-data.len() > 0 {
          let values1 = n-data.map(el => float(el.at(metrics.at(0))))
          let values2 = n-data.map(el => float(el.at(metrics.at(1))))
          pattern-medians.push((
            x: n,
            y1: arrayMedian(values1),
            y2: arrayMedian(values2)
          ))
        }
      }
      
      if pattern-medians.len() > 0 {
        medians.push((
          pattern: pattern,
          alpha: alpha,
          data: pattern-medians
        ))
      }
    }
  }
  
  let x-min = int(ns.first())
  let x-max = int(ns.last())
  
  let all-y1-values = medians.map(m => m.at("data"))
                           .flatten()
                           .map(el => el.at("y1"))
  let all-y2-values = medians.map(m => m.at("data"))
                           .flatten()
                           .map(el => el.at("y2"))
  
  let y-min = calc.min(calc.min(..all-y1-values), calc.min(..all-y2-values))
  let y-max = calc.max(calc.max(..all-y1-values), calc.max(..all-y2-values))
  
  let x-ticks = generate-ticks(x-min, x-max, find-tick-step(x-min, x-max))
  let y-ticks = generate-ticks(y-min, y-max, find-tick-step(y-min, y-max))
  
  canvas(length: 5cm, {
    import draw: *
    set-style(mark: (fill: black), stroke: 0.5pt)
    
    line((0, 0), (1.1, 0), mark: (end: "stealth"))
    line((0, 0), (0, 1.1), mark: (end: "stealth"))
    
    for x in x-ticks {
      if x <= x-max {
        let x-pos = to-screen(x, x-min, x-max)
        line((x-pos, 0), (x-pos, 1), stroke: gray + 0.5pt)
        line((x-pos, 0), (x-pos, -0.02), stroke: black)
        content((x-pos, -0.05), angle: 45deg, anchor: "north", text(size: 0.5em)[#x])
      }
    }
    
    for y in y-ticks {
      if y <= y-max {
        let y-pos = to-screen(y, y-min, y-max)
        line((0, y-pos), (1, y-pos), stroke: gray + 0.5pt)
        line((0, y-pos), (-0.02, y-pos), stroke: black)
        content((-0.03, y-pos), anchor: "east", text(size: 0.5em)[#y])
      }
    }
    
    for (idx, median-group) in medians.enumerate() {
      let pattern = median-group.at("pattern")
      let alpha = median-group.at("alpha")
      let pattern-data = median-group.at("data")
      
      let pattern-idx = patterns.position(p => p == pattern)
      let alpha-idx = alphas.position(a => a == alpha)
      let color = plot-colors.at(pattern-idx * 3 + alpha-idx)
      
      // First metric
      for point in pattern-data {
        let x = to-screen(int(point.x), x-min, x-max)
        let y = to-screen(point.y1, y-min, y-max)
        circle((x, y), radius: 0.01, stroke: none, fill: color)
      }
      
      for i in range(1, pattern-data.len()) {
        let curr = pattern-data.at(i)
        let prev = pattern-data.at(i - 1)
        let curr-x = to-screen(int(curr.x), x-min, x-max)
        let curr-y = to-screen(curr.y1, y-min, y-max)
        let prev-x = to-screen(int(prev.x), x-min, x-max)
        let prev-y = to-screen(prev.y1, y-min, y-max)
        line((prev-x, prev-y), (curr-x, curr-y), stroke: color)
      }
      
      // Second metric (dashed lines)
      for point in pattern-data {
        let x = to-screen(int(point.x), x-min, x-max)
        let y = to-screen(point.y2, y-min, y-max)
        circle((x, y), radius: 0.01, stroke: none, fill: color)
      }
      
      for i in range(1, pattern-data.len()) {
        let curr = pattern-data.at(i)
        let prev = pattern-data.at(i - 1)
        let curr-x = to-screen(int(curr.x), x-min, x-max)
        let curr-y = to-screen(curr.y2, y-min, y-max)
        let prev-x = to-screen(int(prev.x), x-min, x-max)
        let prev-y = to-screen(prev.y2, y-min, y-max)
        line((prev-x, prev-y), (curr-x, curr-y), stroke: (dash: "dashed", paint: color))
      }
    }
    
    // Legend
    let legend-start = 0.3
    for (p-idx, pattern) in patterns.enumerate() {
      for (a-idx, alpha) in alphas.enumerate() {
        let color = plot-colors.at(p-idx * 3 + a-idx)
        let y-pos = legend-start + (p-idx * 0.24) + (a-idx * 0.08)
        
        circle((1.1, y-pos), radius: 0.01, stroke: none, fill: color)
        content((1.13, y-pos), text(size: 0.5em)[#pattern #alpha #metrics.at(0)], anchor: "west")
        
        let y-pos = y-pos + 0.04
        circle((1.1, y-pos), radius: 0.01, stroke: none, fill: color)
        content((1.13, y-pos), text(size: 0.5em)[#pattern #alpha #metrics.at(1)], anchor: "west")
      }
    }
  })
}
