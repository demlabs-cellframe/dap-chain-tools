#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dap_common.h"
#include "dap_enc.h"
#include "dap_config.h"
#include "dap_chain.h"

#define LOG_TAG "dap-chain-cli"

void init()
{
    dap_common_init(NULL);
    dap_config_init(NULL);
    dap_enc_init();
    dap_chain_init();
}

int main(int argc, const char *argv[])
{
    log_it(L_INFO,"dap-chain-cli version 0.0.1 \n");
    if( argc >1){
        init();
        char * l_config_name = argv[1];
        dap_config_t * l_config = dap_config_open(l_config_name);
        if( l_config ){
            const char * l_chain_name = dap_config_get_item_str(l_config,"general","name" );

            if(l_chain_name){
                dap_chain_t * l_chain = dap_chain_open(l_chain_name);
                if( l_chain){
                    if( argc >2){
                        if ( strcmp(argv[2],"block") ==0 ){
                            if(argc>3){
                                if ( strcmp(argv[3],"new") ==0 ){
                                    log_it(L_INFO,"Create new block");
                                }
                            }else{
                                log_it(L_CRITICAL, "Command 'block' need to be specified. Variant: 'new' 'show' ");
                            }
                        }
                    }else{
                        log_it(L_INFO, "Information about '%s'",l_chain_name);
                        dap_chain_info_dump_log(l_chain);
                    }
                }else{
                    log_it(L_CRITICAL,"Can't open dapchain %s",l_chain_name);
                }
            }else{
                log_it(L_CRITICAL,"Can't find dapchain name in config section 'general'");
            }
        }else{
            log_it(L_CRITICAL,"Can't open config name %s",l_config_name);
        }
    }
    return 0;
}
