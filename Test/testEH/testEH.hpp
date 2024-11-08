#pragma once

#include "../../Server/EventHandler.hpp"

// 1. Test out functions
// 2. Test keys function
// 3. Test different if-else paths with errors
//      -> test what is written inside write buffer in case of different functions

class TestEH {

public: 

    void testOutFunc();

    void testFillWriteBuffer();

    void PrintWriteBuffer(int32_t* msg_len);

    void PrintWriteBuffer2();
    
    void OutResponseOffset();

    void testKeys();

    void testCheckReadBuffer();

    void testDoCmds();

private:

    void decodeOutput();

    EventHandler EH;

};