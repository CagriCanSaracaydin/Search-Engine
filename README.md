## Search Engine-C++ Data Structures: Binary Search Tree & Hash Table

This repository contains a robust implementation of two fundamental data structures in C++: a Binary Search Tree (BST) and a Hash Table. These implementations are designed to provide a clear understanding of the underlying mechanics of each data structure, as well as to serve as a starting point for more complex data handling and manipulation tasks in C++ applications.

### Features

- **Binary Search Tree (BST)**
  - Basic operations: Insert, Search, and Delete.
  - Additional functionalities: In-order traversal, Node counting, and Tree height calculation.
  - Optimized for educational clarity and real-world applicability.
  - Customizable for handling different data types through templates.

- **Hash Table**
  - Separate chaining technique to handle collisions.
  - Dynamic resizing with prime number capacity to optimize distribution.
  - Templated to support various key/value types.
  - Load factor checks and automatic rehashing to maintain operational efficiency.
  - Comprehensive methods for insert, search, and clear operations.

### Structure

- **BST Implementation**
  - Encapsulated within a template class that supports generic key and value types.
  - Private inner `Node` class for tree structure with recursive helper functions to manage tree operations.
  - Public interfaces for user interaction with the tree (insert, search, delete, traverse).

- **Hash Table Implementation**
  - Implemented using a vector of linked lists to manage collisions.
  - Contains a custom hash function and resizing logic based on load factors.
  - Templated to accommodate various types of data as keys and values.

### Getting Started

Clone the repository and include the header files (`BST.h` and `HashTable.h`) in your C++ project. No external dependencies are required, as the project uses only standard libraries. 

```bash
git clone https://github.com/yourusername/cpp-data-structures.git
```

### Usage

Here is a quick example of how to use the BST in your code:

```cpp
#include "BST.h"

int main() {
    BST<int, string> myTree;
    myTree.insert(1, "one");
    myTree.insert(2, "two");
    myTree.search(1);  // Returns "one"
    myTree.deleteNode(1);
    return 0;
}
```
