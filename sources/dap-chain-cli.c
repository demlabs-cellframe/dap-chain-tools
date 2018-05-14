#include "dap_chain.h"
#include "dap_chain_mine.h"
#include "dap_common.h"
#include "dap_config.h"
#include "dap_enc.h"
#include "dap_enc_key.h"
#include "dap_enc_msrln16.h"
#include "dap_enc_aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <atomic>
//#include <cstdatomic>
/*#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>*/


#define LOG_TAG "dap-chain-cli"

//FILE* my_file_to_wite_gold;
FILE* my_file_to_wite_gold;
void* gold_mem;
FILE* my_file_to_wite_silver;
void* silver_mem;
FILE* my_file_to_wite_copper;
void* copper_mem;

int blocks_mined;
int blocks_mined_gold;
int blocks_mined_silver;
int blocks_mined_copper;

double total_mining_time;
double total_mining_hashes; //хз че делать пока
double total_hashes_in_minute;

size_t buf_enc_size;
size_t buf_size;
size_t buf_encrypted;
size_t buf_dec_size;

dap_enc_key_t* key;

void* buf_decrypted;
//std::atomic_flag work= ATOMIC_FLAG_INIT;
bool work;

void init() {
    dap_common_init(NULL);
    dap_config_init(NULL);
    dap_enc_init();
    dap_chain_init();
}

void print_help() {}

void make_new_block(){

}

void sing_handler(int signum){
    if (signum == 2){
    /*    munmap(gold_mem,16*blocks_mined);
        munmap(silver_mem, 16*blocks_mined);*/
        //fclose(my_file_to_wite_gold);
        //work.clear();
        dap_chain_mine_stop();
     /*   log_it(L_INFO, "Total hashes for gold coins %s B", ftell(my_file_to_wite_gold) );
        fclose(my_file_to_wite_gold);
        log_it(L_INFO, "Total hashes for silver coins %s B", ftell(my_file_to_wite_silver));
        fclose(my_file_to_wite_silver);
        log_it(L_INFO, "Total hashes for copper coins %s B", ftell(my_file_to_wite_copper));
        fclose(my_file_to_wite_copper);
        log_it(L_INFO, "Total blocks mined %s ", blocks_mined);
        log_it(L_INFO, "Gold blocks mined %s ", blocks_mined_gold);
        log_it(L_INFO, "Silver blocks mined %s ", blocks_mined_silver);
        log_it(L_INFO, "Copper blocks mined %s ", blocks_mined_copper);
        log_it(L_INFO, "Totla mining speed %s ", total_hashes_in_minute//blocks_mined);*/
        exit(0);
    }

}

int main(int argc, const char *argv[]) {
    log_it(L_INFO, "dap-chain-cli version 0.0.1 \n");
    //key_test();
    test_key_aes();
    signal(SIGINT, sing_handler);
    if (argc > 1) {
        if (strcmp(argv[1], "key")==0){
            if (argv[2]>  0){
                if(strcmp(argv[2],"RLWE_MSRLN16")==0){

                }
                else if(strcmp(argv[2],"SIDH16")==0){

                }
                else if (strcmp(argv[2], "FNAm2") == 0){

                }
                else if(strcmp(argv[2],"AES")==0){
                    log_it(L_INFO, "Generation AES key");
                    dap_enc_key_t * key = dap_enc_key_new_generate(DAP_ENC_KEY_TYPE_AES,16);
                    FILE *f = fopen("key.aes", "w");
                    fwrite (key->data, key->data_size,1, f );
                    fclose(f);
                }
                else {
                    log_it(L_CRITICAL, "Please, enter existing encryption type");
                }
            }
            else {
                log_it(L_CRITICAL, "Command generate key need to be specified. "
                                    "Please, enter encryption type");
            }
        } else if (strcmp(argv[1], "encrypt")==0){
            if (argv[2]>  0){
                if (strcmp(argv[2],"AES")==0){
                    if(argv[3] > 0){
                        char c_key[16];
                        if(read_key_from_file("key.aes",16,&c_key) == 0)
                        {
                            log_it(L_CRITICAL, "Please, generate public key first");
                            return;
                        }

                        size_t file_size = get_file_size(argv[3]);
                        if(file_size == 0){
                            log_it(L_CRITICAL, "File not exists");
                            return;
                        }
                        void* in = (void*)malloc(file_size);
                        read_data_from_file(argv[3],in,file_size);
                        void* out = (void*)malloc(file_size);
                        dap_enc_key_t * key = dap_enc_key_new_from_data(DAP_ENC_KEY_TYPE_AES,c_key,16);
                        dap_enc_code(key,in,file_size,out,DAP_ENC_DATA_TYPE_RAW);
                        write_data_to_file(strcat(argv[3],"_cipher"),out,file_size);
                    }
                    else{
                        log_it(L_CRITICAL, "Please, enter file name for encryption");
                    }

                }
                else {
                    log_it(L_CRITICAL, "Please, enter existing encryption type");
                }
            }
            else {
                log_it(L_CRITICAL, "Command encrypt need to be specified. "
                                    "Please, enter encryption type");
            }
        }  else if (strcmp(argv[1], "decrypt")==0){
            if (argv[2]>  0){
                if (strcmp(argv[2],"AES")==0){
                    if(argv[3] > 0){
                        char c_key[16];
                        if(read_key_from_file("key.aes",16,&c_key) == 0)
                        {
                            log_it(L_CRITICAL, "Please, generate public key first");
                            return;
                        }

                        size_t size = get_file_size(argv[3]);
                        if(size == 0){
                            log_it(L_CRITICAL, "File ",argv[3]," not exists");
                            return;
                        }
                        void* data = (void*)malloc(size);
                        read_data_from_file(argv[3], data,size);

                        void* out = (void*)malloc(size);
                        dap_enc_key_t * key = dap_enc_key_new_from_data(DAP_ENC_KEY_TYPE_AES,c_key,16);
                        dap_enc_decode(key,data,size,out,DAP_ENC_DATA_TYPE_RAW);
                        char* string = (char*)malloc(size);
                        memcpy(string,out,size);
                        string[size]='\0';

                        write_data_to_file(strcat(argv[3],"_decrypted"),out,size);

                        log_it(L_INFO, "Decrypted data: ",out);
                    }
                    else{
                        log_it(L_CRITICAL, "Please, enter file name for decryption");
                    }
                }
                else {
                    log_it(L_CRITICAL, "Please, enter existing encryption type");
                }
            }
            else {
                log_it(L_CRITICAL, "Command decrypt need to be specified. "
                                    "Please, enter encryption type");
            }

        } else{
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

                dap_chain_block_cache_t* l_g;
                dap_chain_block_cache_t* l_s;
                dap_chain_block_cache_t* l_c;
                dap_chain_file_open(l_g, l_s, l_c);

                if (l_chain) {
                    if (argc > 2) {
                        if (strcmp(argv[2], "block") == 0) {
                            if (argc > 3) {
                                if (strcmp(argv[3], "new") == 0) {//елси блон новый
                                    blocks_mined = 0;
                                    blocks_mined_copper = 0;
                                    blocks_mined_silver = 0;
                                    blocks_mined_gold = 0;
                                    total_hashes_in_minute = 0;
                                    log_it(L_INFO, "Create new block");//то пишем "блок новый"
                                    dap_chain_block_cache_t *l_block_cache;
                                    if (l_g==0&&l_s==0&&l_c==0){
                                    l_block_cache =

                                            dap_chain_allocate_next_block(l_chain);//размещаем новый блок
                                    }
                                    else if (l_g!=0){
                                        l_block_cache = l_g;
                                    }
                                    else if (l_s!=0){
                                        l_block_cache = l_s;
                                    }
                                    else if (l_c!=0){
                                        l_block_cache = l_g;
                                    }

                                    //m_hash_rate;
                                    if (dap_chain_mine_block(l_block_cache, false, 0) == 0) {//майним
                                        char *l_hash_str =
                                                dap_chain_hash_to_str_new(&l_block_cache->block_hash); //че-то хеш в строку переводим
                                        log_it(L_INFO, "Block mined with hash %s ", l_hash_str);

                                        dap_chain_count_new_block(l_block_cache);
                                        //total_hashes_in_minute = total_hashes_in_minute + sizeof(l_block_cache->block_hash)/l_block_cache->block_mine_time;
                                        //dap_chain_file_write(l_block_cache);
                                        dap_chain_update(l_block_cache);
//
                                        dap_chain_block_cache_dump(l_block_cache);//дамп че-то
                                        DAP_DELETE(l_hash_str);//хеш стирает

                                        while (true){
                                                dap_chain_block_new(&l_block_cache);
                                                l_block_cache =
                                                        dap_chain_allocate_next_block(l_chain);//размещаем новый блок
                                                if (dap_chain_mine_block(l_block_cache, false, 0) ==0){
                                                    blocks_mined+=1;
                                                        l_hash_str =
                                                             dap_chain_hash_to_str_new(&l_block_cache->block_hash); //че-то хеш в строку переводим
                                                         //dap_chain_file_write(l_block_cache);
                                                        dap_chain_update(l_block_cache);
//

                                                        log_it(L_INFO, "Block mined eith hash %s ", l_hash_str);
                                                        dap_chain_block_cache_dump(l_block_cache);//дамп че-то
                                                        DAP_DELETE(l_hash_str);//хеш стирает
                                                }

                                        }
                                    }
                                }
                            } else   {
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
						/* */
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
    }
    return 0;
}


int read_key_from_file(const char* filename, size_t key_size, char* key){
    if(get_file_size(filename)==0)
        return 0;
   int s = get_file_size(filename);
    FILE *f = fopen(filename, "r");
    fread(key,sizeof(char),key_size,f); 
    fclose(f);
    return 1;
}

void write_data_to_file(const char* filename,const void* data, size_t data_size){
    FILE *f = fopen(filename, "w");
    fwrite(data,data_size,1,f); 
    fclose(f);
}

void read_data_from_file(const char* filename, void* data, size_t data_size){
    FILE *f = fopen(filename, "r");
    fread(data,data_size,1,f); 
    fclose(f);
}

int get_file_size(const char* filename){
    struct stat st; 
    stat(filename,&st);
    return st.st_size;
}


