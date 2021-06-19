#include <erl_nif.h>
#include <jack/jack.h>

#define MAX_INPUT_CHANNELS 100
#define MAX_OUTPUT_CHANNELS 100
#define MAX_CLIENTS 100

#define MAX_CLIENT_NAME 100
#define MAX_SERVER_NAME 100
#define MAX_PORT_NAME 100


typedef struct{
        int in_chann;
        int out_chann;
        jack_port_t *input_port[MAX_INPUT_CHANNELS];
        jack_port_t *output_port[MAX_OUTPUT_CHANNELS];
} port_struct;

typedef struct {
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
	ErlNifPid pid;
} gen_server_struct;

gen_server_struct erl_server;
