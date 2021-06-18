jack
=====

An OTP library

Build
-----

    $ rebar3 compile

# Jack API docs

https://jackaudio.org/api/

## Port functions

- register
- unregister

*get*
- port_name
- uuid
- port_uuid
- short_name
- flags
- type
- connections
- latency
- latency range

- connect
- disconnect 
- rename



## client functoins

- open
- close
- activate
- deactivate

*get*
- uuid
- name

## server functions

*get*
- buffer size


## frames

- frames_to_time 
- time_to_frames
*get*
- frames_since_cycle_start
- frames_time
- time


## midi

*get*

## callbacks
- shutdown
- info
- process
- freewhell
- buffer_size
- sample_rate
- client
 port
- latency
- xrun


```
int 	jack_set_thread_init_callback (jack_client_t *client, JackThreadInitCallback thread_init_callback, void *arg) JACK_OPTIONAL_WEAK_EXPORT

int 	jack_set_port_registration_callback (jack_client_t *, JackPortRegistrationCallback registration_callback, void *arg) JACK_OPTIONAL_WEAK_EXPORT
 
int 	jack_set_port_rename_callback (jack_client_t *, JackPortRenameCallback rename_callback, void *arg) JACK_OPTIONAL_WEAK_EXPORT
 
int 	jack_set_port_connect_callback (jack_client_t *, JackPortConnectCallback connect_callback, void *arg) JACK_OPTIONAL_WEAK_EXPORT
 
int 	jack_set_graph_order_callback (jack_client_t *, JackGraphOrderCallback graph_callback, void *) JACK_OPTIONAL_WEAK_EXPORT
```
