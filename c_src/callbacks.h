static void callback_client(const char *name, int reg, void *arg){
        enif_fprintf(stderr,"reg \n");
        enif_send(
                        NULL,
                        &erl_server.pid_client,
                        erl_server.env,
                        enif_make_tuple2(
                                erl_server.env,
                                enif_make_atom(erl_server.env, name),
                                enif_make_int( erl_server.env, reg) == 0 ? enif_make_atom( erl_server.env,"registered") : enif_make_atom( erl_server.env,"unregistered")
                                )
                 );

}

static void callback_port(  jack_port_id_t id, int reg, void * client){
        enif_fprintf(stderr,"port reg \n");
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
                                enif_make_int( erl_server.env, reg) == 0 ? enif_make_atom( erl_server.env,"registered") : enif_make_atom( erl_server.env,"unregistered")
                                )
                 );
}



