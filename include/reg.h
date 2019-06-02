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
#include "struct.h"

class Reg {
    public: 
    Reg();
    ~Reg();
    int id = 0;
    int getArguments(int argc, char *argv[]);
    void openMem(string nameShareMem, int inbox, char *sample,
        int amount_sample);
};