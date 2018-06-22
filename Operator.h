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

//SELECT A RANDOM NUMBER
//
#endif