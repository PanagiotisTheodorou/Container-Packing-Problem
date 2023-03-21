# Container-Packing-Problem
This is a project regarding an assignment, it needs to place boxes in a container

## Overview

The Container Packing Problem (CPP) falls into the category of optimization problems and is a problem like the 2D Bin Packing Algorithm and the Knapsack Problem, that functions with a stack to utilize backtracking. The goal of this problem is to pack a number of boxes into a container without leaving any box behind and without any of the boxes overlapping the container edges. Real world uses are often seen in transportation of heavy or important goods. Most CPPs are done in a 2d or 3d environment and use different shapes and weights.

## To run

Copy the code onto your local IDE and change the example file from the main.
```
PackerProblem* p = loadPackerProblem("example1.txt");
```
If you want to include sorting you need to uncomment it in the loadPackerProblem function:
```
PackerProblem* loadPackerProblem(string filename)
```