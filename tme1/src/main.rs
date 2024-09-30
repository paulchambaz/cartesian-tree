use std::time::Instant;

use backpack::{bruteforce_treesearch, greedy, greedy_continuous, Backpack};
use gnuplot::{Figure, PlotOption::{Caption, Color}};

mod backpack;

fn main() {
    // p13 - sujet td
    // p18 - sujet tme

    test_example();



}

pub fn test_example() {
    let backpack = Backpack{
        total_weight: 20,
        weights: vec![14, 10, 8, 6, 5, 2],
        utilities: vec![24, 19, 16, 13, 5, 3],
    };
    let solution = greedy(&backpack);
    solution.print();
    let solution = greedy_continuous(&backpack);
    solution.print();
    if let Some(solution) = bruteforce_treesearch(&backpack) {
        solution.print();
    }
}

pub fn measure_performance() {
    let sec_max = 0.1;
    let mut tmax = 0;
    for r in vec![100, 1_000, 10_000, 100_000, 1_000_000, 10_000_000] {
        let backpack = Backpack::gen_random(r);
        let start = Instant::now();
        let _ = greedy(&backpack);
        let duration = start.elapsed();

        if duration.as_secs_f32() > sec_max {
            tmax = r;
            break;
        }
    }

    let mut x = Vec::new();
    let mut y = Vec::new();

    for i in 1..=10 {
        let t = i * tmax / 10;
        let backpack = Backpack::gen_random(t);
        let start = Instant::now();
        let _ = greedy(&backpack);
        let duration = start.elapsed();

        x.push(t);
        y.push(duration);
    }

    let mut fg = Figure::new();
    fg.axes2d()
        .lines(&x, &y, &[Caption("Greedy"), Color("black")]);
    let _ = fg.show();
}
