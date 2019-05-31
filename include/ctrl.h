#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>

class Ctrl {
    public: 
    Ctrl();
    ~Ctrl();
    int getArguments(int argc, char *argv[]);
};