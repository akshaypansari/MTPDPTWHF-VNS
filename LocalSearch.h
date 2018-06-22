#ifndef LOCALSEARCH_H_
#define LOCALSEARCH_H_
#include "Solution.h"
#include "Problem.h"
#include "Operator.h"

class LocalSearch
{
    public:

        LocalSearch();
        ~LocalSearch();
        void LocalSearch_operator1(Solution& S,const Problem& p);
        void LocalOpt( Solution& S, const Problem& p);
        bool LocalSearch_operator2(Solution& solution,const Problem& p);//a customer relocation
        std::vector<bool> m_LS2_bookkeep;

    
};



#endif