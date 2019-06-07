#include "init.h"
#include "struct.h"

using namespace std;

Init::Init(){}
Init::~Init(){}
void* routineThread(void *inbox);
Exam **colas;
queue <struct Exam> queueBlood;
queue <struct Exam> queueSkin;
queue <struct Exam> queueDetritos;

struct argHilo {
    sem_t* vacios;
    sem_t* lleno;
    sem_t* mutex;
    Exam *cola;
};

int Init::getArguments(int argc, char *argv[]) {
    int i_rec, ie_rec, oe_rec, b_rec, d_rec, s_rec, q_rec;
    string n_rec;
    if(argc >= 10) {
        for(int i = 1; i < argc; ++i) {
            string opt = argv[i];
            if(opt.compare("-i") == 0) {
                if(argv[i+1][0] != '-') {
                    i_rec = atoi(argv[i+1]);
                } else {
                    i_rec = 5;
                }
            } else if(opt.compare("-ie") == 0) {
                if(argv[i+1][0] != '-') {
                    ie_rec = atoi(argv[i+1]);
                } else {
                    ie_rec = 6;
                }
            } else if(opt.compare("-oe") == 0) {
                if(argv[i+1][0] != '-') {
                    oe_rec = atoi(argv[i+1]);
                } else {
                    oe_rec = 10;
                }
            } else if(opt.compare("-n") == 0) {
                if(argv[i+1][0] != '-') {
                    n_rec = argv[i+1];
                } else {
                    n_rec = "evaluator";
                }
            } else if(opt.compare("-b") == 0) {
                if(argv[i+1][0] != '-') {
                    b_rec = atoi(argv[i+1]);
                } else {
                    b_rec = 100;
                }
            } else if(opt.compare("-d") == 0) {
                if(argv[i+1][0] != '-') {
                    d_rec = atoi(argv[i+1]);
                } else {
                    d_rec = 100;
                }
            } else if(opt.compare("-s") == 0) {
                if(argv[i+1][0] != '-'){
                    s_rec = atoi(argv[i+1]);
                } else {
                    s_rec = 100;
                }
            } else if(opt.compare("-q") == 0) {
                if(i+1 == argc) {
                    q_rec = 6;
                } else if(argv[i+1][0] != '-') {
                    q_rec = atoi(argv[i+1]);
                }
            }
        }
        int fd = shm_open(&n_rec[0u], O_RDWR | O_CREAT | O_EXCL, 0660);

        if (fd < 0) {
            cerr << "Error creando la memoria compartida: " << errno << strerror(errno) << endl;
            exit(1);
        }
        if (ftruncate(fd, (sizeof(struct Exam) * i_rec * ie_rec) + sizeof(struct Exam) * oe_rec + sizeof(struct Header) != 0)) {
            cerr << "Error creando la memoria compartida: " << errno << strerror(errno) << endl;
            exit(1);
        }

        void *dir;
        if ((dir = mmap(NULL, sizeof(struct Header), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
            cerr << "Error mapeando la memoria compartida: " << errno << strerror(errno) << endl;
            exit(1);
        }

        struct Header *pHeader = (struct Header *) dir;
        pHeader -> i = i_rec;
        pHeader -> ie = ie_rec;
        pHeader -> oe = oe_rec;
        pHeader -> q = q_rec;
        pHeader -> b = b_rec;
        pHeader -> d = d_rec;
        pHeader -> s = s_rec;

        string semname = "vacios";
        sem_t **arraySemVacios = new sem_t *[i_rec];
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemVacios[j] = sem_open(realName.c_str(), O_CREAT | O_EXCL, 0660, ie_rec);
        }

        semname = "llenos";
        sem_t **arraySemLlenos = new sem_t *[i_rec];
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemLlenos[j] = sem_open(realName.c_str(), O_CREAT | O_EXCL, 0660, 0);
        }

        semname = "mutex";
        sem_t **arraySemMutex = new sem_t *[i_rec];
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemMutex[j] = sem_open(realName.c_str(), O_CREAT | O_EXCL, 0660, 1);
        }
        munmap(dir, sizeof(struct Header));
        dir = mmap(NULL, (sizeof(struct Exam) * i_rec * ie_rec) + (sizeof(struct Exam) * oe_rec) + sizeof(struct Header) , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        colas = new Exam*[i_rec];
        colas[0] = (struct Exam*) ((char *) dir) + sizeof(struct Header);
        for(int i = 1; i < i_rec; i++){
            colas[i] = (struct Exam*) ((char *) ((char *) dir) + sizeof(struct Header) + (sizeof(struct Exam) * ie_rec * i));
        }
        /*sleep(15);
        int i = 0;
        while(i < ie_rec){
            Exam *copy = (struct Exam*)((char*)colas[0]) + sizeof(struct Exam)*i;
            cout << i << ": "<< copy->id << endl;
            i++;
            sleep(2);
        }*/
        
        pthread_t hilosEntrada[i_rec];
        for(int i = 0; i < i_rec; i++){
            struct argHilo *arg = new argHilo;
            arg -> vacios = arraySemVacios[i];
            arg -> lleno = arraySemLlenos[i];
            arg -> mutex = arraySemMutex[i];
            arg -> cola = colas[i];
            pthread_create(&hilosEntrada[i], NULL, routineThread, arg);
            //pthread_join(hilosEntrada[i_rec], NULL);
        }
        //sem_wait(arraySemLlenos[]);
        //sem_wait(arraySemMutex[]);


        //sem_post(arraySemMutex[]);
        //sem_post(arraySemVacios[]);
        close(fd);
    } else {
        cout << "Invalid number of arguments." << endl;
    }
    for(;;);
    return 0;
}

void* routineThread(void *inbox){
    struct argHilo *arg = (argHilo*) inbox;
    do {
        // verificar si hay algo en la cola
        sem_wait(arg -> lleno);
        sem_wait(arg -> mutex);
    
        Exam *copy = (struct Exam*) arg -> cola;
        cout << copy -> id << " " << copy->i << " " << copy->k << copy->q <<endl;

        sem_post(arg -> mutex);
        sem_post(arg -> vacios);

    } while (true);
    
}