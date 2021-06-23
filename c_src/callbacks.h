#include <unistd.h>


static void callback_client(const char *name, int reg, void *arg){
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

	// get client name 
	char s_name [101];
	sprintf(s_name, "%s:.*", jack_get_client_name( jack_clients.client[(int)((&data)[0])]) );

	// get input ports values

	const char** input_ports = jack_get_ports(jack_clients.client[(int)((&data)[0])], (s_name), NULL,1);

	int inp_len=0;
	while(input_ports[inp_len]){
		inp_len++;
	}

	// create an array to send it to user side
	ERL_NIF_TERM in_arr[inp_len];


	for(int i=0;i< inp_len; i++ ){
		jack_port_t *port = jack_port_by_name(jack_clients.client[(int)((&data)[0])], input_ports[i] );

		in_arr[i] = enif_make_double(
			erl_server.env,
			*((double *) jack_port_get_buffer(  port, (int)nframes ))
			);
	}



	ERL_NIF_TERM input_data = enif_make_list_from_array(
			erl_server.env,
			in_arr,
			inp_len
			);
	// send to pid
	enif_send(
			NULL,
			&erl_server.pid_process,
			erl_server.env,
			enif_make_tuple3(
				erl_server.env,
				enif_make_atom(erl_server.env, "process"),
				enif_make_atom(erl_server.env, jack_get_client_name( jack_clients.client[(int)((&data)[0])]) ),
				input_data
				)
		 );



	// write into output
		
	// get output ports
	const char** output_ports = jack_get_ports(jack_clients.client[(int)((&data)[0])], (s_name), NULL,2);
	

	// if no outputs provided
	if(output_ports == NULL) return 0;
	
	int out_len = 0;
	while(output_ports[out_len]){
		out_len++;
	}

		
	for(int i=0;i<out_len;i++){
		jack_port_t *port = jack_port_by_name(jack_clients.client[(int)((&data)[0])], output_ports[i] );
		jack_default_audio_sample_t *out = jack_port_get_buffer (port, nframes);
		memcpy (out, &jack_clients.port[(int)((&data)[0])].output[i],
				sizeof (jack_default_audio_sample_t) * nframes);

	}
	



	return 0;
}


ERL_NIF_TERM set_buffer(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	// sets buffer on output ports
	if( !enif_is_atom(env, argv[0]) || !enif_is_list(env, argv[1]))  return enif_make_badarg(env);
	
	char client_name[MAX_CLIENT_NAME];
	
	enif_get_atom(env, argv[0], client_name,sizeof(client_name), ERL_NIF_LATIN1);
		
	// check if client exists
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
	
	char full_port_name[201];
	sprintf(full_port_name, "%s:.*", client_name);
	
	// get length of buffer array
	unsigned int len_buffer;
	enif_get_list_length(env, argv[1], &len_buffer);

	
	// get output ports
	const char **ports = jack_get_ports(sr.ptr->client[sr.i], full_port_name,NULL, 2);
	
	// if no outputs provided
	if(ports == NULL){
		return enif_make_tuple2(env, 
				enif_make_atom(env, "error"),
				enif_make_tuple2(
					env, 
					enif_make_atom(env, "jack_status"), 
					enif_make_list1(
						env,
						enif_make_atom(
							env,
							"no_such_ports"
							)               
						)
					)
				);
	}

	int len_ports=0;
	while(ports[len_ports]){
		len_ports++;
	}


	// output array
	double output[len_ports];

	ERL_NIF_TERM list = argv[1];
	ERL_NIF_TERM H;
	ERL_NIF_TERM T;

	for(int i=0; i< len_ports;i++){
		if(i>=len_buffer){
			output[i] = 0;
			continue;
		}
		enif_get_list_cell(env, list, &H, &T);
		
		double d;
		if(enif_get_double(env, H, &d)){
			output[i] = d;
		}else{
			output[i] = 0.0;
		}
		list = T;
	}


	
	for(int i=0;i<MAX_OUTPUT_CHANNELS;i++){
		if(i>len_ports){
			break;
		}
		sr.ptr->port[sr.i].output[i] = output[i];
	}

	return enif_make_atom(env, "ok");
}
