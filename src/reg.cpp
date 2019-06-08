#include "reg.h"

using namespace std;

Reg::Reg(){}
Reg::~Reg(){}

string ids_file = "";
int *num_exams;

int Reg::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-n") == 0) {
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
            munmap(dir, sizeof(struct Header));
            dir = mmap(NULL, (sizeof(struct Exam) * i_rec * ie_rec) + (sizeof(struct Exam) * oe_rec) + sizeof(struct Header) , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            
            //Code to create queues
            Reg::colas = new Exam*[i_rec+1];
            colas[0] = (struct Exam*) ((char *) dir) + sizeof(struct Header);
            for(int i = 1; i < i_rec + 1; i++){
                colas[i] = (struct Exam*) ((char *) ((char *) dir) + sizeof(struct Header) + (sizeof(struct Exam) * ie_rec * i));
            }
            num_exams = new int[i_rec];
            for(int i = 0; i < i_rec; i++) num_exams[i] = 0;
            
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
                if (inbox < 0 || (amount_sample < 1 || amount_sample > 5)) {
                    cout << "invalid parameters" << endl;
                }else{
                    openMem(false, nameShareMem,inbox, sample, amount_sample, i_rec, ie_rec);
                }
                cout << "> ";
            }
        } else {
            // reading file
            if (nameShareMem.find(".") != string::npos || nameShareMem.compare("evaluator") == 0){
                nameShareMem = "evaluator";
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
                munmap(dir, sizeof(struct Header));
                dir = mmap(NULL, (sizeof(struct Exam) * i_rec * ie_rec) + (sizeof(struct Exam) * oe_rec) + sizeof(struct Header) , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                
                //Code to create queues
                Reg::colas = new Exam*[i_rec+1];
                colas[0] = (struct Exam*) ((char *) dir) + sizeof(struct Header);
                for(int i = 1; i < i_rec + 1; i++){
                    colas[i] = (struct Exam*) ((char *) ((char *) dir) + sizeof(struct Header) + (sizeof(struct Exam) * ie_rec * i));
                }
                num_exams = new int[i_rec];
                for(int i = 0; i < i_rec; i++) num_exams[i] = 0;

                // read file
                for(int i = 3; i < argc; ++i){
                    string line;
                    ids_file = "";
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
                            if (inbox < 0 || (amount_sample < 1 || amount_sample > 5)) {
                                cout << "invalid parameters" << endl;
                            }else{
                                openMem(true ,nameShareMem, inbox, sample, amount_sample, i_rec, ie_rec);
                            }
                        }
                        ofstream outFile;
                        string nameFile = argv[i];
                        nameFile = nameFile.substr(0, nameFile.find(".")) + ".spl";
                        outFile.open(nameFile);
                        outFile << ids_file;
                        outFile.close();
                        myfile.close();
                    }
                }   
            }else{
                for(int i = 4; i < argc; ++i){
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
                    munmap(dir, sizeof(struct Header));
                    dir = mmap(NULL, (sizeof(struct Exam) * i_rec * ie_rec) + (sizeof(struct Exam) * oe_rec) + sizeof(struct Header) , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                    
                    //Code to create queues
                    Reg::colas = new Exam*[i_rec+1];
                    colas[0] = (struct Exam*) ((char *) dir) + sizeof(struct Header);
                    for(int i = 1; i < i_rec + 1; i++){
                        colas[i] = (struct Exam*) ((char *) ((char *) dir) + sizeof(struct Header) + (sizeof(struct Exam) * ie_rec * i));
                    }
                    num_exams = new int[i_rec];
                    for(int i = 0; i < i_rec; i++) num_exams[i] = 0;

                    // read file
                    string line;
                    ids_file = "";
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
                            if (inbox < 0 || (amount_sample < 1 || amount_sample > 5)) {
                                cout << "invalid parameters" << endl;
                            }else{
                                openMem(true, nameShareMem, inbox, sample, amount_sample, i_rec, ie_rec);
                            }
                        }
                        ofstream outFile;
                        string nameFile = argv[i];
                        nameFile = nameFile.substr(0, nameFile.find(".")) + ".spl";
                        outFile.open(nameFile);
                        outFile << ids_file;
                        outFile.close();
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

void Reg::openMem(bool isFile, string nameShareMem, int inbox, char *sample, int amount_sample, int i_rec, int ie_rec){
    if(inbox < i_rec){
        // opening semaphores
        string semname = "vacios";
        sem_t **arraySemVacios = new sem_t *[i_rec];
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemVacios[j] = sem_open(realName.c_str(), 0);
        }
        semname = "llenos";
        sem_t **arraySemLlenos = new sem_t *[i_rec];
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemLlenos[j] = sem_open(realName.c_str(), 0);
        }
        semname = "mutex";
        sem_t **arraySemMutex = new sem_t *[i_rec];
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemMutex[j] = sem_open(realName.c_str(), 0);
        }

        sem_wait(arraySemVacios[inbox]);
        sem_wait(arraySemMutex[inbox]);

        Exam *copy = (struct Exam*)((char*)colas[inbox]) + sizeof(struct Exam) * num_exams[inbox];
        //Exam *copy = (struct Exam*)((char*)colas[inbox]) + sizeof(struct Exam);
        //while(copy->q != 0){
        //    cout << "en ciclo" << endl;
        //    copy = (struct Exam*) ((char*) copy) + sizeof(struct Exam);
        //}
        // initialize queues
        copy->id = id;
        copy->i = inbox;
        copy->k = sample[0u];
        copy->q = amount_sample;
        if(num_exams[inbox] < ie_rec) num_exams[inbox]+=1;
        else num_exams[inbox] = 0;
        int id = Reg::id;
        if (isFile) { 
            ids_file += to_string(id) + "\n";
        } else cout << id << endl;
        Reg::id++;

        sem_post(arraySemMutex[inbox]);
        sem_post(arraySemLlenos[inbox]);
    }    
}