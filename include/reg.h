#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>

class Reg {
    public: 
    Reg();
    ~Reg();
    int getArguments(int argc, char *argv[]);
};