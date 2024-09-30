use backpack::{greedy, Backpack};

mod backpack;

fn main() {
    println!("Hello, world!");

    // p13 - sujet td
    // p18 - sujet tme

    let total_weight = 20;
    let weights = vec![14, 10, 8, 6, 5, 2];
    let utilities = vec![24, 19, 16, 13, 5, 3];
    let backpack = Backpack{total_weight, weights, utilities};


    let (utility, solution) = greedy(backpack);

    print!("solution: ");
    for s in solution {
        print!("{} ", s + 1);

    }
    println!("utility: {}", utility);
    // println!("solution: {:?} of utility: {}", solution, utility);
}
