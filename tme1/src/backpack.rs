use rand::Rng;


pub struct Backpack {
    pub total_weight: u64,
    pub weights: Vec<u64>,
    pub utilities: Vec<u64>,
}

pub struct Solution {
    pub utility: f64,
    pub values: Vec<f64>,
}


impl Backpack {
    pub fn gen_random(r: usize) -> Backpack {
        let mut rng = rand::thread_rng();
        let weights: Vec<u64> = (0..r).map(|_| rng.gen_range(1..r) as u64).collect();
        let utilities: Vec<u64> = (0..r).map(|_| rng.gen_range(1..r) as u64).collect();

        let total_weight: u64 = weights.iter().sum::<u64>() / 2;

        Backpack{total_weight, weights, utilities}
    }
}

impl Solution {
    pub fn print(&self) {
        println!("Solution: {:?}, Utility: {}", self.values, self.utility);
    }
}


pub fn greedy(backpack: &Backpack) -> Solution {
    let mut ratios: Vec<(usize, f64)> = Vec::new();
    let len = backpack.weights.len();

    for i in 0..len {
        let ratio = backpack.utilities[i] as f64 / backpack.weights[i] as f64;
        ratios.push((i, ratio));
    }


    ratios.sort_by(|(_, ratio_i), (_, ratio_j)| {
        if let Some(ordering) = ratio_i.partial_cmp(&ratio_j) {
            ordering.reverse()
        } else {
            panic!("the variables are not comparable...");
        }
    });

    let mut remaining_weight = backpack.total_weight;
    let mut total_utility = 0.0;
    let mut solution = vec![0.0; len];

    for i in 0..len {
        let (index, _) = ratios[i];
        let weight = backpack.weights[index];
        let utility = backpack.utilities[index];

        if weight > remaining_weight {
            continue;
        }


        remaining_weight -= weight;
        total_utility += utility as f64;
        solution[index] = 1.0;

        if remaining_weight <= 0 {
            break;
        }
    }

    Solution{utility: total_utility, values: solution}
}


pub fn greedy_continuous(backpack: &Backpack) -> Solution {
    let mut ratios: Vec<(usize, f64)> = Vec::new();
    let len = backpack.weights.len();

    for i in 0..len {
        let ratio = backpack.utilities[i] as f64 / backpack.weights[i] as f64;
        ratios.push((i, ratio));
    }


    ratios.sort_by(|(_, ratio_i), (_, ratio_j)| {
        if let Some(ordering) = ratio_i.partial_cmp(&ratio_j) {
            ordering.reverse()
        } else {
            panic!("the variables are not comparable...");
        }
    });

    let mut remaining_weight = backpack.total_weight;
    let mut total_utility = 0.0;
    let mut solution = vec![0.0; len];

    for i in 0..len {
        let (index, _) = ratios[i];
        let weight = backpack.weights[index];
        let utility = backpack.utilities[index];


        if weight > remaining_weight {
            let ratio = (remaining_weight as f64) / (weight as f64);
            total_utility += ratio * utility as f64;
            solution[index] = ratio;
            break;
        }

        remaining_weight -= weight;
        total_utility += utility as f64;
        solution[index] = 1.0;

        if remaining_weight <= 0 {
            break;
        }
    }

    Solution{utility: total_utility, values: solution}
}

pub fn bruteforce_treesearch(backpack: &Backpack) -> Option<Solution> {
    if let Some((weight, solution)) = bruteforce_treesearch_runner(backpack, 0, vec![]) {
        Some(solution)
    } else {
        None
    }
}

pub fn bruteforce_treesearch_runner(backpack: &Backpack, i: usize, pos: Vec<usize>) -> Option<(u32, Solution)> {
    let len = backpack.weights.len();
    if i >= len {
        let mut values: Vec<f64> = vec![0.0; len];
        let mut utility = 0.0;
        let mut weight = 0;
        for p in pos {
            values[p] = 1.0;
            weight += backpack.weights[p];
            utility += backpack.utilities[p] as f64;
        }

        if weight > backpack.total_weight {
            return None
        }

        return Some((0, Solution{utility, values}));
    }

    let mut pos_clone = pos.clone();
    pos_clone.push(i);

    if let Some((weight_left, res_left)) = bruteforce_treesearch_runner(backpack, i + 1, pos_clone) {
        if let Some((weight_right, res_right)) = bruteforce_treesearch_runner(backpack, i + 1, pos) {
            if res_left.utility > res_right.utility {
                Some((weight_left, res_left))
            } else {
                Some((weight_right, res_right))
            }
        } else {
            Some((weight_left, res_left))
        }
    } else {
        if let Some((weight_right, res_right)) = bruteforce_treesearch_runner(backpack, i + 1, pos) {
            Some((weight_right, res_right))
        } else {
            None
        }
    }



}