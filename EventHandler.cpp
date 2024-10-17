#include "EventHandler.hpp"

void EventHandler::connection_io(Conn* conn) {
    while(read_request()) {}
    perform_request();
    while(write_response()) {}
}

bool EventHandler::read_request() {

    // Copy data into the read_buff

    do {
        size_t cap = conn->r_buf.size() - conn_->r_buf_size;
    } while (rv < 0 && errno == EINTR);
    check_read();

} 

bool EventHandler::write_response(){

    // Copy data into the write buffer

    return check_write();
}

