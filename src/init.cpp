#include "init.h"
#include "struct.h"

using namespace std;

Init::Init(){}
Init::~Init(){}

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

        close(fd);
    } else {
        cout << "Invalid number of arguments." << endl;
    }
    for(;;);
    return 0;
}