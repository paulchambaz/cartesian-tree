# Cartesian Trees Implementation

This project implements Cartesian Trees, a probabilistically balanced data structure combining properties of binary search trees and heaps. Developed as part of the Complexity (COMPLEX) course at Sorbonne University, Master AI2D & CCA M1, 2024-2025.

## About Cartesian Trees

Cartesian Trees, proposed by Jean Vuillemin in 1980, maintain a probabilistically balanced structure by combining:

- Binary Search Tree (BST) property on keys
- Min-Heap property on priorities

Each node contains:

- A key (respecting BST order)
- A priority (respecting heap order)

The structure achieves O(log n) average time complexity for operations like:

- Insertion
- Deletion
- Search

## Usage

Compile and run:

```sh
# Compile
make

# Run tests
./target/release/bin
```

The implementation includes:

- Node structure with key and priority
- Basic operations (insert, search, delete)
- Tree rotations for maintaining heap property
- Performance analysis and testing suite

## Authors

- [Damien Assire](https://www.linkedin.com/in/damien-assire-631642295/)
- [Paul Chambaz](https://www.linkedin.com/in/paul-chambaz-17235a158/)

## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.
