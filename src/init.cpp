#include "init.h"
#include "struct.h"

using namespace std;

Init::Init(){}
Init::~Init(){}
void* routineThread(void *inbox);
void* routineInternos(void *inbox);
Exam **colas;
int *num_exams1;
int numExamSalida = 0;
queue <struct Exam> queueBlood;
queue <struct Exam> queueSkin;
queue <struct Exam> queueDetritos;
sem_t *arraySemVaciosInterno[3];
sem_t *arraySemLlenosInterno[3];
sem_t *arraySemMutexInterno[3];
sem_t *vaciosSalida;
sem_t *llenosSalida;
sem_t *mutexSalida;
struct argHilo {
    sem_t* vacios;
    sem_t* lleno;
    sem_t* mutex;
    Exam *cola;
    int indice;
    int q_rec;
    int ie_rec = 0;
};

struct argInternos {
    sem_t* vaciosInternos;
    sem_t* llenoInternos;
    sem_t* mutexInternos;
    int typeQueue;
    void *dirHeader;
    Exam *cola;
    int oe_rec = 0;
};
void *dirHeader;

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
        if (ftruncate(fd, sizeof(struct Header) + (sizeof(struct Exam) * i_rec * ie_rec) + (sizeof(struct Exam) * oe_rec) != 0)) {
            cerr << "Error creando la memoria compartida: " << errno << strerror(errno) << endl;
            exit(1);
        }

        void *dir;
        if ((dir = mmap(NULL, sizeof(struct Header), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
            cerr << "Error mapeando la memoria compartida: " << errno << strerror(errno) << endl;
            exit(1);
        }
        dirHeader = dir;

        struct Header *pHeader = (struct Header *) dir;
        pHeader -> i = i_rec;
        pHeader -> ie = ie_rec;
        pHeader -> oe = oe_rec;
        pHeader -> q = q_rec;
        pHeader -> b = b_rec;
        pHeader -> d = d_rec;
        pHeader -> s = s_rec;

        // creating semaphores
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
        semname = "vaciosInterno";
        for (int j = 0; j < 3; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemVaciosInterno[j] = sem_open(realName.c_str(), O_CREAT | O_EXCL, 0660, q_rec);
        }
        semname = "llenosInterno";
        for (int j = 0; j < 3; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemLlenosInterno[j] = sem_open(realName.c_str(), O_CREAT | O_EXCL, 0660, 0);
        }
        semname = "mutexInterno";
        for (int j = 0; j < 3; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            arraySemMutexInterno[j] = sem_open(realName.c_str(), O_CREAT | O_EXCL, 0660, 1);
        }
        vaciosSalida = sem_open("vaciosSalida", O_CREAT | O_EXCL, 0660, oe_rec);
        llenosSalida = sem_open("llenosSalida", O_CREAT | O_EXCL, 0660, 0);
        mutexSalida = sem_open("mutexSalida", O_CREAT | O_EXCL, 0660, 1);

        munmap(dir, sizeof(struct Header));
        dir = mmap(NULL, sizeof(struct Header) + sizeof(struct Exam) * i_rec * ie_rec + sizeof(struct Exam) * oe_rec, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        //Code to create queues
        colas = new Exam*[i_rec + 1];
        for(int i = 0; i < i_rec + 1; i++){
            colas[i] = (struct Exam*) ((char *) ((char *) dir) + sizeof(struct Header) + (sizeof(struct Exam) * ie_rec * i));
        }

        num_exams1 = new int[i_rec];
        for(int i = 0; i < i_rec; i++) num_exams1[i] = 0;
        
        pthread_t hilosEntrada[i_rec];
        for(int i = 0; i < i_rec; i++){
            struct argHilo *arg = new argHilo;
            arg -> vacios = arraySemVacios[i];
            arg -> lleno = arraySemLlenos[i];
            arg -> mutex = arraySemMutex[i];
            arg -> cola = colas[i];
            arg -> indice = i;
            arg -> q_rec = q_rec;
            arg -> ie_rec = ie_rec;
            pthread_create(&hilosEntrada[i], NULL, routineThread, arg);
        }  

        pthread_t hilosInternos[3];
        for(int i = 0; i < 3; i++){
            struct argInternos *arg = new argInternos;
            arg -> vaciosInternos = arraySemVaciosInterno[i];
            arg -> llenoInternos = arraySemLlenosInterno[i];
            arg -> mutexInternos = arraySemMutexInterno[i];
            arg -> typeQueue = i;
            arg -> dirHeader = dirHeader;
            arg -> cola = colas[i_rec];
            arg -> oe_rec = oe_rec;
            pthread_create(&hilosInternos[i], NULL, routineInternos, arg);
        }

        for (int i = 0; i < i_rec; i++) {
            pthread_join(hilosEntrada[i], NULL);
        } 
        for (int i = 0; i < 3; i++) {
            pthread_join(hilosInternos[i], NULL);
        }

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
        struct Exam examen;

        sem_wait(arg -> lleno);
        sem_wait(arg -> mutex);

        Exam *copy = (struct Exam*) ((char*) (arg -> cola) + sizeof(struct Exam) * num_exams1[arg->indice]);
        examen.id = copy->id;
        examen.i = copy->i;
        examen.k = copy->k;
        examen.q = copy->q;
        if(copy->k == 'S'){
            sem_wait(arraySemVaciosInterno[0]);
            sem_wait(arraySemMutexInterno[0]);
            
            while(queueSkin.size() >= (unsigned) arg->q_rec) {
                //Cola llena
            }
            cout << copy->id << copy->i << copy->k << copy->q << endl; 
            queueSkin.push(examen);
            copy -> q = 0;
            cout << "Skin" << endl;

            sem_post(arraySemMutexInterno[0]);
            sem_post(arraySemLlenosInterno[0]);
        }else if(copy->k == 'B'){
            sem_wait(arraySemVaciosInterno[1]);
            sem_wait(arraySemMutexInterno[1]);

            while(queueBlood.size() >= (unsigned) arg->q_rec) {
                //Cola llena
            }
            cout << copy->id << copy->i << copy->k << copy->q << endl; 
            queueBlood.push(examen);
            copy -> q = 0;
            cout << "Blood" << endl;

            sem_post(arraySemMutexInterno[1]);
            sem_post(arraySemLlenosInterno[1]);
        }else if(copy->k == 'D'){
            sem_wait(arraySemVaciosInterno[2]);
            sem_wait(arraySemMutexInterno[2]);
            
            while(queueDetritos.size() >= (unsigned) arg->q_rec) {
                //Cola llena
            }
            cout << copy->id << copy->i << copy->k << copy->q << endl; 
            queueDetritos.push(examen);
            copy -> q = 0;
            cout << "Detritos" << endl;

            sem_post(arraySemMutexInterno[2]);
            sem_post(arraySemLlenosInterno[2]);
        }
        if(num_exams1[arg->indice] < arg -> ie_rec-1) num_exams1[arg->indice]+=1;
        else num_exams1[arg->indice] = 0;

        sem_post(arg -> mutex);
        sem_post(arg -> vacios);
    } while (true);
    
}

void* routineInternos(void *inbox) {
    struct argInternos *arg = (argInternos*) inbox;
    struct Header *pHeader = (struct Header *) arg -> dirHeader;
    do {
        struct Exam examen;
        sem_wait(arg -> llenoInternos);
        sem_wait(arg -> mutexInternos);
        Exam *copy = (struct Exam*) ((char*) (arg -> cola) + sizeof(struct Exam) * numExamSalida);
        if (arg->typeQueue == 0) {
            // Skin
            examen = queueSkin.front();
            cout << "pop" << endl;
            queueSkin.pop();
            // Procesar Muestra
            srand(time(NULL));
            for (int i = 0; i < examen.q; i++) {
                int randomSample = rand() % (25-8 +1) + 8;
                pHeader -> s -= randomSample;
            }
            // Obtencion resultado
            int randomResult = rand() % (50-0 +1) + 0;
            if (randomResult <= 15) {
                examen.r = '?';
            } else if (randomResult > 15 && randomResult <= 35) {
                examen.r = 'N';
            } else if (randomResult > 35 && randomResult <= 50) {
                examen.r = 'P';
            }
            // Tiempo de procesamiento
            int randomTime = rand() % 10;
            examen.p = randomTime;
            // Agregar a la cola de salida
            copy -> id = examen.id;
            copy -> i = examen.i;
            copy -> k = examen.k;
            copy -> q = examen.q;
            copy -> r = examen.r;
            copy -> p = examen.p;

        } else if (arg->typeQueue == 1) {
            // Blood
            examen = queueBlood.front();
            cout << "pop" << endl;
            queueBlood.pop();
            // Procesar Muestra
            srand(time(NULL));
            for (int i = 0; i < examen.q; i++) {
                int randomSample = rand() % (7-1 +1) + 1;
                pHeader -> b -= randomSample;
            }
            // Obtencion resultado
            int randomResult = rand() % (50-0 +1) + 0;
            if (randomResult <= 15) {
                examen.r = '?';
            } else if (randomResult > 15 && randomResult <= 35) {
                examen.r = 'N';
            } else if (randomResult > 35 && randomResult <= 50) {
                examen.r = 'P';
            }
            // Tiempo de procesamiento
            int randomTime = rand() % 10;
            examen.p = randomTime;
            // Agregar a la cola de salida
            copy -> id = examen.id;
            copy -> i = examen.i;
            copy -> k = examen.k;
            copy -> q = examen.q;
            copy -> r = examen.r;
            copy -> p = examen.p;
            
        } else if (arg->typeQueue == 2) {
            // Detritos
            examen = queueDetritos.front();            
            cout << "pop" << endl;
            queueDetritos.pop();
            // Procesar Muestra
            srand(time(NULL));
            for (int i = 0; i < examen.q; i++) {
                int randomSample = rand() % (20-5 +1) + 5;
                pHeader -> d -= randomSample;
            }
            // Obtencion resultado
            int randomResult = rand() % (50-0 +1) + 0;
            if (randomResult <= 15) {
                examen.r = '?';
            } else if (randomResult > 15 && randomResult <= 35) {
                examen.r = 'N';
            } else if (randomResult > 35 && randomResult <= 50) {
                examen.r = 'P';
            }
            // Tiempo de procesamiento
            int randomTime = rand() % 10;
            examen.p = randomTime;
            // Agregar a la cola de salida
            copy -> id = examen.id;
            copy -> i = examen.i;
            copy -> k = examen.k;
            copy -> q = examen.q;
            copy -> r = examen.r;
            copy -> p = examen.p;
        }
        if(numExamSalida < arg -> oe_rec - 1) numExamSalida += 1;
        else numExamSalida = 0;
        
        sem_post(arg -> mutexInternos);
        sem_post(arg -> vaciosInternos);
    } while (true);
}