#include "callbacks.h"

int JACK_CLIENTS_COUNT = 0;

jack_struct jack_clients;


ERL_NIF_TERM get_jackStatus(ErlNifEnv* env, unsigned int jackStatus){
	unsigned int n = 0;
	unsigned int j = jackStatus ;
	while(j && (n<14)){
		n += j & 1;
		j >>= 1;
	}

	ERL_NIF_TERM arr[n];
	for(int i=0; i<n; i++ ){
		int o = 0;
		if(jackStatus & JackFailure ) {
			arr[i] = enif_make_atom(env, "jackFailure");
			o = JackFailure;
		}else if(jackStatus & JackInvalidOption ) {
			arr[i] = enif_make_atom(env, "jackInvalidOption");
			o = JackInvalidOption;
		}else if(jackStatus & JackNameNotUnique ) {
			arr[i] = enif_make_atom(env, "jackNameNotUnique");
			o = JackNameNotUnique;
		}else if(jackStatus & JackServerStarted ) {
			arr[i] = enif_make_atom(env, "jackServerStarted");
			o = JackServerStarted;
		}else if(jackStatus & JackServerFailed ) {
			arr[i] = enif_make_atom(env, "jackServerFailed");
			o = JackServerFailed;
		}else if(jackStatus & JackServerError ) {
			arr[i] = enif_make_atom(env, "jackServerError");
			o = JackServerError;
		}else if(jackStatus & JackNoSuchClient ) {
			arr[i] = enif_make_atom(env, "jackNoSuchClient");
			o = JackNoSuchClient;
		}else if(jackStatus & JackLoadFailure ) {
			arr[i] = enif_make_atom(env, "jackLoadFailure");
			o = JackLoadFailure;
		}else if(jackStatus & JackInitFailure ) {
			arr[i] = enif_make_atom(env, "jackInitFailure");
			o = JackInitFailure;
		}else if(jackStatus & JackShmFailure ) {
			arr[i] = enif_make_atom(env, "jackShmFailure");
			o = JackShmFailure;
		}else if(jackStatus & JackVersionError ) {
			arr[i] = enif_make_atom(env, "jackVersionError");
			o = JackVersionError;
		}else if(jackStatus & JackBackendError ) {
			arr[i] = enif_make_atom(env, "jackBackendError");
			o = JackBackendError;
		}else if(jackStatus & JackClientZombie ) {
			arr[i] = enif_make_atom(env, "jackClientZombie");
			o = JackClientZombie;
		}
		jackStatus -= o;
		continue;
	}
	return enif_make_list_from_array(
			env,
			arr,
			n
			);
}

search_results getJackClient(char client_name[100]){
	search_results sr = {
		&jack_clients,
		-1
	};

        for(int i=0; i < JACK_CLIENTS_COUNT; i++){

		if(jack_clients.client[i] == NULL ) continue;

                if( ! strcmp(jack_get_client_name(jack_clients.client[i]), client_name)){
			sr.i = i;
			break;
                }
        }
        return sr;
}

ERL_NIF_TERM client_open(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	
	// check is args are valid
	if( !enif_is_atom(env, argv[0]) || !enif_is_atom(env, argv[1]) || !enif_is_number(env, argv[2]) ) return enif_make_badarg(env);

        char client_name[MAX_CLIENT_NAME];
        char server_name[MAX_SERVER_NAME];
        unsigned int jackopts;

        enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
        enif_get_atom(env, argv[1], server_name,sizeof(server_name), ERL_NIF_LATIN1);
        if (!enif_get_int(env, argv[2], &jackopts)) enif_make_badarg(env);

	
	// create a jack struct object
        int i = JACK_CLIENTS_COUNT;

        jack_clients.options[i] = jackopts;
        jack_clients.port[i].in_chann = 0;
        jack_clients.port[i].out_chann = 0;

	// try to open the jack client
	
	void *void_args;
        jack_clients.client[i] = jack_client_open (
                        client_name, 
			jack_clients.options[i], 
			&jack_clients.status[i], 
			server_name);

	// callbacks
	jack_set_client_registration_callback(jack_clients.client[i], callback_JackClientRegistration, NULL);
	jack_set_port_connect_callback(jack_clients.client[i], callback_JackPortRegistrationCallback, NULL);
	jack_set_port_registration_callback();

	if(jack_clients.status[i] == 0){
		// everything is okay :)

		// register on jack_clients
		JACK_CLIENTS_COUNT++;
		
		return enif_make_tuple2(env, 
				enif_make_atom(env, "ok"),
				enif_make_tuple2(env, enif_make_atom(env, "jackStatus"), 
					get_jackStatus(env, jack_clients.status[i])
					)
				);
	}else if(jack_clients.status[i] == 4 ){
                JACK_CLIENTS_COUNT++;

		return enif_make_tuple3(
				env, 
				enif_make_atom(env, "ok"),
				enif_make_tuple2(
					env, 
					enif_make_atom(env, "jackStatus"), 
					get_jackStatus(env, jack_clients.status[i])
					),
				enif_make_tuple2(
					env,
					enif_make_atom(env, "jackClientRenamed"),
					enif_make_atom(
						env,
						jack_get_client_name(jack_clients.client[i])
						)
					)
				);
        }else{
                return enif_make_tuple2(env, enif_make_atom(env, "error"), 
                                enif_make_tuple2(
					env, 
					enif_make_atom(env, "jackStatus"), 
					get_jackStatus(env, jack_clients.status[i])
					)
				);
        }


}



ERL_NIF_TERM client_close(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
        return enif_make_atom(env, "ok");
}

ERL_NIF_TERM client_activate(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){

	if( !enif_is_atom(env, argv[0])) return enif_make_badarg(env);

        char client_name[MAX_CLIENT_NAME];

        enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
	
	search_results sr = getJackClient(client_name);

	if(sr.i < 0 ) {
                return enif_make_tuple2(env, 
				enif_make_atom(env, "error"), 
				enif_make_atom(env, "jackClientNotFound")
				);
	}
	if(jack_activate(sr.ptr->client[sr.i]) >= 0){
                return enif_make_tuple2(env, 
				enif_make_atom(env, "ok"), 
				enif_make_atom(env, "jackActivated")
				);
	}else{
                return enif_make_tuple2(env, 
				enif_make_atom(env, "error"), 
				enif_make_atom(env, "jackActivationFailed")
				);
	}
}

ERL_NIF_TERM client_deactivate(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
        return enif_make_atom(env, "ok");
}

ERL_NIF_TERM client_get(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
        return enif_make_atom(env, "ok");
}


