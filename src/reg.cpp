#include "reg.h"

using namespace std;

Reg::Reg(){}
Reg::~Reg(){}

int Reg::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-n") == 0) {
        // ¿hay que comparar nameShareMem con -n del init?
        string nameShareMem;
        string reg;
        if(argc == 4){
            nameShareMem = "evaluator";
            reg = argv[3];
        }else{
            nameShareMem = argv[3];
            reg = argv[4];
        }
        if (reg.compare("-") == 0) {
            // interactive mode
            string registers;
            cout << "> ";
            while (getline(cin, registers)) {
                int inbox, amount_sample;
                char *sample;
                char *arr[2];
                int i = 0;
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
                // verificar inbox menor que i-1
                if (inbox < 0 || (amount_sample < 1 || amount_sample > 5)) {
                    cout << "invalid parameters" << endl;
                }else{
                    openMem(nameShareMem, inbox, sample, amount_sample);
                }
                cout << "> ";
            }
        } else {
            // reading file
            if (nameShareMem.find(".") != string::npos || nameShareMem.compare("evaluator") == 0){
                nameShareMem = "evaluator";
                for(int i = 3; i < argc; ++i){
                    string line;
                    ifstream myfile (argv[i]);
                    if (myfile.is_open()) {
                        while (getline (myfile, line)) {
                            char *arr[2];
                            char * pch;
                            int j = 0;
                            int inbox, amount_sample;
                            char *sample;
                            pch = strtok(&line[0u], " ");
                            inbox = atoi(pch);
                            while (pch != NULL) {
                                pch = strtok (NULL, " ");
                                arr[j] = pch;
                                j++;
                            }
                            sample = arr[0];
                            amount_sample = atoi(arr[1]);
                            // verificar inbox menor que i-1
                            if (inbox < 0 || (amount_sample < 1 || amount_sample > 5)) {
                                cout << "invalid parameters" << endl;
                            }else{
                                openMem(nameShareMem, inbox, sample, amount_sample);
                            }
                        }
                        myfile.close();
                    }
                }   
            }else{
                for(int i = 4; i < argc; ++i){
                    //cout << argv[i] << endl;
                    string line;
                    ifstream myfile (argv[i]);
                    if (myfile.is_open()) {
                        while ( getline (myfile,line)) {
                            char *arr[2];
                            int j = 0;
                            char * pch;
                            int inbox, amount_sample;
                            char *sample;
                            pch = strtok(&line[0u], " ");
                            inbox = atoi(pch);
                            while (pch != NULL) {
                                pch = strtok (NULL, " ");
                                arr[j] = pch;
                                j++;
                            }
                            sample = arr[0];
                            amount_sample = atoi(arr[1]);
                            // verificar inbox menor que i-1
                            if (inbox < 0 || (amount_sample < 1 || amount_sample > 5)) {
                                cout << "invalid parameters" << endl;
                            }else{
                                openMem(nameShareMem, inbox, sample, amount_sample);
                            }
                        }
                        myfile.close();
                    }
                }     
            }
        }
    } else {
        cout << "incorrect flag" << endl;
    }
    return 0;
}

void Reg::openMem(string nameShareMem, int inbox, char *sample, int amount_sample){
    int id  = Reg::id;
    //cout << id << nameShareMem << inbox << sample << amount_sample << endl;
    int fd = shm_open(&nameShareMem[0u], O_RDWR, 0660);
    if (fd < 0) {
        cerr << "Error abriendo la memoria compartida: " << errno << strerror(errno) << endl;
        exit(1);
    }

    void *dir;
    if ((dir = mmap(NULL, sizeof(struct exam), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        cerr << "Error mapeando la memoria compartida: " << errno << strerror(errno) << endl;
        exit(1);
    }
    Reg::id++;
}