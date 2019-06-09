#include "rep.h"

using namespace std;

Rep::Rep(){}
Rep::~Rep(){}
int num_exams2 = 0;
queue <struct Exam> examenesCola;

int Rep::getArguments(int argc, char *argv[])  {
    string flag = argv[2];
    if (flag.compare("-n") == 0) {
        string nameShareMem = argv[3];
        string flag2 = argv[4]; // -i int || -m int
        
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
        Rep::colas = new Exam*[i_rec+1];
        for(int i = 0; i < i_rec + 1; i++){
            colas[i] = (struct Exam*) ((char *) ((char *) dir) + sizeof(struct Header) + (sizeof(struct Exam) * ie_rec * i));
        }
        sem_t *vaciosSalida = sem_open("vaciosSalida", oe_rec);
        sem_t *llenosSalida = sem_open("llenosSalida", 0);
        sem_t *mutexSalida = sem_open("mutexSalida", 1);
        
        if(flag2.compare("-i") == 0){
            int time = atoi(argv[5]);

        } else if(flag2.compare("-m") == 0) {
            int examenes = atoi(argv[5]);
            while (true) {
                sem_wait(llenosSalida);
                sem_wait(mutexSalida);
                struct Exam examen;
                Exam *copy;
                do{
                    copy = (struct Exam*) ((char*) (colas[i_rec]) + sizeof(struct Exam) * num_exams2);
                    if(copy->q != 0){
                        examen.id = copy -> id;
                        examen.i = copy -> i;
                        examen.k = copy -> k;
                        examen.q = copy -> q;
                        examen.r = copy -> r;
                        examen.p = copy -> p;
                        examenesCola.push(examen);
                        copy -> q = 0;
                    }
                    if(num_exams2 < oe_rec - 1) num_exams2 += 1;
                    else num_exams2 = 0;
                    if(examenesCola.size() == examenes) Rep::printExams(examenes);
                } while (num_exams2 < oe_rec - 1);
                
                sem_post(mutexSalida);
                sem_post(vaciosSalida);
            }
        }

    } else {
        cout << "incorrect flag" << endl;
    }
    return 0;
}

void Rep::printExams(int examenes) {
    struct Exam examen;
    for(int i = 0; i < examenes; i++){
        examen = examenesCola.front();
        cout << examen.id << " " << examen.i << " " << examen.k << " " << examen.r << endl;
        examenesCola.pop();
    }
}