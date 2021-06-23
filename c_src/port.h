
ERL_NIF_TERM port_register(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	if( !enif_is_atom(env, argv[0]) || 
			!enif_is_atom(env, argv[1]) || 
			!(enif_is_number(env, argv[2]) || enif_is_list(env, argv[2])) ) return enif_make_badarg(env);

        // get client_name, server name and jack options
        char client_name[100];
        char port_name[100];
        int portflags;

        enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
        enif_get_atom(env, argv[1], port_name,sizeof(port_name), ERL_NIF_LATIN1);


	if(enif_is_number(env, argv[2])){
		// if jack options are not an integer or are 0 fail
		if (!enif_get_int(env, argv[2], &portflags)) enif_make_badarg(env);
		if (portflags <= 0) enif_make_badarg(env);
	}

	search_results sr = get_jack_client(client_name);


	// check if client exists to proceed
        if(sr.i < 0) {
		return enif_make_tuple2(env,
				enif_make_atom(env, "error"),
				enif_make_tuple2(
                                        env,
                                        enif_make_atom(env, "jack_status"),
                                        enif_make_list1(
                                                env,
                                                enif_make_atom(
                                                        env,
                                                        "no_such_client"
                                                        )
                                                )
                                        )
                                );
        }

	// check if port already exists
	

	char full_port_name[201];
	sprintf(full_port_name, "^%s:%s$", jack_get_client_name(sr.ptr->client[sr.i]), port_name );

	const char** ports = jack_get_ports( sr.ptr->client[sr.i], full_port_name, NULL,0);


	if(ports != NULL){
		// port does exists

		return enif_make_tuple2(env,
				enif_make_atom(env, "error"),
				enif_make_tuple2(
                                        env,
                                        enif_make_atom(env, "jack_status"),
                                        enif_make_list1(
                                                env,
                                                enif_make_atom(
                                                        env,
                                                        "port_already_exists"
                                                        )
                                                )
                                        )
                                );
	}
	

	if(jack_port_register (
			sr.ptr->client[sr.i],
			port_name,
			JACK_DEFAULT_AUDIO_TYPE,
			portflags,
			0
			) == NULL){
		return enif_make_tuple2(
				env,
				enif_make_atom(env, "error"), 
				enif_make_tuple2(
					env,
					enif_make_atom(env, "jack_status"),
					get_jackStatus(env, jack_clients.status[sr.i])
					)
				);
	}

	
	if(portflags & 1){
		sr.ptr->port[sr.i].in_chann++;
        }else if(portflags & 2){
		sr.ptr->port[sr.i].out_chann++;
        }
	
	return enif_make_tuple2(
			env,
			enif_make_atom(env, "ok"), 
			enif_make_tuple2(
				env,
				enif_make_atom(env, "port_name"),
				enif_make_atom(env, port_name)
				)
			);
}




ERL_NIF_TERM port_unregister(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	if( !enif_is_atom(env, argv[0]) || 
			!enif_is_atom(env, argv[1])  ) return enif_make_badarg(env);

        // get client_name, server name and jack options
        char client_name[100];
        char port_name[100];

        enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
        enif_get_atom(env, argv[1], port_name,sizeof(port_name), ERL_NIF_LATIN1);


	search_results sr = get_jack_client(client_name);




	// check if client exists to proceed
        if(sr.i < 0) {
		return enif_make_tuple2(env,
				enif_make_atom(env, "error"),
				enif_make_tuple2(
                                        env,
                                        enif_make_atom(env, "jack_status"),
                                        enif_make_list1(
                                                env,
                                                enif_make_atom(
                                                        env,
                                                        "no_such_client"
                                                        )
                                                )
                                        )
                                );
        }

	// check if port already exists
	

	char full_port_name[201];
	sprintf(full_port_name, "^%s:%s$", jack_get_client_name(sr.ptr->client[sr.i]), port_name );

	const char** ports = jack_get_ports( sr.ptr->client[sr.i], full_port_name, NULL,0);


	if(ports == NULL){
		// port does not exists

		return enif_make_tuple2(env,
				enif_make_atom(env, "error"),
				enif_make_tuple2(
                                        env,
                                        enif_make_atom(env, "jack_status"),
                                        enif_make_list1(
                                                env,
                                                enif_make_atom(
                                                        env,
                                                        "no_such_port"
                                                        )
                                                )
                                        )
                                );
	}
	
	jack_port_t *port = jack_port_by_name(sr.ptr->client[sr.i], ports[0] );
	int portflags = jack_port_flags(port);
	

	if(jack_port_unregister (
			sr.ptr->client[sr.i],
			port
			)){

		return enif_make_tuple2(
				env,
				enif_make_atom(env, "error"), 
				enif_make_tuple2(
					env,
					enif_make_atom(env, "jack_status"),
					get_jackStatus(env, jack_clients.status[sr.i])
					)
				);
	}

	
	if(portflags & 1){
		sr.ptr->port[sr.i].in_chann--;
        }else if(portflags & 2){
		sr.ptr->port[sr.i].out_chann--;
        }

	
	return enif_make_tuple2(
			env,
			enif_make_atom(env, "ok"), 
			enif_make_tuple2(
				env,
				enif_make_atom(env, "port_name"),
				enif_make_atom(env, port_name)
				)
			);
}
