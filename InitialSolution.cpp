//how check feasibility of single trips work;input-old route,new route positions*2,p,multitrip)
#include "Solution.h" 
#include "Problem.h"
#include "InitialSolution.h"
#include <algorithm>
#include <vector>
#include <iostream>

// template<typename T>
ostream& operator<< (ostream& out, const vector<SingleTrip> v) {
    out << "{";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) 
    {
        for(int j: v[i].cust_id)
            out << j<<" ";
        // for(int k: v[i].depot_early_start_time)
        //     out<<k<<" ";
        out << endl;
        out << v[i].depot_early_start_time << " " << v[i].depot_late_start_time << " " << v[i].trip_duration<<" "<<v[i].vehicletrip_id;        
        if (i != last) 
            out << ", ";
    }
    out << "}";
    cout<<endl;
    return out;
}
ostream& operator<< (ostream& out, const vector<VehicleTrips> v) {
    out << "{";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        for(int j: v[i].Multi)
            out << j<<" ";
        if (i != last) 
            out << ", ";
    }
    out << "}";
    cout<<endl;
    return out;
}

//typedef enum { false, true } bool;
//we can also use different integer to denote different infeasibility
//so that it will help in taking corrective action
//like we don't need to insert any lunch time inside
//also we don't need to 

//extern vector<SingleTrip> GlobalTrips;
bool ShrinkTrip_FeasiblityCheckingforSingleTrip(int tripindex,const int pindex,const int dindex,
                                                LoadRequest& tempreq, Problem& p,Solution& S);
void InsertRequest(int tripindex,int pindex,int dindex,
                    LoadRequest& tempreq, Problem& p,Solution& S);

// bool compareTripEarlyStartTime_duration(int a, int b)
// {
//     if(GlobalTrips[a].depot_early_start_time<GlobalTrips[b].depot_early_start_time)
//         return true;
//     else if(GlobalTrips[a].depot_early_start_time==GlobalTrips[b].depot_early_start_time
//             &&  GlobalTrips[a].trip_duration<GlobalTrips[b].trip_duration)
//         return true;
//     return false;
// }

bool MultiTripFeasiblity(VehicleTrips& VT,Problem &p, Solution& S);
void createSingleTrip(LoadRequest& req, Problem& p, Solution& S);
void ShrinkTrip(SingleTrip& stemp,Problem& p, Solution& S);

using namespace std;
InitialSolution::InitialSolution()
{}

InitialSolution::~InitialSolution()
{}

Solution InitialSolution::InitialSolve(Problem& p, Solution& S)
{
//    vector<SingleTrip> temptrip;
    vector<int> temp_unrouted_custumer;
    temp_unrouted_custumer.resize(p.num_of_request);
    //assign unrouted customer to temp_unrouted_customer
    int j;
    for(unsigned i=0;i< p.sorted_request_id.size();i++)
    {
        temp_unrouted_custumer[i]=p.sorted_request_id[i];
        cout<<p.sorted_request_id[i]<<" ";
    }
    //cout<<"temp_unrouted_and_sorted_custumer assigned"<<endl;
    LoadRequest tempreq=p.requests[temp_unrouted_custumer[0]-1];//-1 done to get index
    //cout<<"Now Id is"<<tempreq.rid<<endl;
    //getting the vehicle type for first request
    VehicleTrips M1;
    M1.TripVehicle=p.get_vehicle(tempreq.pickup);
    //inserting location of trips in Global Trips in Multi vector;
    M1.Multi.push_back(0);
    M1.Multi.push_back(1);
    //now adding the vector in single trip
    SingleTrip strip;
//depot id entered
    strip.cust_id.push_back(p.depot.id);
    // strip.cust_id.push_back(tempreq.pickup.id);
    // strip.cust_id.push_back(tempreq.delivery.id);
    strip.cust_id.push_back(p.returndepot.id);
    strip.vehicletrip_id=0;//vehicle trip start from id 1
    S.GlobalTrips.push_back(strip);

    //assigning lunch trip to the vector
    SingleTrip lunchtrip;
    lunchtrip.depot_early_start_time=M1.TripVehicle.lunch_start_time;
    lunchtrip.depot_late_start_time=M1.TripVehicle.lunch_end_time;
    lunchtrip.trip_duration=M1.TripVehicle.lunch_duration;
    lunchtrip.islunchtrip=true;
    lunchtrip.vehicletrip_id=0;
    S.GlobalTrips.push_back(lunchtrip);

    S.MTrips.push_back(M1);//S.Mtrips[GlobalTrips[i].vehicletrip_id];
    bool isfeasible=ShrinkTrip_FeasiblityCheckingforSingleTrip(0,1,2,tempreq,p,S);
    
    if(isfeasible)
    {
        //cout<<"Isfeasible"<<"we are inside first customer insertion"<<endl;
        InsertRequest(0,1,2,tempreq,p,S);//inside insert request we also need to calculate the cost
    //-------------------- to complete the Insert Request
        temp_unrouted_custumer.erase(temp_unrouted_custumer.begin());
       // cout<<"first assigned customer deleted"<<endl;        
       // cout<<"temp_unrouted_custumer[0]"<<temp_unrouted_custumer[0]<<endl<<endl<<endl;
    }

    cout<<"loop began main******"<<endl;
    int nooftimesreloopdone=0;
    int checkingforerr=0;

    while(!temp_unrouted_custumer.empty())
    {
        bool isinserted=false;
        LoadRequest tempreq=p.requests[temp_unrouted_custumer[0]-1];//
       // cout<<"Now Id is"<<tempreq.rid<<endl<<endl<<endl<<endl;
        for(auto i=0;i<S.GlobalTrips.size();i++)//make a bettr iterator based on some information like multi trip etc.
        {
            //cout<<"second loop"<<endl;
            if(S.GlobalTrips[i].islunchtrip!=1)//to check using a variable;
            //define new variable;
            {
                int reloop=0;//to continue this loop to next customer id if the route is not feasible for a customer
                int GlobalTripSize=S.GlobalTrips[i].cust_id.size();
                
                SingleTrip& temps= S.GlobalTrips[i];//is this correct
                for(auto m=1;m<GlobalTripSize-1;m++)
                {
                    if( p.request_feasibility[p.getRequestID(tempreq.pickup.id)][p.getRequestID(temps.cust_id[m])]==0)
                    {
                        reloop=1;
                        break;
                    }     

                }
                if(reloop==1)
                {
                    nooftimesreloopdone++;
                    continue;
                }
                

                    //    cout<<"S.GlobalTrips[i].cust_id.size()===========  "<<S.GlobalTrips[i].cust_id.size()<<endl;               
                //int index_i=-1,index_j=-1;
               // cout<<endl<<S.GlobalTripsize<<endl;
                for(auto j=1;j<GlobalTripSize;j++)//first index
                {
                    for(auto k=j+1;k<GlobalTripSize+1;k++)//second index
                    {
                      //  cout<<"GlobalTrip index= "<<i<<"pindex= "<<j<<" dindex="<<k<<"----------------------------------------------------------"<<endl;
                        bool feas =ShrinkTrip_FeasiblityCheckingforSingleTrip(i,j,k,tempreq,p,S);
                        if(feas)
                        {
                            InsertRequest(i ,j,k,tempreq,p,S);//inside insert request we also need to calculate the cost
                        //-------------------- to complete the Insert Request
                            temp_unrouted_custumer.erase(temp_unrouted_custumer.begin());
                            //alt+shift change the language
                           // cout<<"Request inserted= "<<tempreq.rid<<endl<<endl<<endl;
                            isinserted=true;
                            break;
                        }
                    }
                    if(isinserted)
                    {
                        break;
                    }
                }
                if(isinserted)
                {
                    break;
                }
            }
            if(isinserted)
            {
                break;
            }
        }
        if(!isinserted)
        {
            cout<<"requested couldn't be inserted , now making a new trip for the request"<<tempreq.rid<<"\n\n";
            createSingleTrip(tempreq,p,S);
            temp_unrouted_custumer.erase(temp_unrouted_custumer.begin());
            isinserted=true;

            //cases if route is created and added to the multitrip and 
            //first make anew trip and try to  add it to exissting vehicletrips
            //if okay then it is inserted
            //other wise make a new multi trip just as done above //also add lunch service time to trips
            //erase the request
            //if everuthhing done then find the total cost and display solution
            //find operators then
        }
    }
    //cout<<"bi"<<endl;
    cout<<S.GlobalTrips<<endl;
    cout<<S.MTrips<<endl;
    cout<<nooftimesreloopdone<<"###############################                           &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
    return  S;
}

//v.erase(std::remove_if(v.begin(), v.end(),[](const int& x) {  return x > 10; }), v.end());// put your condition here

bool ShrinkTrip_FeasiblityCheckingforSingleTrip(int tripindex,const int pindex,const int dindex,
                                                LoadRequest& tempreq, Problem& p,Solution& S)
{
    //cout<<"MainShrinkTrip"<<endl;
    SingleTrip& temps= S.GlobalTrips[tripindex];//is this correct
   // cout<<temps.islunchtrip<<" "<<temps.cust_id[0];
    temps.cust_id.insert(temps.cust_id.begin()+pindex,tempreq.pickup.id);//.insert(temps.cust_id.begin()+pindex);
    temps.cust_id.insert(temps.cust_id.begin()+dindex,tempreq.delivery.id);//
    //cout<<"request inserted for checking"<<endl;
    //see the structure of entering the number in the vector;
    double early_start=p.depot.start_time ,late_start=p.depot.end_time,duration=0,capacity=0;
  //  bool feasible = true;
    for (auto i=1;i<temps.cust_id.size();i++)//checking and feasibility and finding the time window
    {
        Node N0=p.getNode(temps.cust_id[i-1]);//NO=
        Node Ni=p.getNode(temps.cust_id[i]);
        //cout<<"N0.id"<<N0.id<<"Ni.id"<<Ni.id<<endl;
        double s0=N0.service_time;
        double d01=p.distance[temps.cust_id[i-1]][temps.cust_id[i]];
        //cout<<"d(i)(i-1)"<<d01<<"s(i-1)"<<s0<<endl;
        double tempweight=Ni.demand;
        //cout<<"tempweight"<<tempweight<<endl<<endl;
        double ei=Ni.start_time;
        double li=Ni.end_time;
        // store service time and distancehere and add them;
        // also find li and ei and ci and typecons-i
        early_start=early_start+d01+s0;
        late_start=late_start+d01+s0;
        duration=duration+d01+s0;
        if(duration>p.max_travel_time)//to complete
        {
            temps.cust_id.erase(temps.cust_id.begin()+dindex);
            temps.cust_id.erase(temps.cust_id.begin()+pindex);
            return false;
        }
        //cout<<"early_start "<<early_start<<" late_start "<<late_start<<" duration"<<duration<<endl;
       // cout<<" li "<<li<<endl;
       // capacity+=tempweight;declared below
//--------------------------------checking for time window
        if(early_start>li)
        {

           // cout<<"early_start>li"<<endl;            
            temps.cust_id.erase(temps.cust_id.begin()+dindex);
            temps.cust_id.erase(temps.cust_id.begin()+pindex);
            return false;

        }
        else if (late_start<ei)
        {
           // cout<<"late_start<ei"<<endl;
            duration=duration+(ei-early_start);
            early_start=ei;late_start=ei;
        }
        else
        {
           // cout<<"3rd case"<<endl;            
            early_start=max(early_start,ei);
            late_start=min(late_start,li);
            //cout<<"after 3rd case early_start "<<early_start<<" late_start "<<late_start<<" duration"<<duration<<endl;
            
        }
//--------------------------------capacity constraint checking
        //temps.weight_to_carry=max(capacity,capacity+tempweight);
        capacity+=tempweight;
        //cout<<"capacity"<<capacity<<"S.MTrips[temps.vehicletrip_id].TripVehicle.Capacity"<<S.MTrips[temps.vehicletrip_id].TripVehicle.Capacity<<endl;
        if(capacity>S.MTrips[temps.vehicletrip_id].TripVehicle.Capacity) //if current cap is more than cehicle capacity then reurn false
        {
            //cout<<"temps.vehicletrip_id"<<temps.vehicletrip_id<<"##############################################"<<endl;
            
            //cout<<"capacity overflow"<<endl;
            temps.cust_id.erase(temps.cust_id.begin()+dindex);
            temps.cust_id.erase(temps.cust_id.begin()+pindex);
            return false;
        }
        // if(feasible)//checking for multitrip constraint
        // {
        //  //   feasible=MultiTripFeasibility(multitrip(vector of singletrip), vehicleno, );
        // }
        //check capacity constraint everytime, check duration contraint everytime; check time wndow constraint
        //can we use static here to define a fix quantity;
        //cout<<"i am in shrinktripbigger version inside the loop"<<endl;
        
    }
    //if capacity and time window is feasible then temporarily change the temps and check for multitripfeasibility
    //cout<<"p.max_travel_time"<<p.max_travel_time<< endl;
    if(duration>p.max_travel_time)//to complete
    {
        temps.cust_id.erase(temps.cust_id.begin()+dindex);
        temps.cust_id.erase(temps.cust_id.begin()+pindex);
        return false;
    }
    else
    {
        double olddepotstarttime=temps.depot_early_start_time;        
        temps.depot_early_start_time=early_start-duration;
        double olddepotendtime=temps.depot_late_start_time;                
        temps.depot_late_start_time=late_start-duration;
        double oldduration=temps.trip_duration;
        temps.trip_duration=duration;
        //cout<<"\nduration ---------------------"<<duration<<endl;
        //requirement of vehicletripID before hand so have to modify the code accordingly 
        bool feasmulti=MultiTripFeasiblity(S.MTrips[temps.vehicletrip_id],p,S);
           
        temps.trip_duration=oldduration;            
        temps.depot_early_start_time=olddepotstarttime;
        temps.depot_late_start_time=olddepotendtime;
        temps.cust_id.erase(temps.cust_id.begin()+dindex);
        temps.cust_id.erase(temps.cust_id.begin()+pindex);
       // cout<<tripindex<<" "<<pindex<<" "<<dindex<<" "<<"multitrip insertion feasibility= "<<int(feasmulti)<<endl;
        return feasmulti;
    }

    //feasible is true then check the feasibility of multitrip
}
void InsertRequest(int tripindex,int pindex,int dindex,
                    LoadRequest& tempreq, Problem& p,Solution& S)
{
    SingleTrip& temps= S.GlobalTrips[tripindex];
    temps.cust_id.insert(temps.cust_id.begin()+pindex,tempreq.pickup.id);//.insert(temps.cust_id.begin()+pindex);
    temps.cust_id.insert(temps.cust_id.begin()+dindex,tempreq.delivery.id);//
    //cout<<"request inserted permanently"<<endl;
    double early_start=p.depot.start_time ,late_start=p.depot.end_time,duration=0,capacity=0;
    //to edit and make it better
    bool feasible = true;
    for (auto i=1;i<S.GlobalTrips[tripindex].cust_id.size();i++)//checking and feasibility and finding the time window
    {
        Node N0=p.getNode(temps.cust_id[i-1]);//NO=
        Node Ni=p.getNode(temps.cust_id[i]);
        double s0=N0.service_time;
        double d01=p.distance[temps.cust_id[i-1]][temps.cust_id[i]];
        double tempweight=Ni.demand;
        double ei=Ni.start_time;
        double li=Ni.end_time;
        // store service time and distancehere and add them;
        // also find li and ei and ci and typecons-i
        early_start+=d01+s0;
        late_start+=d01+s0;
        duration+=d01+s0;
        capacity+=tempweight;
//--------------------------------checking for time window
        if(early_start>li)
        {
            cout<<"Error in entering request : start time not matching "<<endl<<endl;
        }
        else if (late_start<ei)
        {
            duration=duration+(ei-early_start);
            early_start=ei;late_start=ei;
        }
        else
        {
            early_start=max(early_start,ei);
            late_start=min(late_start,li);
        }   
    }
    temps.depot_early_start_time=early_start-duration;
    temps.depot_late_start_time=late_start-duration;
    temps.trip_duration=duration;
    //-----------------------------------to modify below
    //temps.vehicletrip_id=S.GlobalTrips[tripindex].vehicletrip_id;//to modify
  //     UpdateMultipleTrip(S.MTrips[temps.vehicletrip_id],p);
    
}

//----------------------Multitrip Feasibility checking
bool MultiTripFeasiblity(VehicleTrips& VT,Problem &p, Solution& S)//this contains total trip of single variable
{
  //  cout<<"Yout are now in MultiTripFeasiblity"<<endl;
    // sort multitrip on the basis of trip starttime window and then on duration;
    //cout<<"old VT.multi.size()"<< VT.Multi.size()<<endl;   
    //std::sort(VT.Multi.begin(),VT.Multi.end(), compareTripEarlyStartTime_duration);

        std::sort(VT.Multi.begin(),VT.Multi.end(),[S](int a, int b)->bool {
            // Lambda function
         //   s->GlobalTrips
        if(S.GlobalTrips[a].depot_early_start_time<S.GlobalTrips[b].depot_early_start_time)
            return true;
        else if(S.GlobalTrips[a].depot_early_start_time==S.GlobalTrips[b].depot_early_start_time
                &&  S.GlobalTrips[a].trip_duration<S.GlobalTrips[b].trip_duration)
            return true;
        return false;
        });



    //cout<<"newAfter sorting VT.multi.size()"<< VT.Multi.size()<<endl;   

   // cout<<"S.GlobalTrips.size()"<<S.GlobalTrips.size()<<endl;
    double temp;
    temp= S.GlobalTrips[ VT.Multi[0]].depot_early_start_time;
   // cout<<"GlobalTrips[ VT.Multi[0]].depot_early_start_time "<<S.GlobalTrips[ VT.Multi[0]].depot_early_start_time<<endl;
    for(auto it=VT.Multi.begin()+1;it!=VT.Multi.end();it++)
    {
        //cout<<"new loop ="<<*it<<endl;
        //cout<<"GlobalTrips[*(it-1)].trip_duration"<<S.GlobalTrips[*(it-1)].trip_duration<<endl;
        //cout<<"oldtemp"<<temp<<endl;
        temp=max(temp+S.GlobalTrips[*(it-1)].trip_duration+VT.TripVehicle.lunch_duration*!(S.GlobalTrips[*(it-1)].islunchtrip),
                            S.GlobalTrips[*it].depot_early_start_time );
       // cout<<"newtemp"<<temp<<endl<<"GlobalTrips[*it].depot_late_start_time"<<S.GlobalTrips[*it].depot_late_start_time<<endl;
        
        if( temp>S.GlobalTrips[*it].depot_late_start_time)
        {
            return false;
        }

    }
    return true;

}
//-------------Creating a new SingleTrip//It is fixed
void createSingleTrip(LoadRequest& req, Problem& p, Solution& S)
{
    SingleTrip stemp;//to insert it in the GlobalTrips
    stemp.cust_id.push_back(p.depot.id);
    stemp.cust_id.push_back(req.pickup.id);
    stemp.cust_id.push_back(req.delivery.id);
    stemp.cust_id.push_back(p.returndepot.id);
    stemp.islunchtrip=false;
    int needed_vehicle_type=p.getVehicleID(req);//return type of vehicle
    ShrinkTrip(stemp,p,S);
    S.GlobalTrips.push_back(stemp);
    //cout<<"You are in createSingleTrip"<<endl;
    //getvehicleID gives the least cost vehicle number for the vehicle
    //try to insert the singletrip in each of the multitrip and get a good solution
    int VTid=-1;
    for(auto it=S.MTrips.begin();it!=S.MTrips.end();it++)
    {
       // cout<<"you are checking old multrips to assign customers"<<endl;
        VTid++;
        //cout<<"VTid"<<VTid<<endl;
       // cout<<"it->TripVehicle.type = "<<it->TripVehicle.type<<" needed_vehicle_type = "<<needed_vehicle_type<<endl;
        //assign it to any vehicle already present  but that should less than type_cons
        if(it->TripVehicle.type>=needed_vehicle_type&&it->TripVehicle.type< req.pickup.type_const)
        {
            //inserting only if vehicle type matches
            int tripAddedNumber=S.GlobalTrips.size()-1;
            //cout<<"tripAddedNumber=GlobalTrips.size()-1 = "<<tripAddedNumber<<endl;
            //Try to insert in the current multitrip
            it->Multi.push_back(tripAddedNumber);//changes done here
            bool multifeas=MultiTripFeasiblity(*it,p,S);//sending the vehicletrip object to check for feasibility of multitrip
            if(multifeas)
            {
                //keep the singletrip inside this vehicletrip
                S.GlobalTrips.back().vehicletrip_id=VTid;
                return;

            }
            //else erase the trip from inside the Multi
            std::vector<int>& vec = it->Multi; // use shorter name
            vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
        }
       // cout<<"VTid2    ====   "<<VTid<<endl;
    }
   // cout<<"The request can't be assigned to already present vehicle so new MultiTrip is being created"<<endl;
    S.GlobalTrips.back().vehicletrip_id=S.MTrips.size();
    VehicleTrips VT;
    VT.TripVehicle=p.vehicles[needed_vehicle_type-1];
    //GlobalTrips.size() gives the total element and '-1' give the index of the last element entered
    VT.Multi.push_back(S.GlobalTrips.size()-1);//to get last element entered index
    
    SingleTrip lunchtrip;
    lunchtrip.depot_early_start_time=VT.TripVehicle.lunch_start_time;
    lunchtrip.depot_late_start_time=VT.TripVehicle.lunch_end_time;
    lunchtrip.trip_duration=VT.TripVehicle.lunch_duration;
    lunchtrip.islunchtrip=true;
    lunchtrip.vehicletrip_id=S.MTrips.size();
    S.GlobalTrips.push_back(lunchtrip);
    VT.Multi.push_back(S.GlobalTrips.size()-1);
    //cout<<"S.GlobalTrips.size()-1"<<S.GlobalTrips.size()-1<<endl;
    S.MTrips.push_back(VT);
    int feas=MultiTripFeasiblity(S.MTrips.back(),p,S);
    //cout<<S.MTrips.size()<<endl;
    if(feas)
    {
        cout<<S.MTrips.size()<<endl;
       // cout<<"new vehicle trip is assigned and route created"<<endl;
       // cout<<"bi"<<endl;
        // cout<<S.GlobalTrips<<endl;
        // cout<<S.MTrips<<endl;
        //cout<<"bi"<<endl;
    }
    else
    {
        cout<<S.GlobalTrips<<endl;
        cout<<S.MTrips<<endl;
        S.displaySolution();
        cout<<"this customer can't be allocated"<<req.rid<< endl;
        exit(0);
    }
}
void ShrinkTrip(SingleTrip& stemp,Problem& p, Solution& S)
{
    double early_start=p.depot.start_time ,late_start=p.depot.end_time,duration=0,capacity=0;
    for (auto i=1;i<stemp.cust_id.size();i++)//checking and feasibility and finding the time window
    {
        Node N0=p.getNode(stemp.cust_id[i-1]);//NO=
        Node Ni=p.getNode(stemp.cust_id[i]);
        double s0=N0.service_time;
        double d01=p.distance[stemp.cust_id[i-1]][stemp.cust_id[i]];
        double tempweight=Ni.demand;
        double ei=Ni.start_time;
        double li=Ni.end_time;
        // store service time and distancehere and add them;
        // also find li and ei and ci and typecons-i
        early_start+=d01+s0;
        late_start+=d01+s0;
        duration+=d01+s0;
       // capacity+=tempweight;declared below
//--------------------------------checking for time window
        if(early_start>li)
        {
            cout<<"Infeasible new SingleTrip due to time window in ShrinkTrip: Request  id is ="<<N0.id<<endl;
            
        }
        else if (late_start<ei)
        {
            duration=duration+(ei-early_start);
            early_start=ei;late_start=ei;
        }
        else
        {
            early_start=max(early_start,ei);
            late_start=min(late_start,li);
        }
        capacity+=tempweight;
    }
    if(duration>p.max_travel_time)//to complete
    {
        cout<<"Infeasible new SingleTrip due to duration in ShrinkTrip: Request  id is ="<<p.getNode(stemp.cust_id[1]).id<<endl;
    }
    //update all the value of the stemp
    stemp.depot_early_start_time=early_start-duration;
    stemp.depot_late_start_time=late_start-duration;
    stemp.trip_duration=duration;
    cout<<"Shrink Trip duration"<<duration;
    //cout<<"hi"<<endl;
    cout<<"ID-----"<<p.getNode(stemp.cust_id[1]).id<<"updated"<<endl;
}
        //reduce the time windows
        //for (auto it = my_vector.rbegin(); it != my_vector.rend(); ++it)
//         for (vector<my_class>::reverse_iterator i = my_vector.rbegin(); 
//         i != my_vector.rend(); ++i ) { 
    //     for(auto it=my_vector.rbegin()-1; it != my_vector.rend(); ++it)
    //     {
    //         it.late_start_time=min(it.late_start_time, (it+1).late_start_time-it.duration-rest_time;

    //     }
    // }
    // else(
    //     //permute the combination
    // )
    // return feasible;
    //E(i)=max(E(i-1)+D(i-1)+delta,E(i))
    //L(i)=min::(L(i+1)-D(i)-delta,L(i))
    //


// //multitrip contain vectors of singletrip but we should always be dealing with vector of whole single trip
// //we should be able to delete singletrip from the multitrip vector and that should remain in the singletrip vector
// //we can sort the singletrip on the basis of different things;
// // https://stackoverflow.com/questions/31179517/c-same-object-in-two-different-vectors
// void UpdateMultipleTrip(VehicleTrips& VT,Problem &p)
// {
//     std::sort(VT.Multi.begin(),VT.Multi.end(),compareTripEarlyStartTime_duration);// not required just for safety
//     double temp;
//     temp= S.GlobalTrips[ VT.Multi[0]].depot_early_start_time;
//     for(auto it=VT.Multi.begin()+1;it!=VT.Multi.end();it++)
//     {
//         temp=max( VTstarttime+GlobalTrips[*(it-1)].trip_duration+VT.TripVehicle.lunch_duration*GlobalTrips[*(it-1)].islunchtrip,
//                             S.GlobalTrips[*it].lunch_start_time );
//         if( temp>GlobalTrips[*it].depot_late_start_time)
//         {
//             return false;
//         }
//     }
// } no need of it as of now

