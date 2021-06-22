-module(jack).

-export([ init/0]).
-export([
	 open/1, open/2, open/3,
	 close/1, close/2,
	 activate/1,
	 deactivate/1
	]).
-export([
	 register/3, register_/3,
	 unregister/2,
	 client_get/2
	]).
-export([callback/2]).

-on_load(init/0).

init() ->
	Callback_client = fun (Client_name, Status)-> io:format("[ client ] ~p is ~p ~n",[Client_name, Status]) end,
	Callback_port = fun (Client_name,Port_name, Status)-> io:format("[ port ] ~p on ~p is ~p~n",[Port_name,Client_name, Status]) end,
	Callback_shutdown = fun() -> io:format("[ shutdown ]",[]) end,
	Callback_process = fun() -> io:format("[ process ]",[]) end,

	Pid_c_client = spawn(fun() -> callback_client(Callback_client) end),
	Pid_c_port = spawn(fun() -> callback_port(Callback_port) end),
	Pid_c_shutdown = spawn(fun() -> callback_shutdown(Callback_shutdown) end),
	Pid_c_process = spawn(fun() -> callback_process(Callback_shutdown) end),

	erlang:register(callback_client, Pid_c_client),
	erlang:register(callback_port, Pid_c_port),
	erlang:register(callback_shutdown, Pid_c_shutdown),
	erlang:register(callback_process, Pid_c_process),

	{ok, Path} = file:get_cwd(),
	ok = erlang:load_nif(Path++"/priv/jackerl", {Pid_c_client, Pid_c_port, Pid_c_shutdown, Pid_c_process}).


%%%%%% client functions  %%%%%

%%% open a client

open(Client_name) when is_atom(Client_name)->
	open(Client_name, default, 0).

open(Client_name, Jack_option_int) when is_atom(Client_name) and is_number(Jack_option_int)->
	open(Client_name, default, Jack_option_int);

open(Client_name, Jack_option) when is_atom(Client_name) and is_atom(Jack_option)->
	open(Client_name, [Jack_option]);

open(Client_name, Jack_options) when is_atom(Client_name) and is_list(Jack_options)->
	Jack_option_int = lists:sum([
		       case lists:member(null, Jack_options) of true -> 0; false -> 0 end,
		       case lists:member(no_start_server, Jack_options) of true -> 1; false -> 0 end,
		       case lists:member(exact_name, Jack_options) of true -> 2; false -> 0 end,
		       case lists:member(server_name , Jack_options) of true -> 4; false -> 0 end,
		       case lists:member(load_name, Jack_options) of true -> 8; false -> 0 end,
		       case lists:member(load_init, Jack_options) of true -> 16; false -> 0 end,
		       case lists:member(session_id , Jack_options) of true -> 32; false -> 0 end
		      ]),
	open(Client_name, default, Jack_option_int).

open(_Client_name, _ServerName, _Jack_option_int) ->
	% NIF implementation
	erlang:nif_error("NIF library not loaded").


%%% close a client

close(Client_name) ->
	close(Client_name, default).
close(_Client_name, _Server) ->
	% NIF implementation
	erlang:nif_error("NIF library not loaded").


%%% activate a client

activate(_Client_name) ->
	% NIF implementation
	erlang:nif_error("NIF library not loaded").

%%% deactivate a client

deactivate(_Client_name) ->
	% NIF implementation
	erlang:nif_error("NIF library not loaded").

%%% get client information

client_get(_Client_name, _Parameter)->
	% NIF implementation
	erlang:nif_error("NIF library not loaded").



%%%%%% port functions  %%%%%

%%% register a port

register(Client_name, Port_name, Port_flag) when is_atom(Client_name) and is_atom(Port_name) and is_atom(Port_flag)->
	register(Client_name,Port_name, [Port_flag]);

register(Client_name, Port_name, Port_flags) when is_atom(Client_name) and is_atom(Port_name) and is_list(Port_flags)->
	Port_flags_int = lists:sum([
		       case lists:member(input, Port_flags) of true -> 1; false -> 0 end,
		       case lists:member(output, Port_flags) of true -> 2; false -> 0 end,
		       case lists:member(physical, Port_flags) of true -> 4; false -> 0 end,
		       case lists:member(monitor, Port_flags) of true -> 8; false -> 0 end,
		       case lists:member(terminal, Port_flags) of true -> 16; false -> 0 end
		      ]),
	register_(Client_name, Port_name, Port_flags_int).

register_(_Client_name,_Port_name, _Port_flags) ->
	% NIF implementation
	erlang:nif_error("NIF library not loaded").

unregister(_Client_name,_Port_name) ->
	% NIF implementation
	erlang:nif_error("NIF library not loaded").


%%%%%% concurrency function  %%%%%

%%% 

set_buffer(_Client_name, _Buffer) ->
	erlang:nif_error("NIF library not loaded").

%%%%%% callback functions  %%%%%


callback_shutdown(Callback) ->
	receive 
		{set,New_callback} when is_function(Callback,0) ->
			callback_shutdown(New_callback);
		{shutdown}->
			spawn(
			  fun() ->
					  try Callback() of
						  _ -> ok
					  catch
						  _ -> error
					  end
			  end),
			callback_shutdown(Callback);
		_ -> 
			callback_shutdown(Callback)
	end.

callback_process(Callback) ->
	receive 
		{set,New_callback} when is_function(Callback,2) ->
			callback_client(New_callback);
		{Client_name, registered} when is_atom(Client_name) ->
			spawn(
			  fun() ->
					  try Callback(Client_name,  registered) of
						  _ -> ok
					  catch
						  _ -> error
					  end
			  end),
			callback_process(Callback);
		{Client_name, unregistered} when is_atom(Client_name) ->
			spawn(
			  fun() ->
					  try Callback(Client_name,  unregistered) of
						  _ -> ok
					  catch
						  _ -> error
					  end
			  end),
			callback_process(Callback);
		X -> 
			io:format("----~p~n",[X]),
			callback_process(Callback)
	end.

%%% client callback

callback_client(Callback) -> 
	receive 
		{set,New_callback} when is_function(Callback,2) ->
			callback_client(New_callback);
		{Client_name, registered} when is_atom(Client_name) ->
			spawn(
			  fun() ->
					  try Callback(Client_name,  registered) of
						  _ -> ok
					  catch
						  _ -> error
					  end
			  end),
			callback_client(Callback);
		{Client_name, unregistered} when is_atom(Client_name) ->
			spawn(
			  fun() ->
					  try Callback(Client_name,  unregistered) of
						  _ -> ok
					  catch
						  _ -> error
					  end
			  end),
			callback_client(Callback);
		X -> 
			io:format("------~p~n",[X]),
			callback_client(Callback)
	end.


%%% port callback

callback_port(Callback) -> 
	receive 
		{set,New_callback} when is_function(New_callback,3) ->
			callback_client(New_callback);
		{Client_name, Port_name, registered} when is_atom(Client_name) and is_atom(Port_name) ->
			spawn(
			  fun() ->
					  try Callback(Client_name, Port_name,registered) of
						  _ -> ok
					  catch
						  _ -> error
					  end
			  end),
			callback_client(Callback);
		{Client_name, Port_name, unregistered} when is_atom(Client_name) and is_atom(Port_name) ->
			spawn(
			  fun() ->
					  try Callback(Client_name, Port_name,unregistered) of
						  _ -> ok
					  catch
						  _ -> error
					  end
			  end),
			callback_client(Callback);
		X ->
			io:format("~p~n",[X]),
			callback_client(Callback)
	end.

%%% callback implementation

callback(client, Callback) when is_function(Callback, 2)->
	callback_client ! {set, Callback};
callback(port, Callback) when is_function(Callback, 3)->
	callback_port ! {set, Callback};
callback(shutdown, Callback) when is_function(Callback, 2)->
	callback_shutdown ! {set, Callback}.





%port_rename(_Client_name, _Port_name, _Port_nameNew) ->
%	erlang:nif_error("NIF library not loaded").



%ports_connect(_Client_name, _Port_name1, _Port_name2) ->
%	erlang:nif_error("NIF library not loaded").

%ports_disconnect() ->
%	erlang:nif_error("NIF library not loaded").

%port_get() ->
%	erlang:nif_error("NIF library not loaded").



% server functions
%
%server_create() ->
%	erlang:nif_error("NIF library not loaded").

%server_destroy() ->
%	erlang:nif_error("NIF library not loaded").

%server_start() ->
%	erlang:nif_error("NIF library not loaded").

%server_stop() ->
%	erlang:nif_error("NIF library not loaded").

%server_get()->
%	erlang:nif_error("NIF library not loaded").

%server_set()->
%	erlang:nif_error("NIF library not loaded").


% frames

%frames_get() ->
%	erlang:nif_error("NIF library not loaded").

% midi 

%midi_get() ->
%	erlang:nif_error("NIF library not loaded").

% callback

%callback_info() ->
%	erlang:nif_error("NIF library not loaded").

%callback_shutdown()->
%	erlang:nif_error("NIF library not loaded").

%callback_process()->
%	erlang:nif_error("NIF library not loaded").

%callback_bsize() ->
%	erlang:nif_error("NIF library not loaded").

%callback_srate()->
%	erlang:nif_error("NIF library not loaded").

%callback_client() ->
%	erlang:nif_error("NIF library not loaded").

%callback_port() ->
%	erlang:nif_error("NIF library not loaded").

%callback_latency()->
%	erlang:nif_error("NIF library not loaded").

%callback_xrun()->
%	erlang:nif_error("NIF library not loaded").

