# Library Management System in C (Two Implementations)

This project implements a complete Library Management System (books, authors, and their relationships) using **two different data structures** in C:

1. **Dynamic Arrays** (`array-implementation/`)
2. **Singly Linked Lists** (`linkedlist-implementation/`)

Both versions offer the exact same functionality (Insert, Search, Delete, Save/Load) but use different memory management strategies.

## 🔍 Comparison of Implementations

| Feature | Array Version | Linked List Version |
| :--- | :--- | :--- |
| **Memory Allocation** | `malloc` / `realloc` for contiguous blocks | `malloc` per node (dynamic, non-contiguous) |
| **Sorting Algorithm** | Uses standard `qsort()` from `<stdlib.h>` | Custom **Merge Sort** implemented manually |
| **Search Efficiency** | Uses **Binary Search** (due to contiguous memory) | Uses **Linear Search** (since traversal is needed) |
| **Deletion** | Shifting elements left (O(n)) | Changing pointers (O(1) if node found) |
| **Complexity** | Simpler code, faster access | More complex code, better for frequent insertions/deletions |

## 🚀 How to Run

Navigate to the desired implementation folder and compile:

```bash
# For the Array version
cd array-implementation
gcc library.c -o library_array
./library_array

# For the Linked List version
cd ../linkedlist-implementation
gcc library_linkedLists.c -o library_ll
./library_ll
```

> **Important:** Each folder contains its own `authors.txt`, `books.txt`, and `writes.txt` to avoid conflicts between the two versions.

## Features (Both Versions)

- Insert/Delete Authors & Books
- Search Books (binary in array, linear in list)
- Search Authors
- Automatic saving to `.txt` files
- Handles monographs (when deleting an author, their sole book is also deleted)

##  Purpose

This project was created to demonstrate proficiency in C programming and a deep understanding of fundamental data structures and their trade-offs.

---
