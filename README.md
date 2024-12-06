Search & Rescue Swarm Binary Search Tree Implementation
Project Overview:
A data structure implementation for managing autonomous robotic devices in search and rescue operations. The system supports multiple types of binary search trees (BST, AVL, and Splay) and allows dynamic switching between tree types during runtime.


Features:

Multiple tree type support
Regular Binary Search Tree (BST)
AVL Tree (self-balancing)
Splay Tree (recently accessed nodes at root)
Dynamic tree type conversion
Robot ID-based node organization
Height tracking for all nodes
Memory-efficient implementation

Core Operations:

Insert new robots
Remove robots by ID
Clear entire swarm
Change tree type during runtime
Deep copy support

Technical Details:

Robot IDs must be within MINID-MAXID range
No duplicate IDs allowed
Lowest level nodes have height 0
AVL maintains balance factor ≤ 1
Splay moves accessed nodes to root

Type Conversion Rules:

BST/SPLAY → AVL: Full tree reconstruction
AVL → BST/SPLAY: No reconstruction needed
BST ↔ SPLAY: No reconstruction needed
Any → NONE: Clears all nodes

Project Files:

swarm.h - Class declarations
swarm.cpp - Implementation file
mytest.cpp - Test cases
makefile
