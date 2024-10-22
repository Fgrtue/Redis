#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <map>
#include "Conn.hpp"

// Handles individual client connections

// Each time a connection is ready, the EventLoop delegates
// the enent to ConnectionHandler

// 1. Read Data from client
// 2. Write data to client
// 3. Handle Connection specific logic

struct Conn;

using std::cout;

enum class Res : uint8_t {
    OK,
    ERR,
    NX,
};

class EventHandler {
public:

   void connection_io(Conn* conn);

private:

   void to_initial();

   bool try_fill_buffer();

   // Copy request into read buffer

   void read_request();

   // Check that read was valid

   bool check_read();
   
   // Write response

   bool try_flush_buffer();

   // Copy data into the wright buffer

   void write_response();

   // Perform the checks of errors
   // Check if all the message was sent

   bool check_write();

   // * Perform the request
   // Parse request
   // Choose the right request: get, set, del
   // Depending on the request -- perform it

   bool try_one_request();

   bool check_read_buffer(int *len);

   void fill_write_buffer(int len);

   bool do_request(int len);


   bool parseReq(int len, uint32_t pos);

   void do_get();

   void do_del();

   void do_set();

   ssize_t  rv = 0;
   uint32_t wlen_ = 0;
   Res      rescode_ = Res::OK;
   Conn*    conn_;

   std::vector<std::string> cmd_;

   std::map<std::string, std::string> g_map;
};