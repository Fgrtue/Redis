
// Manages the core loop and registers event handlers

// 1. Rgisters file descriptors (sockets)
// 2. Uses polling to determine which descriptors are ready
// 3. Delegates events to appropriate handlers

// Basically the class that gets into infinite while loop,
// polls on the file descriptors, and in case an event happens
// handles it with the right function