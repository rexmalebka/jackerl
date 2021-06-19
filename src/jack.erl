-module(jack).

-export([
	 init/0,
	 open/1, open/2, open/3,
	 activate/1,
	 pregister/3, register_/3,
	 callback/2,
	 close/1,
	 deactivate/1,
	 client_get/2
	]).

-on_load(init/0).

init() ->
	{ok, Pid} = jack_server:start(),
	{ok, Path} = file:get_cwd(),
	ok = erlang:load_nif(Path++"/priv/jackerl", Pid).



%%% open a client

open(ClientName) when is_atom(ClientName)->
	open(ClientName, default, 0).


open(ClientName, JackOpts) when is_atom(ClientName) and is_number(JackOpts)->
	open(ClientName, default, JackOpts);

open(ClientName, JackOpts) when is_atom(ClientName) and is_atom(JackOpts)->
	open(ClientName, [JackOpts]);

open(ClientName, JackOptsList) when is_atom(ClientName) and is_list(JackOptsList)->
	JackOptsInt = lists:sum([
		       case lists:member(jackNullOption, JackOptsList) of true -> 0; false -> 0 end,
		       case lists:member(jackNoStartServer, JackOptsList) of true -> 1; false -> 0 end,
		       case lists:member(jackUseExactName, JackOptsList) of true -> 2; false -> 0 end,
		       case lists:member(jackServerName, JackOptsList) of true -> 4; false -> 0 end,
		       case lists:member(jackLoadName, JackOptsList) of true -> 8; false -> 0 end,
		       case lists:member(jackLoadInit, JackOptsList) of true -> 16; false -> 0 end,
		       case lists:member(jackSessionId, JackOptsList) of true -> 32; false -> 0 end
		      ]),
	open(ClientName, default, JackOptsInt).

open(_ClientName, _ServerName, _JackOpts) ->
	erlang:nif_error("NIF library not loaded").


%%% close a client

close(_ClientName) ->
	erlang:nif_error("NIF library not loaded").


%%% activate a client

activate(_ClientName) ->
	erlang:nif_error("NIF library not loaded").

%%% deactivate a client

deactivate(_ClientName) ->
	erlang:nif_error("NIF library not loaded").

%%% get client information

client_get(_ClientName, _Parameter)->
	erlang:nif_error("NIF library not loaded").



% port functions

%%% register a port

pregister(ClientName, PortName, PortFlags) when is_atom(ClientName) and is_atom(PortName) and is_list(PortFlags)->
	PortFlagsInt = lists:sum([
		       case lists:member(jackPortIsInput, PortFlags) of true -> 1; false -> 0 end,
		       case lists:member(jackPortIsOutput, PortFlags) of true -> 2; false -> 0 end,
		       case lists:member(jackPortIsPhysical, PortFlags) of true -> 4; false -> 0 end,
		       case lists:member(jackPortCanMonitor, PortFlags) of true -> 8; false -> 0 end,
		       case lists:member(jackPortIsTerminal, PortFlags) of true -> 16; false -> 0 end
		      ]),
	register_(ClientName, PortName, PortFlagsInt).


register_(_ClientName,_PortName, _PortFlags) ->
	erlang:nif_error("NIF library not loaded").

callback(CallbackName, Callback ) ->
	CFun = fun()->
			       receive _ ->
					       io:format("spawned???",[])
			       end
	       end,
	PID = spawn(CFun),
	erlang:register(CallbackName, PID),
	ok.



%port_unregister(_ClientName, _PortName) ->
%	erlang:nif_error("NIF library not loaded").

%port_rename(_ClientName, _PortName, _PortNameNew) ->
%	erlang:nif_error("NIF library not loaded").



%ports_connect(_ClientName, _PortName1, _PortName2) ->
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

