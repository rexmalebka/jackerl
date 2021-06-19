static void callback_JackClientRegistration(const char *name, int reg, void *arg){
        enif_fprintf(stderr,"reg \n");
        enif_send(
                        NULL,
                        &erl_server.pid,
                        erl_server.env,
                        enif_make_tuple3(
                                erl_server.env,
                                enif_make_atom( erl_server.env,"jackClientRegistrationCallback"),
                                enif_make_atom(erl_server.env, name),
                                enif_make_int( erl_server.env, reg)
                                )
                 );

}

static void callback_JackPortRegistrationCallback(  jack_port_id_t *id, int reg, void* arg){
        enif_fprintf(stderr,"port reg \n");

	char *name = "potazio";
        enif_send(
                        NULL,
                        &erl_server.pid,
                        erl_server.env,
                        enif_make_tuple3(
                                erl_server.env,
                                enif_make_atom( erl_server.env,"JackPortRegistrationCallback"),
                                enif_make_atom(erl_server.env, name),
                                enif_make_int( erl_server.env, reg)
                                )
                 );

}



