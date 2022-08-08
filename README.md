# Generic-IPC-Handler
Trialing AF_UNIX sockets  (IPC) 

Initially this was started when I was trialing AF_UNIX sockets, but turned into a cross platform tool for passing data from one app to another.

it works on Mac/Windows and Linux, see src/tests/ for some example code.

# API notes

IPCClient.setup_one_shot() // closes immediately
IPCClient.setup() // stays open

# To send data use 
```
SocketImplementation::send / recv
```

Buffer is limited to 256 this will be removed later.
