use rand::Rng;


#[derive(Debug)]
pub struct Backpack {
    pub total_weight: u64,
    pub weights: Vec<u64>,
    pub utilities: Vec<u64>,
}

#[derive(Debug)]
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
    let len = backpack.weights.len();

    let mut ratios: Vec<_> = backpack.weights.iter()
        .zip(backpack.utilities.iter())
        .enumerate()
        .map(|(i, (&weight, &utility))| (i, utility as f64 / weight as f64))
        .collect();



    ratios.sort_by(|(_, ratio_i), (_, ratio_j)| {
        if let Some(ordering) = ratio_i.partial_cmp(&ratio_j) {
            ordering.reverse()
        } else {
            panic!("the variables are not comparable...");
        }
    });

    let mut remaining_weight = backpack.total_weight;
    let mut utilities = 0.0;
    let mut values = vec![0.0; len];

    for i in 0..len {
        let (index, _) = ratios[i];
        let weight = backpack.weights[index];

        if weight > remaining_weight {
            continue;
        }

        remaining_weight -= weight;
        utilities += backpack.utilities[index] as f64;
        values[index] = 1.0;

        if remaining_weight <= 0 {
            break;
        }
    }

    Solution{utility: utilities, values}
}


pub fn greedy_continuous(backpack: &Backpack) -> Solution {
    let len = backpack.weights.len();

    let mut ratios: Vec<_> = backpack.weights.iter()
        .zip(backpack.utilities.iter())
        .enumerate()
        .map(|(i, (&weight, &utility))| (i, utility as f64 / weight as f64))
        .collect();

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

pub fn bruteforce_treesearch(backpack: &Backpack) -> Solution {
    bruteforce_treesearch_runner_slow(backpack, 0, vec![], 0)
}

pub fn bruteforce_treesearch_count(backpack: &Backpack) -> (Solution, u32) {
    bruteforce_treesearch_runner_slower(backpack, 0, vec![])
}


pub fn bruteforce_treesearch_runner_slower(backpack: &Backpack, i: usize, pos: Vec<usize>) -> (Solution, u32) {
    let len = backpack.weights.len();

    if i >= len {
        let mut values: Vec<f64> = vec![0.0; len];
        let mut total_utility = 0.0;
        let mut total_weight = 0;
        for p in pos {
            values[p] = 1.0;
            total_weight += backpack.weights[p];
            total_utility += backpack.utilities[p] as f64;
        }

        if total_weight > backpack.total_weight {
            return (Solution{utility : 0.0, values : vec![0.0; len]}, 1);
        }

        return (Solution{utility: total_utility, values}, 1);
    }

    let mut pos_clone = pos.clone();
    pos_clone.push(i);

    let (left, lcount) = bruteforce_treesearch_runner_slower(backpack, i + 1, pos_clone);
    let (right, rcount) = bruteforce_treesearch_runner_slower(backpack, i + 1, pos);

    if left.utility > right.utility {
        (left, lcount + rcount)
    } else {
        (right, lcount + rcount)
    }
}

pub fn bruteforce_treesearch_runner_slow(backpack: &Backpack, i: usize, pos: Vec<usize>, weight: u64) -> Solution {
    let len = backpack.weights.len();

    if weight > backpack.total_weight {
        return Solution{utility : 0.0, values : vec![0.0; len]};
    }

    if i >= len {
        let mut values: Vec<f64> = vec![0.0; len];
        let mut total_utility = 0.0;
        let mut total_weight = 0;
        for p in pos {
            values[p] = 1.0;
            total_weight += backpack.weights[p];
            total_utility += backpack.utilities[p] as f64;
        }

        if total_weight > backpack.total_weight {
            return Solution{utility : 0.0, values : vec![0.0; len]};
        }

        return Solution{utility: total_utility, values};
    }

    let mut pos_clone = pos.clone();
    pos_clone.push(i);

    let left = bruteforce_treesearch_runner_slow(backpack, i + 1, pos_clone, weight + backpack.weights[i]);
    let right = bruteforce_treesearch_runner_slow(backpack, i + 1, pos, weight);

    if left.utility > right.utility {
        left
    } else {
        right
    }
}

pub fn bruteforce_treesearch2(backpack: &Backpack) -> Solution {
    bruteforce_treesearch_runner2(backpack, 0, &vec![], 0)
}

pub fn bruteforce_treesearch_runner2(backpack: &Backpack, i: usize, pos: &Vec<usize>, weight: u64) -> Solution {
    let len = backpack.weights.len();
    if i >= len {
        let mut values: Vec<f64> = vec![0.0; len];
        let mut utility = 0.0;
        for p in pos {
            let p = *p;
            values[p] = 1.0;
            utility += backpack.utilities[p] as f64;
        }

        if weight > backpack.total_weight {
            return Solution{utility : 0.0, values : vec![0.0; len]};
        }

        return Solution{utility, values};
    }

    let res = bruteforce_treesearch_runner2(backpack, i + 1, &pos, weight);
    if (weight + backpack.weights[i]) <= backpack.total_weight {
        let mut pos_clone = pos.clone();
        pos_clone.push(i);
        let res_left = bruteforce_treesearch_runner2(backpack, i + 1, &pos_clone, weight + backpack.weights[i]);
        if res_left.utility > res.utility {
            res_left
        } else {
            res
        }
    } else {
        res
    }
}

pub fn branch_and_bound(backpack: &Backpack) -> Solution {
    let mut best = Solution{
        utility: 0.0,
        values: vec![],
    };
    branch_and_bound_runner(backpack, 0, vec![], 0.0, 0, &mut best);
    best
}

pub fn greedy_bab(backpack: &Backpack, i: usize, weight: u64) -> (Vec<usize>, f64) {
    let mut remaining_weight = backpack.total_weight - weight;

    let mut ratios: Vec<_> = backpack.weights.iter()
        .zip(backpack.utilities.iter())
        .enumerate()
        .filter(|(j, (&weight, &_))| (*j >= i) && (weight <= remaining_weight))
        .map(|(i, (&weight, &utility))| (i, utility as f64 / weight as f64))
        .collect();


    ratios.sort_by(|(_, ratio_i), (_, ratio_j)| {
        if let Some(ordering) = ratio_i.partial_cmp(&ratio_j) {
            ordering.reverse()
        } else {
            panic!("the variables are not comparable...");
        }
    });


    let mut utilities = 0.0;
    let mut values = vec![];

    for (index, _) in ratios {
        let weight = backpack.weights[index];

        if weight > remaining_weight {
            continue;
        }

        remaining_weight -= weight;
        utilities += backpack.utilities[index] as f64;
        values.push(index);

        if remaining_weight <= 0 {
            break;
        }
    }

    (values, utilities)
}


pub fn greedy_continuous_bab(backpack: &Backpack, i: usize, weight: u64) -> f64 {
    let len = backpack.weights.len();
    let mut remaining_weight = backpack.total_weight - weight;


    let mut ratios: Vec<_> = backpack.weights.iter()
        .zip(backpack.utilities.iter())
        .enumerate()
        .filter(|(j, (&_, &_))| *j >= i)
        .map(|(i, (&weight, &utility))| (i, utility as f64 / weight as f64))
        .collect();

    ratios.sort_by(|(_, ratio_i), (_, ratio_j)| {
        if let Some(ordering) = ratio_i.partial_cmp(&ratio_j) {
            ordering.reverse()
        } else {
            panic!("the variables are not comparable...");
        }
    });


    let mut total_utility = 0.0;
    let mut solution = vec![0.0; len];

    for (index, _) in ratios {
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

    total_utility
}

pub fn branch_and_bound_runner(backpack: &Backpack, i: usize, pos: Vec<usize>, utility: f64, weight: u64, best: &mut Solution) {
    let len = backpack.weights.len();

    if weight > backpack.total_weight {
        return
    }

    if i >= len {
        return
    }

    let (min_values, min_utility) = greedy_bab(backpack, i, weight);
    let total_min_utility = min_utility + utility;


    if total_min_utility > best.utility {
        best.utility = total_min_utility;

        let mut values = vec![0.0; len];
        for i in min_values {
            values[i] = 1.0;
        }
        for i in &pos {
            values[*i] = 1.0;
        }
        
        best.values = values;
        
    }

    let max = greedy_continuous_bab(backpack, i, weight);
    let total_max_utility = max + utility;


    if total_min_utility >= total_max_utility {
        return
    }

    if best.utility > total_max_utility {
        return
    }

    let mut pos_clone = pos.clone();
    pos_clone.push(i);


    let _ = branch_and_bound_runner(backpack, i + 1, pos_clone, utility + (backpack.utilities[i] as f64), weight + backpack.weights[i], best);
    let _ = branch_and_bound_runner(backpack, i + 1, pos, utility, weight, best);
}
