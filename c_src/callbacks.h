static void callback_client(const char *name, int reg, void *arg){
        enif_fprintf(stderr,"reg \n");
        enif_send(
                        NULL,
                        &erl_server.pid_client,
                        erl_server.env,
                        enif_make_tuple2(
                                erl_server.env,
                                enif_make_atom(erl_server.env, name),
                                enif_make_atom(erl_server.env, enif_make_int( erl_server.env, reg) == 0 ? "unregistered" : "registered")
                                )
                 );

}

static void callback_port(  jack_port_id_t id, int reg, void * client){
	jack_port_t *port = jack_port_by_id(client, id);
	const char * name = jack_port_short_name(port);

        enif_send(
                        NULL,
                        &erl_server.pid_port,
                        erl_server.env,
                        enif_make_tuple3(
                                erl_server.env,
                                enif_make_atom(erl_server.env, jack_get_client_name(client)),
                                enif_make_atom(erl_server.env, name),
                                enif_make_atom(erl_server.env, enif_make_int( erl_server.env, reg) == 0 ? "unregistered" : "registered")
                                )
                 );
}

static void callback_shutdown(void *arg){
        enif_send(
                        NULL,
                        &erl_server.pid_shutdown,
                        erl_server.env,
                        enif_make_tuple1(
                                erl_server.env,
                                enif_make_atom(erl_server.env, "shutdown")
                                )
                 );
}

int callback_process(jack_nframes_t nframes, void *data){

	//enif_fprintf(stderr, "asdfsdf %s-\n", jack_get_client_name(jack_clients.client[(int)((&data)[0])]) );
	//enif_fprintf(stderr, "shortname: %s\n", jack_port_short_name( jack_clients.port[(int)((&data)[0])].input_port[0] ));
	//




	char s_name [101];
	sprintf(s_name, "%s:.*", jack_get_client_name( jack_clients.client[(int)((&data)[0])]) );

	const char** ports = jack_get_ports(jack_clients.client[(int)((&data)[0])], (s_name), NULL,1);

	int len=0;
	while(ports[len]){
		len++;
	}

	ERL_NIF_TERM in_arr[len];


	for(int i=0;i< len; i++ ){
		jack_port_t *port = jack_port_by_name(jack_clients.client[(int)((&data)[0])], ports[i] );

		in_arr[i] = enif_make_double(
			erl_server.env,
			*((double *) jack_port_get_buffer(  port, (int)nframes ))
			);
	}


	ERL_NIF_TERM input_data = enif_make_list_from_array(
			erl_server.env,
			in_arr,
			len
			);
	

		enif_send(
				NULL,
				&erl_server.pid_process,
				erl_server.env,
				enif_make_tuple2(
					erl_server.env,
					enif_make_atom(erl_server.env, "process"),
					input_data
					)
			 );
	return 0;
}


/*
ERL_NIF_TERM set_buffer(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	 if( !enif_is_atom(env, argv[0]) || !enif_is_list(env, argv[1]))  return enif_make_badarg(env);
	 char client_name[MAX_CLIENT_NAME];

	 enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
	 
	 unsigned int len;
	 enif_get_list_length(env, argv[1], &len);
	 enif_fprintf(stderr,"s:%d\n", &len);
	 return enif_make_atom(env, "ok");
}
*/
