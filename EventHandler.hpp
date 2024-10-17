#pragma once

// Handles individual client connections

// Each time a connection is ready, the EventLoop delegates
// the enent to ConnectionHandler

// 1. Read Data from client
// 2. Write data to client
// 3. Handle Connection specific logic

struct Conn;

class EventHandler {
public: 
   void connection_io(Conn* conn);

private:

   // Copy request into read buffer

   bool read_request();

   // Check that we can read the request
   // 1. The length is provided
   // 2. The length is less than the message
   // 3. The length + 4 bytes for the lenght is not more
   // than the amounth that we read so far

   // Perform the request *

   // Move the buffer, erasing what has been read so far
   
   // Write response

   // Copy data into the wright buffer
   // Perform the checks of errors
   // Check if all the message was sent

   // * Perform the request
   // Parse request
   // Choose the right request: get, set, del
   // Depending on the request -- perform it

   ssize_t rv = 0;
   Conn* conn_;

};