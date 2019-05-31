#include "reg.h"

using namespace std;

Reg::Reg(){}
Reg::~Reg(){}

int Reg::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-n") == 0) {
        // ¿hay que comparar nameShareMem con -n del init?
        string nameShareMem = argv[3];
        // Interactive mode, subcmds: list, update
        string reg = argv[4];
        if (reg.compare("-") == 0) {
            // interactive mode
            while (true) {
                int inbox, amount_sample;
                char *sample;
                char *arr[2];
                int i = 0;
                string registers;
                cout << "> ";
                getline(cin, registers);
                char * pch;
                pch = strtok(&registers[0u], " ");
                inbox = atoi(pch);
                while (pch != NULL) {
                    pch = strtok (NULL, " ");
                    arr[i] = pch;
                    i++;
                }
                sample = arr[0];
                amount_sample = atoi(arr[1]);
                cout << "registros melisimos" << endl;
                // verificar inbox menor que i-1
                if (inbox < 0 || (amount_sample < 1 || amount_sample > 5)) {
                    cout << "invalid parameters" << endl;
                }
            }
        } else {
            // reading file

        }
        
        
    } else {
        cout << "incorrect flag" << endl;
    }
    return 0;
}