use std::time::Instant;

use backpack::{branch_and_bound, bruteforce_treesearch, bruteforce_treesearch2, bruteforce_treesearch_count, greedy, greedy_continuous, Backpack, Solution};
use gnuplot::{Figure, PlotOption::{Caption, Color}};

mod backpack;

fn main() {
    // p13 - sujet td
    // p18 - sujet tme

    // test_example();
    // measure_performance(greedy);

    measure_performance(branch_and_bound);

    // for i in 1..100 {
    //     let backpack = Backpack::gen_random(24);
    //     let solution_bruteforce = bruteforce_treesearch(&backpack);
    //     let solution_branch_and_bound = branch_and_bound(&backpack);
    //
    //     if solution_bruteforce.utility != solution_branch_and_bound.utility {
    //         panic!("{:?} produced different solutions {:?} vs {:?}", backpack, solution_bruteforce, solution_branch_and_bound);
    //     }
    //     println!("{}", i);
    // }
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
    let solution = bruteforce_treesearch(&backpack);
    solution.print();
    let solution = bruteforce_treesearch2(&backpack);
    solution.print();

    let (_, count) = bruteforce_treesearch_count(&backpack);
    println!("To acquire this solution, we need to step through at most {} leafs", count);

    let solution = branch_and_bound(&backpack);
    solution.print();
}

pub fn measure_performance_greedy() {
    let sec_max = 60.0;
    for t in 10..=100 {
        let backpack = Backpack::gen_random(t);
        let start = Instant::now();
        let _ = bruteforce_treesearch(&backpack);
        let duration = start.elapsed();

        if duration.as_secs_f32() > sec_max {
            println!("Maximum iterations that could be done before {}s was {}", sec_max, t);
            break;
        }
    }
}

pub fn measure_performance(function: fn(&Backpack) -> Solution) {
    let sec_max = 0.1;
    let mut tmax = 0;
    for r in vec![100, 1_000, 10_000, 100_000, 1_000_000, 10_000_000, 100_000_000] {
        println!("{}", r);
        let backpack = Backpack::gen_random(r);
        let start = Instant::now();
        let _ = function(&backpack);
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
        let mut average_duration = 0.0;
        let samples = 10;
        for _ in 1..samples {
            let backpack = Backpack::gen_random(t);
            let start = Instant::now();
            let _ = function(&backpack);
            let duration = start.elapsed().as_secs_f32();
            average_duration += duration;
        }

        x.push(t);
        y.push(average_duration / samples as f32);
    }

    let mut fg = Figure::new();
    fg.axes2d()
        .lines(&x, &y, &[Caption("Greedy"), Color("black")]);
    let _ = fg.show();

}
