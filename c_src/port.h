int get_jack_port(int i, char *port_name, int portflag){
	
	int j = -1;
	if(portflag &1){
		for(int k=0; k< MAX_INPUT_CHANNELS; k++){
			if(jack_clients.port[i].input_port[k] == NULL) continue;
			if( !strcmp(jack_port_name(jack_clients.port[i].input_port[k] ), port_name ) ){
				j = k;
				break;
			}
		}
	}else{
		for(int k=0; k< MAX_INPUT_CHANNELS; k++){
			if(jack_clients.port[i].output_port[k] == NULL) continue;
			if( !strcmp(jack_port_name(jack_clients.port[i].output_port[k] ), port_name ) ){
				j = k;
				break;
			}
		}	
	}
	return j;
}



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
		if (!enif_get_int(env, argv[2], &portflags)) enif_make_badarg(env);
	}

	search_results sr = get_jack_client(client_name);

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

	const char *pname;
	
	char full_port_name[201];
	sprintf(full_port_name, "%s:%s", jack_get_client_name(sr.ptr->client[sr.i]), port_name );
	int jj = get_jack_port(sr.i, full_port_name, portflags);
	
	if( jj >=0 ){
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
	

	if(portflags & 1){
		//input port
		int j = 0;//sr.ptr->port[sr.i].in_chann;

		for(int k=0;k<MAX_PORT_NAME; k++){
			if(!jack_clients.port[sr.i].input_port[j]){
				j = k;
				break;
			}
		}

		sr.ptr->port[sr.i].input_port[j] = jack_port_register (
				sr.ptr->client[sr.i],
				port_name,
				JACK_DEFAULT_AUDIO_TYPE,
                                portflags,
				0
				);

                if(sr.ptr->port[sr.i].input_port[j] == NULL){
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

		
                pname = jack_port_short_name(sr.ptr->port[sr.i].input_port[j]);
		sr.ptr->port[sr.i].in_chann++;

        }else if(portflags & 2){
		
		int j = 0;//sr.ptr->port[sr.i].in_chann;

		for(int k=0;k<MAX_PORT_NAME; k++){
			if(!jack_clients.port[sr.i].output_port[j]){
				j = k;
				break;
			}
		}
                
		sr.ptr->port[sr.i].output_port[j] = jack_port_register (
				sr.ptr->client[sr.i],
				port_name,
				JACK_DEFAULT_AUDIO_TYPE,
				portflags,
				0
				);

                if(sr.ptr->port[sr.i].output_port[j] == NULL){
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

                pname = jack_port_short_name(sr.ptr->port[sr.i].output_port[j]);
		sr.ptr->port[sr.i].out_chann++;
        }
	
	return enif_make_tuple2(
			env,
			enif_make_atom(env, "ok"), 
			enif_make_tuple2(
				env,
				enif_make_atom(env, "port_name"),
				enif_make_atom(env, pname)
				)
			);
}

ERL_NIF_TERM port_unregister(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	// check params
	if( !enif_is_atom(env, argv[0]) || 
			!enif_is_atom(env, argv[1]) ) return enif_make_badarg(env);

        // get client_name, server name and jack options
        char client_name[100];
        char port_name[100];

        enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
        enif_get_atom(env, argv[1], port_name,sizeof(port_name), ERL_NIF_LATIN1);

	search_results sr = get_jack_client(client_name);

	// check if client exists
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
	
	// get full name to retrieve information
	
	char full_port_name[201];
	sprintf(full_port_name, "%s:%s", jack_get_client_name(sr.ptr->client[sr.i]), port_name );


	// get port 
	jack_port_t *port = jack_port_by_name(sr.ptr->client[sr.i], full_port_name);


	if(port == NULL ){
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
	

	int portflags = jack_port_flags(port);

	int j = get_jack_port(sr.i, full_port_name, portflags);

	if( j < 0 ){
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

	if(portflags & 1){
		// if port is  input



                if(jack_port_unregister(sr.ptr->client[sr.i], sr.ptr->port[sr.i].input_port[j] )){
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

		
		sr.ptr->port[sr.i].input_port[j] = NULL;
		sr.ptr->port[sr.i].in_chann--;
        }else if(portflags & 2){
                // if port is output
		
		int j = get_jack_port(sr.i, full_port_name, portflags);

                if(jack_port_unregister(sr.ptr->client[sr.i], sr.ptr->port[sr.i].output_port[j] )){
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
		
		sr.ptr->port[sr.i].output_port[j] = NULL;
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



