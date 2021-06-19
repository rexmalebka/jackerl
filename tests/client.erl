-module(client).
-include_lib("eunit/include/eunit.hrl").

open_test() ->
	Callback = fun(C,R)-> io:format("~p: ~p~n",[C,R]) end,
	jack:open(hola),
	jack:callback(jackClientRegistrationCallback, Callback),
	jack:register_(hola, entrada, 1),
	jack:activate(hola),
	ok.

