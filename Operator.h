#ifndef OPERATOR_H_
#define OPERATOR_H_
#include "Solution.h"
#include "Problem.h"
#include <algorithm>


template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v);
bool MultiTripFeasiblity(VehicleTrips& VT,const Problem& p, Solution& S);//this contains total trip of single variable
bool BestInsertPlace(int request_index,const Problem& p, Solution& S, double cost,std::vector<bool>& bookkeep);
void RemoveRequest(const Problem& p, Solution& S);
void Erase_ID_from_Trip(std::vector<SingleTrip>::iterator temp_strip,const int remove_id_location,Solution& solution,const Problem& p);
bool bool_Check_Feasibility(int vehiclenum,int customer_id, const Problem& p, Solution& solution);
bool bool_insert_customer(SingleTrip& temps1,SingleTrip& temps2, int customer_id1,int customer_id2,const  Problem& p,Solution& solution,  double cost);
void removeCustomer(Solution &solution, const Problem& p);
void AddCustomers(const Problem& p, Solution& solution);
void InsertRequest(int tripindex,int pindex,int dindex,
                    const LoadRequest& tempreq,const Problem& p,Solution& S);
void Edited_createSingleTrip(const LoadRequest& req,const Problem& p, Solution& S/*, int pushback_id*/);



//SELECT A RANDOM NUMBER
//
#endif