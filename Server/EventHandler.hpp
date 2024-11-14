#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <map>
#include "Clock.hpp"
#include "Conn.hpp"
#include "CircBuf.hpp"
#include "Set/Set.hpp"

// In case of testing make testEh a friend class for EventHandler

// Handles individual client connections

// Each time a connection is ready, the EventLoop delegates
// the enent to ConnectionHandler

// 1. Read Data from client
// 2. Write data to client
// 3. Handle Connection specific logic


struct Conn;

using std::cout;

// #1. Add serialization enumiration
// nil      -> [ mes_len ] [ Ser Code ]
// err      -> [ mes_len ] [ Ser Code ] [ err code ] [ err_mes ]
// int      -> [ mes_len ] [ Ser Code ] [ int ]
// string   -> [ mes_len ] [ Ser Code ] [ str ]
// array    -> [ mes_len ] [ Ser Code ] [ arr_len ] [ Ser Code1 ] [ str/int ] ... [Ser Code n] [ str/len ] 

enum class SER {
    NIL,
    ERR,
    STR,
    INT,
    DOUB,
    ARR,
};

class EventHandler {
public:

   void connection_io(Conn* conn);

   void clearTTL();

   std::optional<uint64_t> topTTL();

private:

   void to_initial(Conn* conn);

   bool try_fill_buffer();

   // Copy request into read buffer

   void read_request();

   // Check that read was valid

   bool check_read();

   bool try_one_request();

   bool check_read_buffer(int *len);

   bool do_request(int len);

   bool parseReq(int len);

   void do_range();

   void do_get();

   void do_del();

   void do_set();

   void do_expire();

   void do_ttl();

   void sendErr(int32_t err_no, std::string msg);
   
   // Write response

   void fill_write_buffer();

   bool try_flush_buffer();

   void fill_response();

   // Copy data into the wright buffer

   void write_response();

   // Perform the checks of errors
   // Check if all the message was sent

   bool check_write();

   // * Perform the request
   // Parse request
   // Choose the right request: get, set, del
   // Depending on the request -- perform it

   void outNil();

   void outErr(int32_t, const std::string&);

   void outStr(const std::string_view);

   void outInt(uint64_t);

   void outDouble(double);

   void outArr(uint64_t);

   ssize_t  rv = 0;
   Conn*    conn_;

   CircBuf<4UL + Conn::max_mes>* r_buf;
   CircBuf<4UL + Conn::max_mes>* w_buf;

   std::vector<std::string> cmd_;

   std::string out_;
   std::string response_;

   uint32_t offset_;

   Set set;
};