#ifndef RUN_H_
#define RUN_H_
#include<string>

class Run
{
public:
    Run();
    virtual ~Run();
    void RunMTPDPTWHF(std::string filename, int seed);
};

#endif