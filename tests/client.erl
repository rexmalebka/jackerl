-module(client).
-include_lib("eunit/include/eunit.hrl").

open_test() ->
	Callback = fun(C,R)-> io:format("uwu: ~p: ~p~n",[C,R]) end,
	jack:open(hola),
	jack:activate(hola),
	jack:callback(client, Callback),
	jack:register(hola, entrada, [input]),
	jack:unregister(hola, entrada).
