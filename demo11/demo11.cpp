//
// Created by root on 6/29/18.
//

#include <stdio.h>
#include "demoConfig.h"

#include "demo7/demo7_1.h"

int main(int argc, char* argv[]){

    printf("%s version: %d.%d \n", argv[0], demo11_VERSION_MAJOR, demo11_VERSION_MINOR);

    demo7::print_demo71();
}

