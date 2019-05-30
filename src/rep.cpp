#include "rep.h"

using namespace std;

Rep::Rep(){}
Rep::~Rep(){}

int Rep::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-s")) {
        string nameShareMem = argv[3];
        // Interactive mode, subcmds: list, update
        
    } else {
        cout << "incorrect flag" << endl;
    }
    return 0;
}