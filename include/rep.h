#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include "struct.h"
#include <queue>

class Rep {
    public: 
    Rep();
    ~Rep();
    Exam **colas;
    int getArguments(char *argv[]);
    void printExams(int examenes);
};