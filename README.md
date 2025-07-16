<div align="center">

<h1>🚚🗺️ TSP Branch & Bound — Optimal Route Solver</h1>

<p>A High-Performance C++ Toolkit that finds optimal tours for classic <a href="https://en.wikipedia.org/wiki/Travelling_salesman_problem" target="_blank">Travelling Salesman Problem (TSP)</a> instances.</br>
The project contains parallelized <a href="https://en.wikipedia.org/wiki/Branch_and_bound" target="_blank">Branch-And-Bound (B&B)</a> implementations,
tested on <a href="https://www.grid5000.fr/w/Grid5000:Home" target="_blank">Grid5000's</a> <b><i>multi-core</b></i> and <b><i>multi-node</b></i> architectures, as well as parallelized implementations of <a href="https://www.math.uwaterloo.ca/tsp/concorde.html" target="_blank">Concorde TSP Solver</a>.</p>

<div>
  <img src="https://img.shields.io/badge/Solo-Project-gray?logo=codecrafters&labelColor=cyan&logoColor=%23323232" style="height: 30px; width: auto;">
</div>

</div>

---

## 🎯 Goals
This toolkit aims to find exact solutions to [tsplib](https://github.com/mastqe/tsplib) TSPs for an undirected graph with symmetric edge costs in three ways:
- Parallelizing Concorde on a multi-node architecture
- Implementing an [OpenMP](https://www.openmp.org/) suitable Branch and Bound approach for multi-core architectures.
- Swapping the default single-threaded LP solver that Concorde uses ([QSOPT](https://www.math.uwaterloo.ca/~bico/qsopt/)) with the multi-threaded [CPLEX](https://www.ibm.com/products/ilog-cplex-optimization-studio)
- Benchmark and record solving times of different implementations in various scenarios

---

## 🗂️ Custom Implementations
- **BaseRec.cpp** – Base recursive implementation of Branch and Bound algorithm
- **ImprovedRec.cpp** – Implemented ***Bitmasking*** and a more suitable ***lower bound***
- **BaseOpenMP.cpp** – Achieved multiple core ***parallelization via OpenMP***
- **run_parallel.sh** – ***Parralelizes Concorde*** on multiple available nodes with different seeds and then lists each node's performance time
- **run_parallel_best.sh** – Also fires up multiple Concorde runs, but ***frees resources*** immediately after one node finishes (the fastest one)

---

## 🏗️ Tech Stack

| Layer                | Technology / Language |
|----------------------|------------|
| Core Algorithm       | C++ |
| External TSP Solver  | [Concorde](https://www.math.uwaterloo.ca/tsp/concorde.html) |
| LP Solver            | [QSOPT](https://www.math.uwaterloo.ca/~bico/qsopt/), [CPLEX](https://www.ibm.com/products/ilog-cplex-optimization-studio) |
| Parallel Execution   | Bash, SSH, [OpenMP](https://www.openmp.org/) |
| Problem Dataset      | [TSPLIB](https://github.com/mastqe/tsplib) (`*.tsp`) |
| Computational Testbed| [Grid5000](https://www.grid5000.fr/w/Grid5000:Home) |

---

## 🎬 Showcase
For detailed test run results, visualizations, and in-depth information about the problem and implemented solutions, please visit:

<a href="https://shorturl.at/mUccA" target="_blank">
    <img src="https://img.shields.io/badge/Presentation%20-%20red?logo=niconico" style="height: 35px; width: auto;">
</a>



