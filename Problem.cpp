#include "Problem.h"
#include "Vehicle.h"
#include <vector>
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
Problem::Problem()
{
    num_of_request=0, rest_time=0, lunch_duration=0, vehicle_type_number=0, max_travel_time=0;
    p_seed=0;
}
Problem::~Problem()
{
}

const Node& Problem::getNode(const int cid ) const
{
    if(cid<1)
        return depot;
    else if(cid<=num_of_request)
    {
        return requests[cid-1].pickup;
    }
    else if(cid<=2*num_of_request)
    {
        return requests[cid-num_of_request-1].delivery;
    }
    return returndepot;
}
int Problem::getRequestID(int cid) const//input->request Id and output the Node index
{
    cid--;
    if(cid>=num_of_request)
    {
        cid-=num_of_request;
    }
    return cid;
}
bool Problem::IsDeliveryNode(const int cid)const
{
    int requestid= getRequestID(cid);

    return requests[requestid].delivery.id==getNode(cid).id;
}

bool Problem::IsPickupNode(const int cid)const
{
    int requestid= getRequestID(cid);

    return requests[requestid].pickup.id==getNode(cid).id;

}

int Problem::checkcapacity(double demand, int typeconst )
{
    unsigned i=0;
    while(i!=vehicle_type_number)
    {
        if(demand<vehicles[i].Capacity&& typeconst!=vehicles[i].type)
        {
            return vehicles[i].type;
        }
        else i++;
    }
}
//vehicle type constraint
Vehicle Problem::get_vehicle(Node& tempNode)
{
 
    //int const_type=tempNode.type_const;
    for(int i=0;i<vehicle_type_number;i++)
    {
        if(vehicles[i].Capacity>=tempNode.demand)
        {
            if(tempNode.type_const<=vehicles[i].type)
            {
                cout<<"Error To get vehicle; vehicle mismatch"<<endl;
                exit(0);
            }
            else
            {
                return vehicles[i];
            }
        }
    }
}

int Problem::getVehicleID(LoadRequest& Req)
{
    for(int i=0 ;i<vehicle_type_number;i++)
    {
        cout<<"getvehicleID"<<i<<endl;
        cout<<"Req.pickup.type_const = "<<Req.pickup.type_const<<"vehicles[i].Capacity"<<vehicles[i].Capacity<<endl;
        if(vehicles[i].Capacity>=Req.pickup.demand)
        {
            if(Req.pickup.type_const<=vehicles[i].type)
            {
                cout<<"Error in assinging the Request ID"<<Req.rid<<endl;
                exit(0);


            }
            else if(Req.pickup.type_const>vehicles[i].type)
            {
                cout<<"returned type"<<vehicles[i].type<<endl;
                return vehicles[i].type;
            }
        }
    }
    cout<<"request cant be assigned a proper vehicle : Code in problem.cpp"<<endl;
}

void Problem::LoadProblem(const std::string &filename)
{
    instance_name=filename;

    // to complete according to the question
    std::string file_path= "MTPDPHFTW_dataset/"+filename+".txt";
 //   std::cout<<"hi"<<std::endl;
    std::ifstream fin(file_path.c_str());
    std::cout<<"hi"<<std::endl;
    std::string s_temp;

    getline(fin, s_temp);
    cout<<s_temp<<endl;//display instance

    getline(fin, s_temp);
       cout<<s_temp<<endl;//display empty line

    getline(fin, s_temp);
    std::cout<<s_temp<<std::endl;//display vehicle type header

    fin>>vehicle_type_number>>num_of_request>>max_travel_time>>rest_time;
    cout<<vehicle_type_number<<" "<<num_of_request<<" "<<max_travel_time<<" "<<rest_time<<endl;//about vehicle type
    getline(fin, s_temp);
    std::cout<<s_temp<<std::endl;
    getline(fin, s_temp);
    //std::cout<<s_temp<<std::endl;
    getline(fin, s_temp);
    std::cout<<s_temp<<std::endl;
    //std::cout<<"i am in problem2.cpp"<<" " <<vehicle_type_number<<std::endl;
    for(int i=0;i<vehicle_type_number;i++)
    {
        Vehicle v;
        std::cout<<i<<" ";
        fin>>v.type>>v.lunch_start_time>>v.lunch_end_time;
        fin>>v.lunch_duration>>v.fixed_cost;
        fin>>v.variable_cost>>v.Capacity;
        cout<<v.fixed_cost<<endl;
        vehicles.push_back(v);
    }
    std::cout<<"i am in problem3.cpp"<<"hi"<<std::endl;
    getline(fin, s_temp);
    getline(fin, s_temp);
    std::cout<<s_temp<<std::endl;
        getline(fin, s_temp);
    std::cout<<s_temp<<std::endl;
    fin>>depot.id>>depot.type_const>>depot.demand>>depot.start_time;
    fin>>depot.end_time>>depot.service_time;
    std::cout<<"i am in problem3.cpp"<<"hi"<<std::endl;
    for(int i=0;i<num_of_request;i++)
    {
        LoadRequest R;
        fin>>R.pickup.id>>R.pickup.type_const>>R.pickup.demand;
        fin>>R.pickup.start_time>>R.pickup.end_time>>R.pickup.service_time;
        cout<<R.pickup.id<<R.pickup.start_time<<endl;
        R.rid=i+1;//rid==pickup.id==n+delivery.id
        if(R.pickup.type_const==0){
            R.pickup.type_const=vehicle_type_number+1;
        }
        requests.push_back(R);

    }
    cout << requests.size() << endl;
    for(int i=0;i<num_of_request;i++)
    {
        fin>>requests[i].delivery.id>>requests[i].delivery.type_const>>requests[i].delivery.demand
            >>requests[i].delivery.start_time>>requests[i].delivery.end_time>>requests[i].delivery.service_time;
       // cout<<endl;
       // cout<<requests[i].delivery.id<<" "<<endl;
    }
//     for(auto R=requests.begin();R!=requests.end();R++)
//     {
//         /*std::ifstream::*/fin>>R->delivery.id>>R->delivery.type_const>>R->delivery.demand;
//         fin>>R->delivery.start_time>>R->delivery.end_time>>R->delivery.service_time;
//         // cout<<R->delivery.id<<" "<<R->delivery.type_const<<" "<<R->delivery.demand<<endl;
//         // cout<<R->delivery.start_time<<" "<<R->delivery.end_time<<" "<<R->delivery.service_time<<endl;
//         cout<<"ok"<<endl;
//   //      requests[i].rid=i+1;//not required here as already defined above
//     }

    getline(fin, s_temp);
    getline(fin, s_temp);
    getline(fin, s_temp);
    int total_nodes=2*num_of_request+1;
 //   distance.resize(total_nodes, std::vector<double>(total_nodes));
    distance.resize(total_nodes);
    for(unsigned int i = 0; i < total_nodes; ++i)
    {
	distance[i].resize( total_nodes );
    }
    for(int i=0;i<total_nodes;i++)
    {
        for(int j=0;j<total_nodes;j++)
        {
            fin>>distance[i][j];
        }
    }

    returndepot=depot;
//std::cout<<"i am in problem.cpp"<<depot.id<<std::endl;
    SortRequest();
    //cout<<"hi";

}

struct Minduration
{
    int id;
    double duration;
/*    bool operator <(const Minduration &x)const
	{
		return duration < x.duration;
	}*/
};

bool compare_duration(Minduration const& r1,Minduration const& r2)
{
    if(r1.duration<r2.duration)
        return true;
    if(r1.duration==r2.duration && r1.id<r2.id)
        return true;
    return false;
}
void Problem::SortRequest()
{
    vector<Minduration> Min(num_of_request);
    for(int i=0;i<num_of_request;i++)
    {
        //Minduration M;
        Min[i].id=requests[i].rid;//id has the customer real id number and not index
        Min[i].duration=std::min(requests[i].pickup.end_time-requests[i].pickup.start_time,requests[i].delivery.end_time-requests[i].delivery.start_time);
       // cout<<"id"<<Min[i].id<<"dur"<<Min[i].duration<<endl;
       //Min.push_back(M);
    }
//   cout<<"sortreq"<<endl;
    std::sort(Min.begin(),Min.end(),compare_duration);
//   cout<<"sr2"<<endl;
    for(int i=0;i<num_of_request;i++)
    {
        cout<<Min[i].id<<" dur= "<<Min[i].duration<<endl;
    }
    sorted_request_id.resize(num_of_request);
    for(int i=0;i<num_of_request;i++)
    {
        sorted_request_id[i]=Min[i].id;//this inserts the id number of the node
    }
    // for(int i=0;i<num_of_request;i++)
    // {
    //     cout<<sorted_request_id[i]<<" "<<endl;
    // }
}