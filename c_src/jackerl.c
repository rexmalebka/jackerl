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


/*
ERL_NIF_TERM client_open2(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){


	// get client_name, server name and jack options
	char client_name[100];
	char server_name[100];
	int jackopts;

	enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
	enif_get_atom(env, argv[1], server_name,sizeof(server_name), ERL_NIF_LATIN1);
	enif_get_int(env, argv[2], &jackopts);

	// create a jack struct object
	int i = jack_clients_count;

	jack_clients.options[i] = jackopts;
	jack_clients.port[i].in_chann = 0;
	jack_clients.port[i].out_chann = 0;


	// try to open the jack client
	jack_clients.client[i] = jack_client_open (
			client_name, jack_clients.options[i], &jack_clients.status[i], server_name);


	if(jack_clients.status[i] == 0){
		// everything is okay :)
		
		// register on jack_clients
		jack_clients_count++;

		return enif_make_tuple2(env, enif_make_atom(env, "ok"), 
				enif_make_tuple2(env, enif_make_atom(env, "jackstatus"), enif_make_int(env, jack_clients.status[i]))
				);
	
	}else if(jack_clients.status[i] == 4 ){
		//jack_clients.client_name[i] = jack_get_client_name(jack_clients.client[i]);

		// register on jack_clients
		jack_clients_count++;

		return enif_make_tuple3(env, enif_make_atom(env, "ok"), 
			enif_make_tuple2(env, enif_make_atom(env, "jackstatus"), enif_make_int(env, jack_clients.status[i])),
			enif_make_tuple2(env, enif_make_atom(env, "clientname"), enif_make_atom(env,
				       	jack_get_client_name(jack_clients.client[i])))
			);	
	}else{
		return enif_make_tuple2(env, enif_make_atom(env, "error"), 
				enif_make_tuple2(env, enif_make_atom(env, "jackstatus"), enif_make_int(env, jack_clients.status[i])));
	}
}

ERL_NIF_TERM register_port(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){

	// get client_name, server name and jack options
	char client_name[100];
	char port_name[100];
	int portflags;

	enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
	enif_get_atom(env, argv[1], port_name,sizeof(port_name), ERL_NIF_LATIN1);
	enif_get_int(env, argv[2], &portflags);
	
	int i = searchJackClient(client_name);

	if(i<0){
		return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, "jackclientnotfound"));
	}

	const  char *portname;
	if(portflags & 1){
		// if is input
		int j = jack_clients.port[i].in_chann;
		jack_clients.port[i].input_port[j] = jack_port_register (jack_clients.client[i], port_name,
						 JACK_DEFAULT_AUDIO_TYPE,
						 portflags, 0);
		if(jack_clients.port[i].input_port[j] == NULL){
			return enif_make_tuple2(env, enif_make_atom(env, "error"), 
					enif_make_atom(env, "portsnotavailable"));
		}

		portname = jack_port_name(jack_clients.port[i].input_port[j]);
		jack_clients.port[i].in_chann++;
		jack_activate(jack_clients.client[i]);

	}else if(portflags & 2){
		int j = jack_clients.port[i].out_chann;
		jack_clients.port[i].output_port[j] = jack_port_register (jack_clients.client[i], port_name,
						 JACK_DEFAULT_AUDIO_TYPE,
						 portflags, 0);

		if(jack_clients.port[i].output_port[j] == NULL){
			return enif_make_tuple2(env, enif_make_atom(env, "error"), 
					enif_make_atom(env, "portnotavailable"));
		}

		portname = jack_port_name(jack_clients.port[i].output_port[j]);
		jack_clients.port[i].out_chann++;
		jack_activate(jack_clients.client[i]);
	}


	return enif_make_tuple2(env, enif_make_atom(env, "ok"), 
			enif_make_tuple2(env,
				enif_make_atom(env, "portregistered"),
				enif_make_atom(env, portname) 
				)
			);
}

*/	


static ErlNifFunc nif_funcs[] = {
	{"client_open", 3, client_open},
	{"client_activate", 1, client_activate},
	{"port_register_", 3, port_register},
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
	return 0;
}

int nif_upgrade(ErlNifEnv* env, void **argc, ERL_NIF_TERM argv){
	return 0;
}

void nif_unload(ErlNifEnv* env, void *argc){
}

ERL_NIF_INIT(jack, nif_funcs, NULL, NULL, NULL, nif_unload)
