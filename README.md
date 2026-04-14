Mini Malloc - Custom Memory Allocator
A thread-safe, heap-based memory allocator implemented in C. This project mimics the behavior of the standard malloc and free functions by managing memory blocks directly using system calls.

🧠 Implementation Details
The allocator manages a linked list of memory blocks (areas) within a custom heap space initialized via sbrk.

Key Features:
Allocation Strategy: Best-Fit. The allocator scans the free list to find the smallest available block that fits the requested size, reducing wasted memory.

Thread Safety: Fully thread-safe implementation using pthread_mutex to handle concurrent memory requests.

Splitting: When a found block is significantly larger than the requested size, it is split into two to preserve space for future allocations.

Coalescing (Merging): During my_free, the allocator automatically merges adjacent free blocks (both forward and backward) to combat fragmentation.

Safety Markers: Each block contains a BLOCK_MARKER (0xDD) to validate pointers during the free process and prevent corruption.

🏗 Data Structures
The stats Header
Located at the very start of the heap, it tracks global metadata:

magic: Verification constant (0x55).

blocks: Total number of existing blocks.

pages: Number of memory pages allocated from the OS.

The area Struct
Each memory block is preceded by metadata to manage the doubly linked list:

C
typedef struct area {
    uint8_t marker;     // 0xDD for validation
    struct area *prev;  // Pointer to previous block
    struct area *next;  // Pointer to next block
    bool in_use;        // Allocation status
    uint32_t length;    // Size of the usable data area
} area;
🚀 How to Run
Clone the repository:

Bash
git clone https://github.com/joelle-meheshem/malloc.git
cd malloc
Compile the test program:

Bash
gcc -o malloc_test main.c my_malloc.c -lpthread
Run the executable:

Bash
./malloc_test