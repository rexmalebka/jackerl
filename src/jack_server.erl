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
	gen_server:start_link({global,?MODULE},?MODULE, [], []).

init(_) ->
	State =	#{ 
	 jackProcessCallback => nofun, 
	 jackShutdownCallback => nofun, 
	 jackPortRegistrationCallback => nofun, 
	 jackClientRegistrationCallback => nofun 
	},
	{ok, State}.



handle_call({ X=jackClientRegistrationCallback, Callback}, _From, State) ->
	NewState = maps:update(X, Callback, State ),
	{reply, {ok, X, Callback }, NewState};

handle_call({ X=jackPortRegistrationCallback, Callback}, _From, State) ->
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

handle_cast({jackProcessCallback}, State) ->
    	{noreply, State};

handle_cast({jackShutdownCallback}, State) ->
    	{noreply, State};

handle_cast({X=jackPortRegistrationCallback, ClientName, PortName, 0}, State) ->
	io:format("registered one",[]),
	Callback = maps:get(X,State),
	case Callback of
		nofun ->
	      		{noreply, State};
		_ ->
			spawn(fun() ->
					      try Callback(ClientName, PortName,  unregistered) of
						      _ -> ok
					      catch
						      X -> error
					      end
			      end ),
	      		{noreply, State}
	end;

handle_cast({X=jackPortRegistrationCallback, ClientName, PortName, Y}, State) when is_number(Y) ->
	Callback = maps:get(X,State),
	case Callback of
		nofun ->
	      		{noreply, State};
		_ ->
			spawn(fun() ->
					      try Callback(ClientName, PortName,  registered) of
						      _ -> ok
					      catch
						      X -> error
					      end
			      end ),
	      		{noreply, State}
	end;


handle_cast({X=jackClientRegistrationCallback, ClientName, 0}, State) ->
	Callback = maps:get(X,State),
	case Callback of
		nofun ->
	      		{noreply, State};
		_ ->
			spawn(fun() ->
					      try Callback(ClientName,  unregistered) of
						      _ -> ok
					      catch
						      X -> error
					      end
			      end ),
	      		{noreply, State}
	end;

handle_cast({X=jackClientRegistrationCallback, ClientName, Y}, State) when is_number(Y) ->
	Callback = maps:get(X,State),
	case Callback of
		nofun ->
	      		{noreply, State};
		_ ->
			spawn(fun() ->
					      try Callback(ClientName,  registered) of
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
