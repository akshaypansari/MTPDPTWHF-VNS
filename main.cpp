#include <iostream>
using namespace std;
#include "Run.h"
#include "Solution.h"
#include <vector>
#include <iomanip>
#include "InitialSolution.h"
#include <chrono>
#include <ctime>


int main()
{
    
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();    
    Run r;
    r.RunMTPDPTWHF("104editedMTPDPHFTw",1);//set the parameter here
    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();
 
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end-t_start).count()
              << " ms\n";

    return 0;
}