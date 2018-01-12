#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dap_common.h"
#include "dap_config.h"
#include "dap_chain.h"
#include "dap_chain.h"

#define LOG_TAG "dap-chain-cli"
int main(int argc, const char *argv[])
{
    log_it(L_INFO,"dap-chain-cli version 0.0.1 \n");
    if( argc >1){
        if ( strcmp(argv[1],"block") ==0 ){
            if ( strcmp(argv[1],"new") ==0 ){
                log_it(L_INFO,"Create new block");
            }
        }
    }
    return 0;
}
