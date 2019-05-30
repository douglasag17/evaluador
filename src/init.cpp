#include "init.h"

using namespace std;

int Init::getArguments(int argc, char *argv[])  {
    if(argc >= 10){
        for(int i = 1; i < argc; ++i){
            string opt = argv[i];
            if(opt.compare("-i") == 0){
                if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }else{
                    cout << "opt por default" << endl;
                }
            }else if(opt.compare("-ie") == 0){
                if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }else{
                    cout << "opt por default" << endl;
                }
            } else if(opt.compare("-oe") == 0){
                if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }else{
                    cout << "opt por default" << endl;
                }
            } else if(opt.compare("-n") == 0){
                if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }else{
                    cout << "opt por default" << endl;
                }
            }else if(opt.compare("-b") == 0){
                if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }else{
                    cout << "opt por default" << endl;
                }
            }else if(opt.compare("-d") == 0){
                if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }else{
                    cout << "opt por default" << endl;
                }
            }else if(opt.compare("-ee") == 0){
                if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }else{
                    cout << "opt por default" << endl;
                }
            }else if(opt.compare("-s") == 0){
                if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }else{
                    cout << "opt por default" << endl;
                }
            }else if(opt.compare("-q") == 0){
                if(i+1 == argc){
                    cout << "opt por default 9" << endl;
                }else if(argv[i+1][0] != '-'){
                    cout << argv[i+1] << endl;
                }
            }
        }
    }else{
        cout << "Invalid number of arguments." << endl;
    }
}