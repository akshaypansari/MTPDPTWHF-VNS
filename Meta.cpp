#include <iostream>
#include <algorithm>

#include "Meta.h"
#include "Problem.h"
#include "Solution.h"
#include "Operator.h"

using std::cout;
using std::endl;

VNS::VNS(){};
VNS::~VNS(){};
void VNS::Perturb(Solution &solution,const Problem &p, const int para)
{
    for (int i=0;i<para;i++)
    {
        removeCustomer(solution, p);
    }
    cout<<"SolutionDisplayed before adding the customers using Regret Insertion"<<endl;
    solution.displaySolution();
    cout<<"servedSingleTrips="<<solution.servedSingleTrips.size()<<"unservedSingleTrips="<<solution.unservedSingleTrips.size()<<"servedLunchTrips="<<solution.servedLunchTrips.size()<<"unservedLunchTrips="<<solution.unservedLunchTrips.size()<<endl;
    // std::sort(solution.unrouted_cust_request_id.begin(),solution.unrouted_cust_request_id.end(),[solution,p ](int a, int b)->bool {
    //     LoadRequest R1=p.requests[a];
    //     LoadRequest R2=p.requests[b];

    //     if(std::min(R1.pickup.end_time-R1.pickup.start_time,R1.delivery.end_time-R1.delivery.start_time)<
    //        std::min(R2.pickup.end_time-R2.pickup.start_time,R2.delivery.end_time-R2.delivery.start_time))
    //     // if(solution.GlobalTrips[a].depot_early_start_time<S.GlobalTrips[b].depot_early_start_time)
    //         return true;
    //     return false;
        
    // });
    // cout<<solution.unrouted_cust_request_id<<endl;//display indices

    AddCustomers(p,solution);
    



    // getchar();
}
