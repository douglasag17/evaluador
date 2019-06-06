#pragma once
#include "struct.h"
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
#include <cstdlib>


class Reg {
    public: 
    Reg();
    ~Reg();
    int id = 0;
    int getArguments(int argc, char *argv[]);
    void openMem(bool isFile, string nameShareMem, int inbox, char *sample,
        int amount_sample);
};