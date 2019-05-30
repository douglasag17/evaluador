#include "reg.h"

using namespace std;

int Reg::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-s")) {
        string nameShareMem = argv[3];
        // Interactive mode, subcmds: list, update
        
    } else {
        cout << "incorrect flag" << endl;
    }
}