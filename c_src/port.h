/*
int getPortFlags(ErlNifEnv* env, ERL_NIF_TERM term){
	unsigned int *len;
	int p=0;

	ERL_NIF_TERM* H;
	ERL_NIF_TERM* T;

	enif_fprintf(stderr, "adsf adsfsdfdsf\n");
	while(enif_get_list_cell(env, term, H, T)){
		enif_fprintf(stderr, "miaumiau \n");
	
		char a[MAX_PORT_NAME];
		
		if(!enif_get_atom(env, *H, a, sizeof(a), ERL_NIF_LATIN1)) continue;
		enif_fprintf(stderr, "adsf -%s- \n", a);
		
		if( strcmp(a, "jackPortIsInput")) p |= 1;
		if( strcmp(a, "jackPortIsOutput")) p |= 2;
		if( strcmp(a, "jackPortIsPhysical")) p |= 4;
		if( strcmp(a, "jackPortCanMonitor")) p |= 8;
		if( strcmp(a, "jackPortIsTerminal")) p |= 16;
		term = *T;
	}
	return p;
}
*/
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
                                enif_make_atom(env, "jackClientNotFound")
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
                                        enif_make_atom(env, "portsNotAvailable")
					);
                }

		
                pname = jack_port_short_name(sr.ptr->port[sr.i].input_port[j]);
		sr.ptr->port[sr.i].in_chann++;
		enif_fprintf(stderr,"is input %d\n", sr.ptr->port[sr.i].in_chann);

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
                                        enif_make_atom(env, "portsNotAvailable")
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
				enif_make_atom(env, "portName"),
				enif_make_atom(env, pname)
				)
			);
}
