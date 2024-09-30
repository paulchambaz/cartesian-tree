
pub struct Backpack {
    pub total_weight: u32,
    pub weights: Vec<u32>,
    pub utilities: Vec<u32>,
}


pub fn greedy(backpack: Backpack) -> (u32, Vec<usize>) {
    let mut ratios: Vec<(usize, f32)> = Vec::new();
    let len = backpack.weights.len();

    for i in 0..len {
        let ratio = backpack.utilities[i] as f32 / backpack.weights[i] as f32;
        ratios.push((i, ratio));
    }


    ratios.sort_by(|(_, ratio_i), (_, ratio_j)| {
        if let Some(ordering) = ratio_i.partial_cmp(&ratio_j) {
            ordering.reverse()
        } else {
            panic!("the variables are not comparable...");
        }
    });

    let mut total_weight = backpack.total_weight;
    let mut total_utility = 0;
    let mut solution = Vec::new();

    for i in 0..len {
        let (index, _) = ratios[i];
        let weight = backpack.weights[index];

        if weight > total_weight {
            continue;
        }

        let utility = backpack.weights[index];

        total_weight -= weight;
        total_utility += utility;
        solution.push(index);

        if total_weight <= 0 {
            break;
        }
    }

    println!("{:?}", ratios);

    (total_utility, solution)
}


