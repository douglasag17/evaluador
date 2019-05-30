#pragma once
#include <stdio.h>
#include <string>
#include <iostream>

class Reg {
    public: 
    Reg();
    ~Reg();
    int getArguments(int argc, char *argv[]);
};