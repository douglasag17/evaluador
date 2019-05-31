#include "stop.h"

using namespace std;

Stop::Stop(){}
Stop::~Stop(){}

int Stop::getArguments(char *argv[]) {
    string flag = argv[2];
    if (flag.compare("-n") == 0) {
        string nameShareMem;
        nameShareMem = argv[3];
        shm_unlink(&nameShareMem[0u]);
    }
    return 0;
}