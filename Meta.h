/*
Author:Akshay
Date:29.6.18
This file contain the metaheuristic VNS initially
*/

#ifndef META_H_
#define META_H_
#include "Solution.h"
#include "Problem.h"
#include "Operator.h"
class VNS
{
    public:

        VNS();
        ~VNS();
        void Perturb(Solution &solution,const Problem& p,const int para);
        
};
#endif 