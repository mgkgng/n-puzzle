# N-Puzzle 🧩

> **“Who in the world am I? Ah, that's the great puzzle.”**
> ― Lewis Carroll, Alice in Wonderland

![preview](/assets/npuzzle.gif)


## Project Overview 🔎
### Objectifs
- Implementing the A* and IDA* algorithm with various heuristics or searching strategies to solve `n x n` N-Puzzles.
- Benchmarking the performance of different algorithms and heuristics to refine our approach
- Generating puzzles randomly and verifying their solvability

### Achievements
- Understanding and implementation of various algorithms, heuristics and searching strategies.
    - A* and Iterative Deepening A* algorithms
    - Manhanttan, Hamming, Euclidian and Linear Conflict heuristics
    - Uniform-Cost and Greedy Searches
- Enhanced the performance by fine-tuning the data structures (e.g., vector hashing).
- Crafted an intuitive and creative web interface which encompasses the project's entire process.
- Explored the use of WebAssembly to run the C++ program in the Svelte app.

## Getting started 🦾

### Prerequisites
1. **Emscripten SDK**: Required for compilling C++ into WebAssembly. Follow the installation guide on the [Emscripten documentation page](https://emscripten.org/docs/getting_started/downloads.html). 
2. **Node.js**: Necessary for the web environment. Download and install from Node.js [official website](https://nodejs.org/en/download).

### Installation
1. **Compile C++ Program**: Transform the C++ logic into WebAssembly. Navigate to the project's directory and execute:
```
cd n_puzzle
make
```
2. **Set Up Web Interface**: Install dependencies and initialize the server.
```
cd .. // return to the project root
npm install // install node dependencies
npm run dev // launch the server in development mode
```
3. **Ready to go**: Open your browser and navigate to `localhost:8080`!

### Usage
You can either generate a random puzzle or load pre-formatted puzzle files to run the n-puzzle solver.
- **Generate Option**: Instantly create a random, solvable 3x3 puzzle.
- **Load Option**: Utilize the `npuzzle-gen.py` script to generate custom puzzles.
```
# create a 4x4 solvable puzzle into puzzle.txt file
python3 npuzzle-gen.py -s 4 > puzzle.txt

# view more script options
python3 npuzzle-gen.py -h
```
