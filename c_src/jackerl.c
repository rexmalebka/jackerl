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
	{"activate", 1, client_activate},
	{"register_", 3, port_register},
/*
	{"client_close", 1, client_close},
	{"client_deactivate", 1, client_deactivate},
	{"client_get", 2, client_get},
	{"port_unregister", 3, client_open},
	{"port_rename", 3, client_open},
	{"port_connect", 3, client_open},
	{"port_disconnect", 3, client_open},
*/
};

int nif_load(ErlNifEnv* env, void **argc, const ERL_NIF_TERM argv){  
	ErlNifPid* pid;
	erl_server.env = enif_alloc_env();
	enif_get_local_pid(env, argv, pid);

	erl_server.pid = *pid;

	return 0;
}

int nif_upgrade(ErlNifEnv* env, void **argc, ERL_NIF_TERM argv){
	return 0;
}

void nif_unload(ErlNifEnv* env, void *argc){
}

ERL_NIF_INIT(jack, nif_funcs, nif_load, NULL, NULL, nif_unload)
