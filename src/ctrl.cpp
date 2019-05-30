#include "ctrl.h"

using namespace std;

int Ctrl::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-s")) {
        string nameShareMem = argv[3];
        // Interactive mode, subcmds: list, update
        
    } else {
        cout << "incorrect flag" << endl;
    }
}