# jackerl

erlang nif implementation for jack audio.

## clients manipulation.


### open a client

function arity:

|                   invokation                   |    arity    |
|:-----------------------------------------------|:-----------:|
| `jack:open(ClientName).`                       | jack:open/1 |
| `jack:open(ClientName, JackOptsAtom).`         | jack:open/2 |
| `jack:open(ClientName, JackOptsList).`         | jack:open/2 |
| `jack:open(ClientName, ServerName, JackOpts).` | jack:open/3 |


|     __Param__    |             __description__             |      __type__     |       __example__      |
|:------------:|:-----------------------------------:|:-------------:|:------------------:|
|  `ClientName`  |           jack client name          |      atom     |     `white_noise`    |
|  `ServerName`  | jack server name (normally default) |      atom     |       `default`      |
|   `JackOpts`   |          JackOptions number         |    integer    |          `2`         |
| `JackOptsList` |     JackOptions in list of atoms (see below)    | list of atoms | `[jackUseExactName,jackServerName, ]` |



outputs:

|                 output                |                         description                         |
|:-------------------------------------:|:-----------------------------------------------------------:|
|     `{ok, {clientName, ClientName}}`    |  returns the jack client name in case that jack renames it. |
| `{error, {jackStatus, JackStatusList}}` | returns a list of atoms with jack status atoms (see below). |

example:

```erlang

ClientName   = white_noise.
ServerName   = default.
JackOpts     = 0;
JackOptAtom  = jackNullOption;
JackOptsList = [ jackNoStartServer, jackUseExactName ].


% different ways of opening jack clients

{ ok, {clientName, ClientName }}          = jack:open(ClientName).
{ ok, {clientName, ClientNewName }}       = jack:open(ClientName, JackOptsAtom).
jack:open(ClientName, JackOptsList).
jack:open(ClientName, ServerName, JackOpts).    
```

### close a client

function arity:

|                   invokation                   |    arity    |
|:-----------------------------------------------|:-----------:|
| `jack:close(ClientName).`                      | jack:close/1 |


|     __Param__    |             __description__             |      __type__     |       __example__      |
|:------------:|:-----------------------------------:|:-------------:|:------------------:|
|  `ClientName`  |           jack client name          |      atom     |     `white_noise`    |


outputs:

|                 output                |                         description                         |
|:-------------------------------------:|:-----------------------------------------------------------:|
|     `{ok, {clientName, ClientName}}`    |  returns the jack client name. |
| `{error, {jackStatus, JackStatusList}}` | returns a list of atoms with jack status atoms (see below). |

example:

```erlang

ClientName   = white_noise.

{ ok, {clientName, ClientName }}          = jack:close(ClientName).
{ error, {jackStatus, Status}}            = jack:close(not_existing_client)
```

### activate a client


function arity:

|                   invokation                   |    arity    |
|:-----------------------------------------------|:-----------:|
| `jack:activate(ClientName).`                      | jack:activate/1 |


|     __Param__    |             __description__             |      __type__     |       __example__      |
|:------------:|:-----------------------------------:|:-------------:|:------------------:|
|  `ClientName`  |           jack client name          |      atom     |     `white_noise`    |


outputs:

|                 output                |                         description                         |
|:-------------------------------------:|:-----------------------------------------------------------:|
|     `{ok, {clientName, ClientName}}`    |  returns the jack client name. |
| `{error, {jackStatus, JackStatusList}}` | returns a list of atoms with jack status atoms (see below). |


### deactivate a client


function arity:

|                   invokation                   |    arity    |
|:-----------------------------------------------|:-----------:|
| `jack:deactivate(ClientName).`                      | jack:deactivate/1 |


|     __Param__    |             __description__             |      __type__     |       __example__      |
|:------------:|:-----------------------------------:|:-------------:|:------------------:|
|  `ClientName`  |           jack client name          |      atom     |     `white_noise`    |


outputs:

|                 output                |                         description                         |
|:-------------------------------------:|:-----------------------------------------------------------:|
|     `{ok, {clientName, ClientName}}`    |  returns the jack client name. |
| `{error, {jackStatus, JackStatusList}}` | returns a list of atoms with jack status atoms (see below). |



## ports manipulation.


### register a port

function arity:

|                   invokation                   |    arity    |
|:-----------------------------------------------|:-----------:|
| `jack:register(ClientName, PortName, PortFlags).`                      | jack:register/3 |
| `jack:register(ClientName, PortName, PortFlagsList).`                      | jack:register/3 |


|     __Param__    |             __description__             |      __type__     |       __example__      |
|:------------:|:-----------------------------------:|:-------------:|:------------------:|
|  `ClientName`  |           jack client name          |      atom     |     `white_noise`    |
|  `PortName`  |           port name          |      atom     |     `input`    |
|  `PortFlags`  |           port flags (see below)          |      int     |     1    |
|  `PortFlagsList`  |           port flags list of atoms(see below)          |      list of atoms     |     `[ jackPortIsInput ]`    |


outputs:

|                 output                |                         description                         |
|:-------------------------------------:|:-----------------------------------------------------------:|
|     `{ok, {portName, PortName}}`    |  returns the jack client name. |
| `{error, {jackStatus, JackStatusList}}` | returns a list of atoms with jack status atoms (see below). |

### unregister a port

function arity:

|                   invokation                   |    arity    |
|:-----------------------------------------------|:-----------:|
| `jack:unregister(ClientName, PortName, PortFlags).`                      | jack:unregister/3 |
| `jack:unregister(ClientName, PortName, PortFlagsList).`                      | jack:unregister/3 |


|     __Param__    |             __description__             |      __type__     |       __example__      |
|:------------:|:-----------------------------------:|:-------------:|:------------------:|
|  `ClientName`  |           jack client name          |      atom     |     `white_noise`    |
|  `PortName`  |           port name          |      atom     |     `input`    |
|  `PortFlags`  |           port flags (see below)          |      int     |     1    |
|  `PortFlagsList`  |           port flags list of atoms(see below)          |      list of atoms     |     `[ jackPortIsInput ]`    |


outputs:

|                 output                |                         description                         |
|:-------------------------------------:|:-----------------------------------------------------------:|
|     `{ok, {portName, PortName}}`    |  returns the jack client name. |
| `{error, {jackStatus, JackStatusList}}` | returns a list of atoms with jack status atoms (see below). |

## callbacks


function arity:

|                   invokation                   |    arity    |
|:-----------------------------------------------|:-----------:|
| `jack:callback(CallbackAtom, Callback).`       | jack:callback/2 |

Callback Arity

|     __Call    |             __description__             |      __type__     |       __example__      |
|:------------:|:-----------------------------------:|:-------------:|:------------------:|
|  `CallbackAtom`  |          atom for a callback (see below)        |      atom     |  ``  |
|  `Callback`      |    function with certain arity (see below)      |   function    |     `fun(ClientName, PortName, Reg )->  io:format("port: ~p on ~p is ~p",[PortName, ClientName, Reg]) end`     |


outputs:

|                 output                |                         description                         |
|:-------------------------------------:|:-----------------------------------------------------------:|
|     `{ok, {portName, PortName}}`    |  returns the jack client name. |
| `{error, {jackStatus, JackStatusList}}` | returns a list of atoms with jack status atoms (see below). |
















## atom flags

### Jack Options atoms

|        __atom__       |   __jack equivalent__  |                                                                                    __description__                                                                                    |
|:-----------------:|:------------------:|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|   `jackNullOption`  |   JackNullOption   |                                                                 Null value to use when no option bits are needed.                                                                 |
| `jackNoStartServer` | JackNoStartServer  | Do not automatically start the JACK server when it is not already running. This option is always selected if $JACK_NO_START_SERVER is defined in the calling process environment. |
|  `jackUseExactName` |  JackUseExactName  |                                       Use the exact client name requested. Otherwise, JACK automatically generates a unique one, if needed.                                       |
|   `jackServerName`  |   JackServerName   |                                                                          Optional server name parameter.                                                                          |
|    `jackLoadName`   |    JackLoadName    |                                                                               (not implemented yet)                                                                               |
|    `jackLoadInit`   |    JackLoadInit    |                                                                               (not implemented yet)                                                                               |
|   `jackSessionId`   |   JackSessionID    |                                                                               (not implemented yet)                                                                               |
### Jack Status atoms

|        atom       |   jack equivalent  |               Description                                                                                                                                                                                          |
|:-----------------:|:------------------:|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|    `failure`    | JackFailure        | Overall operation failed.                                                                                                                                                                                                                                                                                                                                                                      |
| `invalid_option` | JackInvalidOption  | The operation contained an invalid or unsupported option.                                                                                                                                                                                                                                                                                                                                      |
| `name_not_unique` | JackNameNotUnique  | The desired client name was not unique.  |
| `server_started` | JackServerStarted  | The JACK server was started as a result of this operation.   |
|  `server_failed` | JackServerFailed   | Unable to connect to the JACK server.                                                                                                                                                                                                                                                                                                                                                          |
|  `server_error`  | JackServerError    | Communication error with the JACK server.                                                                                                                                                                                                                                                                                                                                                      |
|  `no_such_client` | JackNoSuchClient   | Requested client does not exist.                                                                                                                                                                                                                                                                                                                                                               |
|  `load_failure`  | JackLoadFailure    | Unable to load internal client                                                                                                                                                                                                                                                                                                                                                                 |
|  `init_failure`  | JackInitFailure    | Unable to initialize client                                                                                                                                                                                                                                                                                                                                                                    |
|   `shm_failure`  | JackShmFailure     | Unable to access shared memory                                                                                                                                                                                                                                                                                                                                                                 |
|  `version_error` | JackVersionError   | Client's protocol version does not match                                                                                                                                                                                                                                                                                                                                                       |
|  `backend_error` | JackBackendError   |                                                                                                                                                                                                                                                                                                                                                                                                |
|  `client_zombie` | JackClientZombie   |                                                                                                                                                                                                                                                                                                                                                                                                |
|  `no_such_port` |     | Requested port does not exist.                                                                                                                                                                                                                                                                                                                                                               |

### Port flags

| atom                  | jack equivalent     | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
|-----------------------|---------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `input`     | JackPortIsInput     | if JackPortIsInput is set, then the port can receive data.                                                                                                                                                                                                                                                                                                                                                                                                                                  |
| `output`    | JackPortIsOutput    | if JackPortIsOutput is set, then data can be read from the port.                                                                                                                                                                                                                                                                                                                                                                                                                            |
| `physical`  | JackPortIsPhysical  | if JackPortIsPhysical is set, then the port corresponds to some kind of physical I/O connector.                                                                                                                                                                                                                                                                                                                                                                                             |
| `monitor,` | JackPortCanMonitor  | if JackPortCanMonitor is set, then a call to jack_port_request_monitor() makes sense. Precisely what this means is dependent on the client. A typical result of it being called with TRUE as the second argument is that data that would be available from an output port (with JackPortIsPhysical set) is sent to a physical output connector as well, so that it can be heard/seen/whatever. Clients that do not control physical interfaces should never create ports with this bit set. |
| `terminal`  | JackPortIsTerminal  | JackPortIsTerminal means: for an input port: the data received by the port will not be passed on or made available at any other port for an output port: the data available at the port does not originate from any other port Audio synthesizers, I/O hardware interface clients, HDR systems are examples of clients that would set this flag for their ports.                                                                                                                            |
### Jack Callback

| atom                             | jack equivalent                | Description |
|----------------------------------|--------------------------------|-------------|
| `process`            | JackProcessCallback            |             |
| `shutdown`           | JackShutdownCallback           |             |
| `port_registration`   | JackPortRegistrationCallback   |             |
| `client_registration` | jackClientRegistrationCallback |             |
