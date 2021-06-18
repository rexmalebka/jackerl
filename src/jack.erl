-module(jack).

-export([
	 client_open/1, client_open/2, client_open/3,
	 client_activate/1,
	 port_register/3, port_register_/3
	 %client_close/1,
	 %client_deactivate/1,
	 %client_get/2
	]).

-on_load(init/0).

init() ->
	{ok, Path} = file:get_cwd(),
	ok = erlang:load_nif(Path++"/priv/jackerl", 0).

% client functions
%
% client_open(whiteNoise).
% client_open(whiteNoise, 0).
% client_open(whiteNoise, {jacknullOption}).
% client_open({whiteNoise, default}, {jacknulloption}).
% client_open(whiteNoise, default, 0).


client_open(ClientName) when is_atom(ClientName)->
	client_open(ClientName, default, 0).


client_open(ClientName, JackOpts) when is_atom(ClientName) and is_number(JackOpts)->
	client_open(ClientName, default, JackOpts);

client_open(ClientName, JackOpts) when is_atom(ClientName) and is_atom(JackOpts)->
	client_open(ClientName, [JackOpts]);

client_open(ClientName, JackOptsList) when is_atom(ClientName) and is_list(JackOptsList)->
	JackOptsInt = lists:sum([
		       case lists:member(jackNullOption, JackOptsList) of true -> 0; false -> 0 end,
		       case lists:member(jackNoStartServer, JackOptsList) of true -> 1; false -> 0 end,
		       case lists:member(jackUseExactName, JackOptsList) of true -> 2; false -> 0 end,
		       case lists:member(jackServerName, JackOptsList) of true -> 4; false -> 0 end,
		       case lists:member(jackLoadName, JackOptsList) of true -> 8; false -> 0 end,
		       case lists:member(jackLoadInit, JackOptsList) of true -> 16; false -> 0 end,
		       case lists:member(jackSessionId, JackOptsList) of true -> 32; false -> 0 end
		      ]),
	client_open(ClientName, default, JackOptsInt).

client_open(_ClientName, _ServerName, _JackOpts) ->
	erlang:nif_error("NIF library not loaded").


client_close(_ClientName) ->
	erlang:nif_error("NIF library not loaded").

client_activate(_ClientName) ->
	erlang:nif_error("NIF library not loaded").

client_deactivate(_ClientName) ->
	erlang:nif_error("NIF library not loaded").

client_get(_ClientName, _Parameter)->
	erlang:nif_error("NIF library not loaded").

% port functions

port_register(ClientName, PortName, PortFlags) when is_atom(ClientName) and is_atom(PortName) and is_list(PortFlags)->
	PortFlagsInt = lists:sum([
		       case lists:member(jackPortIsInput, PortFlags) of true -> 1; false -> 0 end,
		       case lists:member(jackPortIsOutput, PortFlags) of true -> 2; false -> 0 end,
		       case lists:member(jackPortIsPhysical, PortFlags) of true -> 4; false -> 0 end,
		       case lists:member(jackPortCanMonitor, PortFlags) of true -> 8; false -> 0 end,
		       case lists:member(jackPortIsTerminal, PortFlags) of true -> 16; false -> 0 end
		      ]),
	port_register_(ClientName, PortName, PortFlagsInt).


port_register_(_ClientName,_PortName, _PortFlags) ->
	erlang:nif_error("NIF library not loaded").

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

