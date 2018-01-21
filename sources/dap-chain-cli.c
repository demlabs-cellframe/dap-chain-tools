#include "dap_chain.h"
#include "dap_chain_mine.h"
#include "dap_common.h"
#include "dap_config.h"
#include "dap_enc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_TAG "dap-chain-cli"

void init() {
    dap_common_init(NULL);
    dap_config_init(NULL);
    dap_enc_init();
    dap_chain_init();
}

void print_help() {}

int main(int argc, const char *argv[]) {
    log_it(L_INFO, "dap-chain-cli version 0.0.1 \n");
    if (argc > 1) {
        init();
        char *l_config_name = strdup(argv[1]);
        dap_config_t *l_config = dap_config_open(l_config_name);
        if (l_config) {
            const char *l_chain_name =
                    dap_config_get_item_str(l_config, "general", "name");
            const char *l_chain_file_storage =
                    dap_config_get_item_str(l_config, "general", "file_storage");
            const char *l_chain_file_cache =
                    dap_config_get_item_str(l_config, "general", "file_cache");

            if (l_chain_name) {
                dap_chain_t *l_chain =
                        dap_chain_open(l_chain_file_storage, l_chain_file_cache);
                if (l_chain) {
                    if (argc > 2) {
                        if (strcmp(argv[2], "block") == 0) {
                            if (argc > 3) {
                                if (strcmp(argv[3], "new") == 0) {
                                    log_it(L_INFO, "Create new block");
                                    dap_chain_block_cache_t *l_block_cache =
                                            dap_chain_allocate_next_block(l_chain);
                                    if (dap_chain_mine_block(l_block_cache, false, 0) == 0) {
                                        char *l_hash_str =
                                                dap_chain_hash_to_str_new(&l_block_cache->block_hash);
                                        log_it(L_INFO, "Block mined with hash %s ", l_hash_str);
                                        dap_chain_block_cache_dump(l_block_cache);
                                        DAP_DELETE(l_hash_str);
                                    }
                                }
                            } else {
                                log_it(L_CRITICAL, "Command 'block' need to be specified. "
                                                   "Variant: 'new' 'show' ");
                            }
                        } else if (strcmp(argv[2], "wallet") == 0) {
                            if (argc > 3) {
                                if (strcmp(argv[3], "create") == 0) {
                                    log_it(L_INFO, "Create blochain wallet");
                                }
                            } else {
                                log_it(L_CRITICAL, "Command 'wallet' need to be specified. "
                                                   "Variant: 'new' 'show' ");
                            }
                        }
                    } else {
                        log_it(L_INFO, "Information about '%s'", l_chain_name);
                        dap_chain_info_dump_log(l_chain);
                    }
                } else {
                    log_it(L_CRITICAL, "Can't open dapchain %s", l_chain_name);
                }
            } else {
                log_it(L_CRITICAL,
                       "Can't find dapchain name in config section 'general'");
            }
        } else {
            log_it(L_CRITICAL, "Can't open config name %s", l_config_name);
        }
        free(l_config_name);
    }
    return 0;
}
