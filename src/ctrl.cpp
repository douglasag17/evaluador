#include "ctrl.h"

using namespace std;

Ctrl::Ctrl(){}
Ctrl::~Ctrl(){}

int Ctrl::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-n") == 0) {
        string nameShareMem;
        if(argc == 3){
            nameShareMem = "evaluator";
        }else{
            nameShareMem = argv[3];
        }

        int fd = shm_open(nameShareMem.c_str(), O_RDWR, 0660);
        if (fd < 0) {
            cerr << "Error abriendo la memoria compartida: " << errno << strerror(errno) << endl;
            exit(1);
        }
        void *dir;
        if ((dir = mmap(NULL, sizeof(struct Header), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
            cerr << "Error mapeando la memoria compartida: " << errno << strerror(errno) << endl;
            exit(1);
        }
        struct Header *pHeader = (struct Header *) dir;
        int i_rec = pHeader -> i;
        int oe_rec = pHeader ->oe;
        int ie_rec = pHeader -> ie;
        int sCopy = pHeader -> sCopy;
        int bCopy = pHeader -> bCopy;
        int dCopy = pHeader -> dCopy;
        munmap(dir, sizeof(struct Header));
        dir = mmap(NULL, (sizeof(struct Exam) * i_rec * ie_rec) + (sizeof(struct Exam) * oe_rec) + sizeof(struct Header) , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        Ctrl::colas = new Exam*[i_rec+1];
        for(int i = 0; i < i_rec + 1; i++){
            colas[i] = (struct Exam*) ((char *) ((char *) dir) + sizeof(struct Header) + (sizeof(struct Exam) * ie_rec * i));
        }
        // Interactive mode, subcmds: list, update
        string sub_cmd;
        cout << "> ";
        while (getline(cin, sub_cmd)) {
            char *sample;
            int reactive_level;
            if (sub_cmd.compare("list processing") == 0) {
                cout << "Processing: " << endl;
    
            } else if (sub_cmd.compare("list waiting") == 0) {
                cout << "Waiting: " << endl;
                for(int i = 0; i < i_rec; i++){
                    for(int j = 0; j < ie_rec; j++){
                        Exam *copy = (struct Exam*) ((char*) (colas[i]) + sizeof(struct Exam) * j);
                        if(copy->q != 0){
                            cout << copy -> id << " " << copy -> i << " " << copy -> k << " " << copy -> q << endl;
                        }
                    }
                }
            } else if (sub_cmd.compare("list reported") == 0) {
                cout << "Reported: " << endl;
                for(int i = 0; i < oe_rec; i++){
                    Exam *copy = (struct Exam*) ((char*) (colas[i_rec]) + sizeof(struct Exam) * i);
                    if(copy->q != 0){
                        cout << copy -> id << " " << copy -> i << " " << copy -> k << " " << copy -> r << endl;
                    }
                }
            } else if (sub_cmd.compare("list reactive") == 0) {
                munmap(dir, (sizeof(struct Exam) * i_rec * ie_rec) + (sizeof(struct Exam) * oe_rec) + sizeof(struct Header));
                dir = mmap(NULL, sizeof(struct Header) , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                struct Header *pHeader = (struct Header *) dir;
                cout << "Blood level: " << pHeader -> b << endl;
                cout << "Detritos level: " <<  pHeader -> d << endl;
                cout << "Skin level: " << pHeader -> s << endl;
            } else if (sub_cmd.compare("list all") == 0) {
                cout << "Processing: " << endl;
                
                cout << "Waiting: " << endl;
                for(int i = 0; i < i_rec; i++){
                    for(int j = 0; j < ie_rec; j++){
                        Exam *copy = (struct Exam*) ((char*) (colas[i]) + sizeof(struct Exam) * j);
                        if(copy->q != 0){
                            cout << copy -> id << " " << copy -> i << " " << copy -> k << " " << copy -> q << endl;
                        }
                    }
                }
                cout << "Reported: " << endl;
                for(int i = 0; i < oe_rec; i++){
                    Exam *copy = (struct Exam*) ((char*) (colas[i_rec]) + sizeof(struct Exam) * i);
                    if(copy->q != 0){
                        cout << copy -> id << " " << copy -> i << " " << copy -> k << " " << copy -> r << endl;
                    }
                }
                
                munmap(dir, (sizeof(struct Exam) * i_rec * ie_rec) + (sizeof(struct Exam) * oe_rec) + sizeof(struct Header));
                dir = mmap(NULL, sizeof(struct Header) , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                struct Header *pHeader = (struct Header *) dir;
                cout << "Blood level: " << pHeader -> b << endl;
                cout << "Detritos level: " <<  pHeader -> d << endl;
                cout << "Skin level: " << pHeader -> s << endl;
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
                else{
                    int valor = 0;
                    if(sample[0u] == 'B'){
                        if(pHeader -> b + reactive_level <= bCopy){
                            pHeader -> b += reactive_level;
                        }else{
                            valor = reactive_level - (reactive_level - (bCopy-pHeader -> b));
                            if(pHeader -> b + valor <= bCopy){
                                pHeader -> b += valor;
                            }  
                        }
                    }else if(sample[0u] == 'S'){
                        if(pHeader -> s + reactive_level <= sCopy){
                            pHeader -> s += reactive_level;
                        }else{
                            valor = reactive_level - (reactive_level - (sCopy-pHeader -> s));
                            if(pHeader -> s + valor <= sCopy){
                                pHeader -> s += valor;
                            }
                        }
                    }else if(sample[0u] == 'D'){
                        if(pHeader -> d + reactive_level <= dCopy){
                            pHeader -> d += reactive_level;
                        }else{
                            valor = reactive_level - (reactive_level - (dCopy-pHeader -> d));
                            if(pHeader -> d + valor <= dCopy){
                                pHeader -> d += valor;
                            }
                        }
                    }
                }
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