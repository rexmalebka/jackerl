-module(jack_server).
-behaviour(gen_server).

-export([start/0]).
-export([
	 init/1, 
	 callback/0,
	 handle_call/3, 
	 handle_cast/2,
	 handle_info/2,
         terminate/2]).



% what do I need to store ? 


start() ->
	gen_server:start_link(?MODULE, [], []).

init(_) ->
	State =	#{ 
	 process => nofun, 
	 shutdown => nofun, 
	 port_registration => nofun, 
	 clientRegistration => nofun 
	},
	{ok, State}.



handle_call({ X=client_registration, Callback}, _From, State) ->
	NewState = maps:update(X, Callback, State ),
	{reply, {ok, X, Callback }, NewState};

handle_call({ X=port_registration, Callback}, _From, State) ->
	NewState = maps:update(X, Callback, State ),
	{reply, {ok, X, Callback }, NewState};

%handle_call({ X=jackPortRegistrationCallback, Callback}, _From, State) 
%  when is_function(Callback, 3) ->
%	NewState = maps:update(X, Callback, State ),
%	{reply, {ok, X, Callback }, NewState};

handle_call(Request, _From, State) ->
	io:format("handle_call: ~p~n", [Request]),
	{reply, ok, State}.




handle_info(_Msg, State) ->
	io:format("handle_info: ~p~n", [_Msg]),
	{noreply, State}.





% cast are for nifs invokations

handle_cast({process}, State) ->
    	{noreply, State};

handle_cast({shutdown}, State) ->
    	{noreply, State};

handle_cast({X=port_registration, Client_name, Port_name, Status}, State) ->
	io:format("registered one ~p ~p ~p~n",[Client_name, Port_name, Status]),
	{noreply, State};

handle_cast({X=client_registration, Client_name, Status}, State) ->
	Callback = maps:get(X,State),
	case Callback of
		nofun ->
	      		{noreply, State};
		_ ->
			spawn(fun() ->
					      try Callback(Client_name,  Status) of
						      _ -> ok
					      catch
						      X -> error
					      end
			      end ),
	      		{noreply, State}
	end;

handle_cast(Msg, State) ->
	io:format("handle_cast: ~p~n", [Msg]),
    	{noreply, State}.

terminate(_Reason, _State) ->
	io:format("~p~n",["DED"]),
	ok.


callback() ->
	%erlang:send(whereis(?MODULE), adsfdsf),	
	%gen_server:call(whereis(?MODULE), {sdfsdfsdfsdf}).
	0.
