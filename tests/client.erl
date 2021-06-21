-module(client).
-include_lib("eunit/include/eunit.hrl").

open_close_client_test() ->
	?assert({ok, {client_name, hola}} =:= jack:open(hola)),
	?assert({ok, {client_name, hola}} =:= jack:close(hola)).

no_such_client_test()->
	?assert({error, {jack_status, [no_such_client]}} =:= jack:close(non_existing)).

no_such_port_test() ->
	jack:open(white_noise),
	?assert({error, {jack_status, [no_such_port]}} =:= jack:unregister(white_noise, non_existing)).

on_shutdown_test() ->
	jack:open(test),
	jack:register(test,a,input),
	jack:register(test,b,output),
	jack:activate(test),
	jack:close(test),

	jack:open(test2),
	jack:register(test2,a,input),
	jack:register(test2,b,output),
	jack:activate(test2),
	jack:close(test2).

