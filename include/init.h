#pragma once
#include <stdio.h>
#include <string>
#include <iostream>

class Init {
    public: 
    Init();
    ~Init();
    int getArguments(int argc, char *argv[]);
};