# C++ Data Structures and Algorithms Labs

A source-only collection of C++ coursework covering expression parsing, sparse matrices, linked stacks, binary/Huffman trees, recursion and backtracking.

## Contents

- `expression_evaluator`: infix expression processing and test cases.
- `sparse_matrix`: linked sparse-matrix storage, transpose, addition and multiplication.
- `binary_tree`: binary-tree traversal exercises.
- `huffman_tree`: Huffman construction, coding, decoding and compression analysis.
- `eight_queens`: linked-list representation with recursive backtracking.
- `stack`, `task_3`, `task_4`: additional stack and course exercises.

## Verification

All 15 `.cpp` files pass:

```bash
g++ -std=c++17 -fsyntax-only <file.cpp>
```

This is a historical coursework archive, not a unified production library. Several exercises have their own `main()` and should be compiled independently.
