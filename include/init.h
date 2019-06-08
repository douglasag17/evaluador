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
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <queue>
#include <time.h>
#include "struct.h"

class Init {
    public: 
    Init();
    ~Init();
    int getArguments(int argc, char *argv[]);
};