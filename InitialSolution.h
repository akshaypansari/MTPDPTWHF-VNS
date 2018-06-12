/*
created by: Akshay Pansari
Date:7.8.18
//This finds initial solution using Sequential Insertion
*/
#ifndef INITIALSOLUTION_H_
#define INITIALSOLUTION_H_

#include <iostream>
#include "Solution.h"
#include "Problem.h"

class InitialSolution
{
    public:
    InitialSolution();
    ~InitialSolution();
    Solution InitialSolve(Problem& p, Solution& S);
};

#endif