use Backpack::{Backpack, Solution};

pub struct res {
	backpack : &Backpack,
	results : Vector<usize>,
	computed : Vector<bool>
}

impl res {
	pub fn get(&self, i : usize, j : usize) -> usize {
		self.results[i * self.get_weight() + j] // not sure
	}

	pub fn is_computed(self, i : usize, j : usize) -> bool {
		self.computed[i * self.get_weight() + j]
	}

	pub fn get_weight(&self) -> usize {
		b.weight.len + 1
	}

	pub fn get_nb_elt(&self) -> usize {
		b.total_weight + 1
	}


	
	pub fn compute(&self, i : usize, j : usize) -> usize {
		for i in 0..=self.get_weight() {
			self.results[i] = INFINITY;
		}
		for j in 0..self.get_nb_elt() {
			self.results[self.get_weight() + j + 1] = Backpack.weights[j];
		}

		for i in 1..=self.get_nb_elt() {
			for v in 1..=get_weight() {
				let coo = i * self.get_weight() + v;
				if(backpack.utilities[i - 1] > v) {
					self.results[coo] = self.get(i-1, v);
				} else {
					self.results[coo] = min(self.get(i - 1,v), poids(backpack.utilities[i]) + self.get(i - 1, v - valeur(backpack.utilities[i - 1])));
				}
			}
		}
	}
}

pub fn dyn_method(b : &Backpack) -> Solution {
	let len : usize = b.weights.len * (b.total_weight + 1); // cell for full backpack and empty
	let v : Vector<usize> = vec![0; len];

	// ...
	// back
}

// Pour i allant de 0 à nVmax
// 	P[0, v] = +INFINY
// P[1, valeur(a_1)] = poids(a_1)
// Pour i allant de 1 à n
// 	Pour v allant de 1 à nVmax
// 		si valeur(a_i) > v alors
// 			P[i, v] = P[i-1, v]
// 		Sinon
// 			P[i,v] = min{P[i-1, poids(a[i+1]) + P[i-1, v - valeur(a[i])]]}
