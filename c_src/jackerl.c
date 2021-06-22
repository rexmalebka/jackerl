#include <erl_nif.h>
#include <jack/jack.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "jackerl_types.h"
#include "client.h"
#include "port.h"


//jack data




static ErlNifFunc nif_funcs[] = {
	{"open", 3, client_open},
	{"close", 2, client_close},

	{"activate", 1, client_activate},
	{"deactivate", 1, client_deactivate},

	{"register_", 3, port_register},
	{"unregister", 2, port_unregister},
	//{"set_buffer", 2, set_buffer},
	
/*
	{"client_get", 2, client_get},
	{"port_unregister", 3, client_open},
	{"port_rename", 3, client_open},
	{"port_connect", 3, client_open},
	{"port_disconnect", 3, client_open},
*/
};

int nif_load(ErlNifEnv* env, void **argc, const ERL_NIF_TERM argv){  
	
	// pid for callbacks 
	ErlNifPid pid_client;
	ErlNifPid pid_port;
	ErlNifPid pid_shutdown;
	ErlNifPid pid_process;

	erl_server.env = enif_alloc_env();
	

	const ERL_NIF_TERM* arr;
	int arity;
	enif_get_tuple(
			env,
			argv,
			&arity,
			&arr
			);


	enif_get_local_pid(env, arr[0], &pid_client);
	enif_get_local_pid(env, arr[1], &pid_port);
	enif_get_local_pid(env, arr[2], &pid_shutdown);
	enif_get_local_pid(env, arr[3], &pid_process);

	erl_server.pid_client = pid_client;
	erl_server.pid_port = pid_port;
	erl_server.pid_shutdown = pid_shutdown;
	erl_server.pid_process = pid_process;


	for(int i=0; i<MAX_CLIENTS; i++){
		for(int j=0; j<MAX_INPUT_CHANNELS; j++){
			jack_clients.port[i].input_port[j] = NULL;
		}
		for(int j=0; j<MAX_OUTPUT_CHANNELS; j++){
			jack_clients.port[i].output_port[j] =NULL;
		}
	}


	return 0;
}

int nif_upgrade(ErlNifEnv* env, void **argc, ERL_NIF_TERM argv){
	return 0;
}

void nif_unload(ErlNifEnv* env, void *argc){
}

ERL_NIF_INIT(jack, nif_funcs, nif_load, NULL, NULL, nif_unload)
