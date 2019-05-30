#include "init.h"
#include "reg.h"
#include "ctrl.h"
#include "rep.h"

#include <stdio.h>  
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class Evaluator {
    int main(int argc, char *argv[]) {
        string cmd = argv[1];
        if (cmd.compare("init") == 0) {
            Init init;
            init.getArguments(argc, argv);
            cout << "entreee" << endl;
        } else if (cmd.compare("reg") == 0) {
            cout << argv[2] << endl;
        } else if (cmd.compare("ctrl") == 0) {
            cout << argv[2] << endl;
        } else if (cmd.compare("rep") == 0) {
            cout << argv[2] << endl;
        } else {
            cout << "command not found" << endl;
        }
        return 0;
    }
};