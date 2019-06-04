#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>

class Init {
    public: 
    Init();
    ~Init();
    int getArguments(int argc, char *argv[]);
};