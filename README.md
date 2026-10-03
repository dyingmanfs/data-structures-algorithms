# C Data Structures & Algorithms

A collection of C projects focused on fundamental **data structures and algorithms**, including hash tables, collision resolution, dynamic rehashing, directed weighted graphs, adjacency lists, and path finding.

These projects were developed as part of **CNG315 – Algorithms** at METU NCC.

## Projects

### 1. Hash Table Subscription Management

A subscription management system implemented using a dynamically allocated hash table.

The system reads customer and subscription information from a text file and stores customers in a hash table.

Supported collision-resolution techniques:

- Linear Probing
- Quadratic Probing
- Double Hashing

The hash table automatically performs **rehashing when the load factor exceeds 0.5**.

Main features:

- Read subscription data from file
- Group subscriptions by customer
- Dynamically allocate subscription arrays
- Create and manage a hash table
- Linear probing
- Quadratic probing
- Double hashing
- Automatic rehashing
- Prime-number table resizing
- Hash-based customer search
- Display customer subscription information

The hash key is generated using XOR operations on the characters of the customer's name.

### 2. Directed Warehouse Route Graph

A directed weighted graph representing warehouse delivery routes between locations in Cyprus.

The graph is implemented using an **adjacency list**.

Each warehouse is represented as a vertex, while each delivery route is represented as a weighted directed edge.

Main features:

- Read warehouse locations from file
- Read routes and distances from file
- Dynamically create graph vertices
- Create weighted directed edges
- Track vertex indegree and outdegree
- Display the adjacency list
- Find warehouses receiving the most deliveries
- Find warehouses sending the most deliveries
- Find the longest route
- Find the shortest route
- Search for a path between two warehouses
- Calculate total route distance

Example route:

```text
Lefkosa
   ↓ 60 km
Gazimagusa
   ↓ 20 km
Iskele
   ↓ 25 km
Dipkarpaz
```

Total distance:

```text
105 km
```

## Data Structures

### Hash Table

The subscription project uses:

```c
typedef struct {
    int subscriptionID;
    char serviceName[50];
    float serviceCharge;
    int devicesRegistered;
    char startDate[11];
    char endDate[11];
    char status[10];
    char country[50];
} Subscription;
```

and:

```c
typedef struct {
    char name[100];
    int subscriptionCount;
    float subscriptionCostTotal;
    Subscription *subscriptions;
    int subscriptionCapacity;
} Customer;
```

### Graph

The warehouse project uses linked structures for vertices and edges.

```c
struct graphArc {
    int weight;
    struct graphVertex *destination;
    struct graphArc *next;
};
```

```c
typedef struct graphVertex {
    struct graphVertex *next;
    char warehouseName[50];
    int inDegree;
    int outDegree;
    int processed;
    struct graphArc *firstArc;
} Vertex;
```

## Project Structure

```text
c-data-structures-algorithms/
│
├── hash-table-subscription-system/
│   ├── src/
│   │   └── hash_table_subscriptions.c
│   └── data/
│       └── subscriptions.txt
│
├── warehouse-route-graph/
│   ├── src/
│   │   └── warehouse_graph.c
│   │
│   ├── data/
│   │   ├── WarehouseLocations.txt
│   │   └── WarehouseRoutes.txt
│   │
│   ├── images/
│   │   └── warehouse_graph_visualization.jpg
│   │
│   └── sample_output.txt
│
├── README.md
└── .gitignore
```

## Compilation

A C compiler such as GCC can be used.

### Hash Table Project

```bash
gcc hash_table_subscriptions.c -o hash_table
```

Run:

```bash
./hash_table
```

Make sure the required input file is available in the program's working directory.

### Warehouse Graph Project

Compile:

```bash
gcc warehouse_graph.c -o warehouse_graph
```

Run with departure and destination warehouses:

```bash
./warehouse_graph Lefkosa Dipkarpaz
```

The application reads:

```text
WarehouseLocations.txt
WarehouseRoutes.txt
```

and searches for a route between the provided locations.

## Example Warehouse Output

```text
Warehouses receiving deliveries from the most locations:
- Guzelyurt
- Bostanci
- Lapta

Warehouse sending deliveries to the most locations:
- Lefkosa

Route with highest distance:
Lefkosa -60-> Gazimagusa

Route with smallest distance:
Guzelyurt -5-> Kalkanli
```

## Concepts Practiced

- C Programming
- Dynamic Memory Allocation
- Structures
- Pointers
- File I/O
- Linked Lists
- Hash Tables
- Open Addressing
- Linear Probing
- Quadratic Probing
- Double Hashing
- Rehashing
- Load Factor
- Directed Graphs
- Weighted Graphs
- Adjacency Lists
- Graph Traversal
- Recursive Path Finding

## Academic Context

These projects were developed as part of:

**CNG315 – Algorithms**

at **METU NCC Computer Engineering**.

The repository is intended to demonstrate practical implementations of fundamental data structures and algorithms in C.

## Author

**Furkan Sağlam**

## Keywords

`C` `Algorithms` `Data Structures` `Hash Table` `Graph` `Adjacency List` `Dynamic Memory` `Pointers` `Rehashing` `Path Finding`
