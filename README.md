# parallel-two-way-counter
Reading lines as an input from a text file (thread 1), adding each line alternatively to two queues (thread 1), getting count of vowel-consonant from each line in two queues (thread 2, thread 3), storing results of count from two queues in the output queue (thread 2, thread 3), and adding results of vowel-consonant count from the output queue (thread 4).

Most of the files in this repo have been borrowed from https://github.com/ourarash/multithreading_cpp
    My coded file for parallel-two-way-counter is located in src/main/two_way_counter.cc 

## Installation

```bash
git clone https://github.com/RudrenduMahindar/parallel_two_way_counter.git
```

## Running examples:

You can run this either using `bazel` OR just plain `g++`.

For `bazel`:

```bash
bazel run src/main:two_way_counter
```

For `g++`:
```bash
g++ -std=c++17 -pthread src/main/two_way_counter.cc  -I ./
./a.out
```

## Bazel Installation and Visual Studio Setup
For instructions on installing Bazel and setting it up in Visual Studio Code see [here](https://github.com/ourarash/cpp-template)
