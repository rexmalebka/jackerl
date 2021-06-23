#include <erl_nif.h>
#include <jack/jack.h>

#define MAX_OUTPUT_CHANNELS 100
#define MAX_CLIENTS 100

#define MAX_CLIENT_NAME 100
#define MAX_SERVER_NAME 100
#define MAX_PORT_NAME 100


typedef struct{
        int in_chann;
        int out_chann;
        double output[MAX_OUTPUT_CHANNELS];
} port_struct;

typedef struct {
	void * proc_indx[MAX_CLIENTS];
        jack_client_t *client[MAX_CLIENTS];
        jack_options_t options[MAX_CLIENTS];
        jack_status_t status[MAX_CLIENTS];
        port_struct port[MAX_CLIENTS];
} jack_struct;

typedef struct {
	jack_struct *ptr;
	int i;
} search_results;

typedef struct {
	ErlNifEnv *env;
	ErlNifPid pid_client;
	ErlNifPid pid_port;
	ErlNifPid pid_shutdown;
	ErlNifPid pid_process;
} gen_server_struct;

gen_server_struct erl_server;
