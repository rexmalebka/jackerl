-module(jack_proc).

-export([start/0]).
-export([
	 init/0,
	 callback/1
	]).

start()->
	PID=spawn(?MODULE, init,[]),
	PID.

init() ->
	State = #{
         jackProcessCallback => nofun,
         jackShutdownCallback => nofun,
         jackPortRegistrationCallback => nofun,
         jackClientRegistrationCallback => nofun
        },
	loop(State).

loop(State) ->
	receive 
		X ->
			io:format("~n---~p----~n",[X]),
			loop(State)
	end.


callback(CName)->
	io:format("----~p~n--",[ lists:member(CName, registered())]),
	erlang:send(CName, sdf),
	ok.
