# tiny_cpp_projects

A collection of small C++ projects and libraries designed for educational purposes. These implementations aim to provide valuable learning experiences and insights into various programming concepts and techniques.

## RDMA Communication Lib

> What You Can Learn

[repo](./projects/rdma_comm/)

- How to write an efficient RDMA server and client for high-performance communication.

- How to use CMake to build a large, well-structured C++ project that is easy to debug and maintain.

- How to implement some intresting utils
  - thread logger
  - connection pool
  - thread pool

```shell
cmake -B build -DCONDA_PREFIX="$CONDA_PREFIX" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

## WebServer

webserver is a high-performance server based on C++, built with CMake.

> What You Can Learn

[repo](./projects/webserver/)

- How to use smart pointer, like `unique_ptr`, etc

- How to use CMake to build whole project

- How to work with system calls like epoll to build high-performance C++ servers and implement callback logic

- How to use RAII to manage memory resource


## XV6 OS Kernel

A simplified implementation of a OS kernel based on RISC-V in C++.
(maybe we should use rust?)

> What You Can Learn

## MINI-LSM

A simplified implementation of a Log-Structured Merge Tree (LSM) storage engine in C++.

> What You Can Learn

- How LSM trees work as a fundamental data structure used in many modern databases like LevelDB, RocksDB, and Cassandra

- Core concepts of LSM including:
  - Memory tables (memtables) and sorted string tables (SSTables)
  - Compaction strategies and leveling
  - Write-ahead logging (WAL)
  - Bloom filters for efficient lookups

- Practical experience with:
  - File I/O and data serialization
  - Concurrent programming patterns
  - Memory management in C++
  - Performance optimization techniques

The implementation focuses on educational value while maintaining reasonable performance characteristics. It's a great way to understand the internals of modern storage engines.
