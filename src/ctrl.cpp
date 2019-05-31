#include "ctrl.h"

using namespace std;

Ctrl::Ctrl(){}
Ctrl::~Ctrl(){}

int Ctrl::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-s") == 0) {
        // ¿hay que comparar nameShareMem con -n del init?
        string nameShareMem;
        if(argc == 3){
            nameShareMem = "evaluator";
        }else{
            nameShareMem = argv[3];
        }
        // Interactive mode, subcmds: list, update
        string sub_cmd;
        cout << "> ";
        while (getline(cin, sub_cmd)) {
            char *sample;
            int reactive_level;
            if (sub_cmd.compare("list processing") == 0) {
                cout << "processing" << endl;
            } else if (sub_cmd.compare("list waiting") == 0) {
                cout << "waiting" << endl;
            } else if (sub_cmd.compare("list reported") == 0) {
                cout << "reported" << endl;
            } else if (sub_cmd.compare("list reactive") == 0) {
                cout << "reactive" << endl;
            } else if (sub_cmd.compare("list all") == 0) {
                cout << "all" << endl;
            } else if (sub_cmd.find("update") != string::npos) {
                char *arr[3];
                int i = 0;
                char * pch;
                pch = strtok(&sub_cmd[0u], " ");
                while (pch != NULL) {
                    arr[i] = pch;
                    pch = strtok (NULL, " ");
                    ++i;
                }
                sample = arr[1];
                reactive_level = atoi(arr[2]);
                if (reactive_level < 0) cout << "reactive level must be positive\n";
            } else {
                cout << "command not found" << endl;
            }
            cout << "> ";
        }
        
    } else {
        cout << "incorrect flag" << endl;
    }
    return 0;
}