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

	search_results sr = getJackClient(client_name);

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
	
	if(portflags & 1){
		//input port
		int j = sr.ptr->port[sr.i].in_chann;

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
                int j = sr.ptr->port[sr.i].out_chann;
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

	search_results sr = getJackClient(client_name);

	// check if user exists
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

	char full_port_name[201];
	sprintf(full_port_name, "%s:%s", jack_get_client_name(sr.ptr->client[sr.i]), port_name );

	const jack_port_t *port = jack_port_by_name(sr.ptr->client[sr.i], full_port_name);
	if(port == NULL){
		enif_fprintf(stderr,":%s~n",port_name);
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

	if(portflags & 1){
		//input port
		int j = sr.ptr->port[sr.i].in_chann;

                if(!jack_port_unregister(sr.ptr->client[sr.i], port )){
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

		
		sr.ptr->port[sr.i].in_chann--;

        }else if(portflags & 2){
                int j = sr.ptr->port[sr.i].out_chann;
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
