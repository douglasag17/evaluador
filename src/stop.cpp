#include "stop.h"
#include "struct.h"

using namespace std;

Stop::Stop(){}
Stop::~Stop(){}

int Stop::getArguments(char *argv[]) {
    string flag = argv[2];
    if (flag.compare("-n") == 0) {
        string nameShareMem;
        nameShareMem = argv[3];
        // deleting semaphores
        int fd = shm_open(&nameShareMem[0u], O_RDWR, 0660);
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

        string semname = "vacios";
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            sem_unlink(&realName[0u]);
        }
        semname = "llenos";
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            sem_unlink(&realName[0u]);
        }
        semname = "mutex";
        for (int j = 0; j < i_rec; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            sem_unlink(&realName[0u]);
        }
        semname = "vaciosInterno";
        for (int j = 0; j < 3; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            sem_unlink(&realName[0u]);
        }
        semname = "llenosInterno";
        for (int j = 0; j < 3; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            sem_unlink(&realName[0u]);
        }
        semname = "mutexInterno";
        for (int j = 0; j < 3; j++) {
            ostringstream name;
            name << semname << j;
            string realName(name.str());
            sem_unlink(&realName[0u]);
        }
        sem_unlink("vaciosSalida");
        sem_unlink("llenosSalida");
        sem_unlink("mutexSalida");

        // deleting share memory
        shm_unlink(&nameShareMem[0u]);
    }
    return 0;
}