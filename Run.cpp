#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "Run.h"
#include "Problem.h"
#include "Solution.h"
#include "LoadRequest.h"
#include "InitialSolution.h"
#include "LocalSearch.h"
#include "Meta.h"

// 
//extern vector<SingleTrip> GlobalTrips;
bool compare_start_time(LoadRequest const& r1,LoadRequest const& r2)
{
    if(r1.pickup.start_time<r2.pickup.start_time) return true;
    if(r1.pickup.start_time==r2.pickup.start_time && r1.pickup.end_time<r2.pickup.end_time ) return true;
    if(r1.pickup.end_time==r2.pickup.end_time && r1.delivery.end_time<r2.delivery.end_time) return true;
    if(r1.delivery.end_time==r2.delivery.end_time && r1.delivery.start_time<r2.delivery.start_time) return true;
    return false;
}
Solution Initial_Solution(Problem &p);
Run :: Run()
{}

Run::~Run()
{
}

void Run::RunMTPDPTWHF(std::string filename, int seed)
{
    std::string fname = filename;
    std::cout <<"Instance: "<< fname <<std::endl;

    Solution Sbest;
    Sbest.total_solution_cost=1000000000;
    int SbestmtripsSize=10000;
    Problem p;
    // std::srand(1000);
    for(int t=0;t<2;t++)//edits//best till now is at t=10 qhwn seed was 1000
    {   
 
    p.LoadProblem(fname);
    p.p_seed=seed;
 
    InitialSolution I;

     cout<<"Run number === "<<t<<"--------------------------------------------------------------------"<<endl;
     cout<<"t value "<<t<<endl;
    
    Solution S_init;
    S_init=I.InitialSolve(p,S_init);
    S_init.Calculate_Solution_Cost(p);
    if(S_init.total_solution_cost==0)
    {
        cout<<"There is sthg wrong"<<endl;
        exit(0);
    }
    
    if(S_init.total_solution_cost<Sbest.total_solution_cost && S_init.MTrips.size()<=SbestmtripsSize)
        {
            Sbest=S_init;
            SbestmtripsSize=S_init.MTrips.size();
            //S_init.~Solution();
            cout<<"t value "<<t<<endl;
          }
    //cout<< "hi///// "<<Sbest.total_solution_cost<<endl;    
        
    }
    Sbest.displaySolution();
    cout<<"Total Cost"<<Sbest.total_solution_cost<<endl;
    cout<<"servedSingleTrips="<<Sbest.servedSingleTrips.size()<<"unservedSingleTrips="<<Sbest.unservedSingleTrips.size()<<"servedLunchTrips="<<Sbest.servedLunchTrips.size()<<"unservedLunchTrips="<<Sbest.unservedLunchTrips.size()<<endl;
    getchar();
    // system("PAUSE");
    LocalSearch LS;
    LS.LocalOpt(Sbest,p);
    Sbest.displaySolution();
    Sbest.Calculate_Solution_Cost(p);
    cout<<"servedSingleTrips="<<Sbest.servedSingleTrips.size()<<"unservedSingleTrips="<<Sbest.unservedSingleTrips.size()<<"servedLunchTrips="<<Sbest.servedLunchTrips.size()<<"unservedLunchTrips="<<Sbest.unservedLunchTrips.size()<<endl;
    cout<<"globaltrips size="<<Sbest.GlobalTrips.size()<<endl;
    // cout<<"second run----------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    getchar();

    // LS.LocalOpt(Sbest,p);
    // Sbest.displaySolution();
    // Sbest.Calculate_Solution_Cost(p);
    cout<<"solution= "<<endl;
    Sbest.Calculate_Solution_Cost(p);
    Solution SbestGlobal=Sbest;
    int iteration=0;
    int numbertobedeleted=5;
    std::vector<int> bestdeletednum;
    while(iteration<100&&numbertobedeleted<p.num_of_request/3)
    {
        cout<<"iteration"<<iteration<<endl;
    // Solution Stempbest=Sbest;
        VNS V1;
        V1.Perturb(Sbest,p,numbertobedeleted);
        // LocalSearch LS;
        LS.LocalOpt(Sbest,p);
        Sbest.displaySolution();
        Sbest.Calculate_Solution_Cost(p);
        if(Sbest.total_solution_cost<SbestGlobal.total_solution_cost)
        {
            bestdeletednum.push_back(numbertobedeleted);
            SbestGlobal=Sbest;
            numbertobedeleted=5;
            iteration=0;
            cout<<"got better solution"<<endl;
        }
        else
        {
            cout<<"else perturb"<<endl;
            Sbest=SbestGlobal;
            numbertobedeleted++;
            iteration++;
        }
        cout<<"servedSingleTrips="<<Sbest.servedSingleTrips.size()<<"unservedSingleTrips="<<Sbest.unservedSingleTrips.size()<<"servedLunchTrips="<<Sbest.servedLunchTrips.size()<<"unservedLunchTrips="<<Sbest.unservedLunchTrips.size()<<endl;
        cout<<"globaltrips size="<<Sbest.GlobalTrips.size()<<endl;
        // getchar();
    }
    cout<<"iteration == "<<iteration<<" numbertobedeleted=="<<numbertobedeleted<<endl;
    cout<<"best iteration numbers in the vector form"<<bestdeletednum<<"-----------------------------"<<endl;
    SbestGlobal.displaySolution();
    SbestGlobal.Calculate_Solution_Cost(p);

    


        

    
    // Problem p;
    // p.LoadProblem(fname);
    // p.p_seed=seed;
    // Solution S_init;
    // InitialSolution I;
    // S_init=I.InitialSolve(p,S_init);
    // S_init.Calculate_Solution_Cost(p);
    // cout<<"Total Cost"<<S_init.total_solution_cost<<endl; 
 
    // InitialSolution I;
    //Sbest.Calculate_Solution_Cost(p);
   // Sbest.displaySolution();
       

  /*  Solution S;
    S=Initial_Solution(p);*/

    


}

/*Solution Initial_Solution(Problem &p)
{
    Solution S;
    std::vector<int> unrouted;
    unrouted.resize(p.num_of_request);
    
    std::sort(p.requests.begin(),p.requests.end(),compare_start_time);

    for(unsigned int i=0;i<p.num_of_request;i++)
    { 
        unrouted[i]=p.requests[i].rid;//this will give the id which are there initially;
        std::cout<<unrouted[i]<<" ";//debug
    }
   // for(unsigned int i=0;i<p.requests.size();i++);
    int r=0;
    while(r<p.num_of_request);
    {
        
    //     int insert=0;
    //     if (!S.MTrips.empty())
    //     {
    //         std::vector<MultipleTrip>::iterator i1;
    //         for(i1=S.MTrips.begin();i1!=S.MTrips.end();i1++)
    //         {
    //             if(!i1->Trips.empty())//https://stackoverflow.com/questions/4775411/c-call-class-method-using-vector-iterator
    //             {
    //                 std::vector<SingleTrip>::iterator i2;
    //                 for(i2=i1->Trips.begin();i2!=i1->Trips.end();i2++)
    //                 {
    //                     //check if the request can be inserted into it or not;
    //                     if(i2->fix_trip!=0)
    //                     {

    //                         //try to insert the customer in the current position & and check if the trip can be fixed
    //                     }
    //                     else 
    //                     {
    //                         //create a new trip and update values
    //                         //insert it into the multitrip
    //                         //insert++
    //                     }

    //                     //if yes very good insert the request and update the value
    //                     //otherwise make a new trip in the same vehicle;//can also make a function for the same
    //                     //inserted then make insert=1;
    //                     //if new strip is created then then make the new trip and insert in the multitrip
    //                     //if the single trip can't be inserted into the trip then make a new multitrip
    //                 }

    //             }
    //             if(insert==0)
    //             {
    //                 MultipleTrip;
    //                 //create NewMultiTrip and insert function
    //                 //update the vehicle and insert==1
    //             }

    //         }
    //     LoadRequest Req= p.requests[r];
    //    // MultipleTrip Multi;
    //     SingleTrip strip;
    //     int vtype=p.checkcapacity(Req.pickup.demand);
    //     strip.cust_id.push_back(0);
    //     strip.cust_id.push_back(Req.pickup.id);
    //     strip.cust_id.push_back(Req.delivery.id);
    //     strip.cust_id.push_back(0);
    //     strip.depot_start_time=Req.pickup.start_time-p.distance[0][Req.pickup.id];
    //     strip.begin_time.push_back(strip.depot_start_time);
    //     strip.depart_time.push_back(strip.depot_start_time);
    //     strip.begin_time.push_back(max(Req.pickup.start_time,strip));
    //     }
    //     else
    //     {
    //         MultipleTrip temp_Mtrip;
    //         S.createMultipleTrip(temp_Mtrip,p,Req);
    //         S.MTrips.push_back(temp_Mtrip);
    //         //create NewMultitrip and insert function and update insert==1;
    //         //also update other variables
    //     }


        
        

       

    //     //S.MTrips.push_back(Multi);
    //     unrouted.erase(unrouted.begin()+r);//verify it at the end
    //     r++;
    }
}
*/