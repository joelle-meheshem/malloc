# 🚀 MiniMalloc – Custom Memory Allocator in C

## 📌 Overview

MiniMalloc is a custom implementation of a dynamic memory allocator in C, inspired by the behavior of `malloc`, `free`, and `realloc`.

The project demonstrates low-level memory management by manually controlling heap allocation using system calls and managing memory blocks with metadata.

---

## ⚙️ Features

* ✅ Custom `my_malloc` and `my_free`
* ✅ `my_realloc` implementation
* ✅ Best-fit allocation strategy
* ✅ Block splitting to reduce fragmentation
* ✅ Coalescing (merging adjacent free blocks)
* ✅ Thread-safe using `pthread_mutex`
* ✅ Heap expansion using `sbrk`
* ✅ Heap visualization with `print_heap`

---

## 🧩 Memory Layout

```
[ stats header ][ block ][ block ][ block ] ...
```

Each block contains metadata:

```
| marker | prev | next | in_use | size |
```

---

## 🧪 Example Usage

```c
#include "my_malloc.h"

int main() {
    int *arr = (int *)my_malloc(5 * sizeof(int));

    for (int i = 0; i < 5; i++) {
        arr[i] = i * 10;
    }

    arr = (int *)my_realloc(arr, 10 * sizeof(int));

    my_free(arr);
}
```

---

## 🔍 Debugging (Heap Visualization)

The allocator includes a debug function:

```c
print_heap();
```

Example output:

```
Heap state:
[USED | size=20] -> [FREE | size=4000] -> NULL
```

---

## 🛠️ Build & Run

```bash
make
./test
```

Or manually:

```bash
gcc -Wall -Wextra -pthread -Iinclude src/my_malloc.c tests/main.c -o test
./test
```

---

## 📂 Project Structure

```
malloc_project/
│
├── include/
│   └── my_malloc.h
│
├── src/
│   └── my_malloc.c
│
├── tests/
│   └── main.c
│
├── Makefile
└── README.md
```

---

## 🧠 Key Concepts Demonstrated

* Heap memory management
* Fragmentation handling
* Pointer arithmetic
* Linked-list based memory tracking
* Thread synchronization
* Low-level systems programming

---

## 🔥 Recent Improvements

* Added `realloc` support
* Implemented heap visualization (`print_heap`)
* Refactored project into modular structure (src/include/tests)
* Added Makefile for easier compilation

---

## 📊 Future Improvements

* [ ] Replace `sbrk` with `mmap`
* [ ] Add memory alignment support
* [ ] Detect double free errors
* [ ] Add memory corruption protection (canaries)
* [ ] Implement performance benchmarks

---

## 👩‍💻 Author

Joelle Meheshem
Computer Science Student
