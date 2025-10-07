Floyd-Warshall all-pairs shortest paths

This small program computes all-pairs shortest paths using the Floyd–Warshall algorithm.

Compile (Windows PowerShell):

    g++ -std=c++17 -O2 main.cpp -o floyd

Run with an input file or pipe input:

    .\floyd < input.txt

Input format:
    n m
    u v w   (m lines)

Example input (4 nodes, 5 directed edges):

    4 5
    1 2 3
    2 3 4
    1 3 10
    3 4 2
    4 2 -6

Expected output: 4x4 matrix of shortest distances (INF means no path).
