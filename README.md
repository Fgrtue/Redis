# Redis
This is my implementation of Redis Server

## Write server side
1. Handle the possibility of connection to the server
	0. Create a structure for connections
		- Use read and write separate buffers
		- Add a state flag for deciding what to do with connection
	1. Choose a port for connection
	2. Get information about your address
		- IPv4 or IPv6
		- Port
		- Type of transport protocol
	3. Try to connect to create listening socket
		- try socket
		- try the port in case it is not available
		- try bind
	4. Handle the Polling stage
		- Use vector to store Connections
			- This vector owns the fd of other sockets
		- Use vector to store pollfd
			- This is temporary array for having the polled fd's
		- Start a while loop
		- For each connection fill a pfd and put it in pfds 
		- Poll on pfds
		- Walk through all the active connections except for listening socket
			- get the connection using the fd key of the polled file descriptor
			- handle the I/O of the connection
			- in case the state changed to end -- close the connection
		- check if the listening socket is ready for accepting new connections
	5. Create Accept new connections function
		1. Accept new fd independently of the IP protocol, using `sockaddr_storage`
		2. Set new fd to non-blocking
		3. Create new connection based on this fd
	6. Create connection input/output function
		1. in infinite while loop try to fill the buffer for read
			1. Read the amount of elements that can fit in the buffer
			2. Handle different cases when read returned some error
			3. Also handle the case when the client was intended to end the connection
		2. in infinite while loop try to do the request
			1. For this just read what's in the buffer, decoding the length of the message
			2. If all the sizes are okay with the max message size -> try to write in the connection
		3. Writing in the connection is done in infinite loop
- Suggestions
	- Use unique ptr instead of raw ptr
	- Use exceptions for error handling
	- Use RAII for sockets
	- Use std::array and std::vector instead of C-style arrays