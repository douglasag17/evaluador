#include "init.h"

using namespace std;

Init::Init(){}
Init::~Init(){}

int Init::getArguments(int argc, char *argv[]) {
    int i_rec, ie_rec, oe_rec, b_rec, d_rec, ee_rec, s_rec, q_rec;
    string n_rec;
    if(argc >= 10) {
        for(int i = 1; i < argc; ++i) {
            string opt = argv[i];
            if(opt.compare("-i") == 0) {
                if(argv[i+1][0] != '-') {
                    i_rec = atoi(argv[i+1]);
                } else {
                    i_rec = 5;
                }
            } else if(opt.compare("-ie") == 0) {
                if(argv[i+1][0] != '-') {
                    ie_rec = atoi(argv[i+1]);
                } else {
                    ie_rec = 6;
                }
            } else if(opt.compare("-oe") == 0) {
                if(argv[i+1][0] != '-') {
                    oe_rec = atoi(argv[i+1]);
                } else {
                    oe_rec = 10;
                }
            } else if(opt.compare("-n") == 0) {
                if(argv[i+1][0] != '-') {
                    n_rec = argv[i+1];
                } else {
                    n_rec = "evaluator";
                }
            } else if(opt.compare("-b") == 0) {
                if(argv[i+1][0] != '-') {
                    b_rec = atoi(argv[i+1]);
                } else {
                    b_rec = 100;
                }
            } else if(opt.compare("-d") == 0) {
                if(argv[i+1][0] != '-') {
                    d_rec = atoi(argv[i+1]);
                } else {
                    d_rec = 100;
                }
            } else if(opt.compare("-s") == 0) {
                if(argv[i+1][0] != '-'){
                    s_rec = atoi(argv[i+1]);
                } else {
                    s_rec = 100;
                }
            } else if(opt.compare("-q") == 0) {
                if(i+1 == argc) {
                    q_rec = 6;
                } else if(argv[i+1][0] != '-') {
                    q_rec = atoi(argv[i+1]);
                }
            }
        }
    } else {
        cout << "Invalid number of arguments." << endl;
    }
    return 0;
}