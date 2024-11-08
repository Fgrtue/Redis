

#include "testEH.hpp"
#include <iostream>



void TestEH::decodeOutput() {

    char code = EH.out_[0];
    if (code == static_cast<char>(SER::NIL)) {

        cout << "nil\n";
    } else if (code == static_cast<char>(SER::STR)) {

        EH.out_.erase(0, 1);
        cout << "String: ";
        cout << EH.out_ << "\n";
    } else if (code == static_cast<char>(SER::INT)) {

        EH.out_.erase(EH.out_.begin());
        uint64_t num = 0;
        memcpy(&num, &EH.out_[0], 8);
        cout << "Int: " << num << "\n";
    } else if (code == static_cast<char>(SER::ARR)) {

        EH.out_.erase(EH.out_.begin());
        uint64_t len = 0;
        memcpy(&len, &EH.out_[0], 8);
        cout << "Arr: " << len << "\n";
    } else if (code == static_cast<char>(SER::ERR)) {
        EH.out_.erase(EH.out_.begin());
        uint32_t err_code = 0;
        memcpy(&err_code, &EH.out_[0], 4);
        EH.out_.erase(0, 4);
        cout << "CODE: " << err_code << "\n";
        cout << "Message: " << EH.out_ << "\n";
    }
    cout << "\n";
    EH.out_.clear();
}

void TestEH::OutResponseOffset() {
        
    cout << "Offset: " << EH.offset_ << "\n";

    cout << "Out.size: " << EH.out_.size() << "\n";

    cout << "Response.size: " << EH.response_.size() - EH.offset_ << "\n";
}

void TestEH::PrintWriteBuffer(int32_t* msg_len) {

    int32_t len = 0;
    EH.w_buf->writeToBuff(&len, 4);
    cout << "Message len: " << len << "\n";
    char bit;
    EH.w_buf->writeToBuff(&bit, 1);
    EH.w_buf->writeToBuff(msg_len, 4);
    cout << "Str_len: " << *msg_len << "\n";
    std::string msg;
    msg.resize(EH.w_buf->Size());
    EH.w_buf->writeToBuff(msg.data(), EH.w_buf->Size());
    cout << msg << "\n";
    cout << "Buf size after send: " << EH.w_buf->Size() << "\n";
}

void TestEH::PrintWriteBuffer2() {

    cout << "SecondWrite\n";
    std::string msg;
    msg.resize(EH.w_buf->Size());
    EH.w_buf->writeToBuff(msg.data(), EH.w_buf->Size());
    cout << msg << "\n";
    
}


void TestEH::testOutFunc() {

    cout << "Test outNil\n";
    EH.outNil();
    decodeOutput();

    cout << "Test outErr\n";
    int32_t code = 128;
    std::string msg = "Error message";
    EH.outErr(code, msg);
    decodeOutput();

    cout << "Test outStr\n";
    std::string str = "Some Message";
    EH.outStr(str);
    decodeOutput();

    cout << "Test outInt\n";
    uint64_t num = 128;
    EH.outInt(num);
    decodeOutput();

    cout << "Test outArr\n";
    uint64_t arr_len = 128;
    EH.outArr(arr_len);
    decodeOutput();
}

void TestEH::testFillWriteBuffer() {

    // Make some connection

    Conn conn;
    EH.to_initial(&conn);

    // 1. Fill out Err // Make the message big enough
    {
        std::string msg = "#################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################aaaaa###################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################";
        EH.outStr(msg);

    // 2. print offset

        cout << "\nOffset before filling buf:\n";

        OutResponseOffset();

    // 3. Fill write buffer

        EH.fill_write_buffer();

    // 4. Print offset again

        cout << "\nOffset after filling buf1:\n";

        OutResponseOffset();

    // 5. print elements from w_buf: read first four bits, 
    //      then the rest 

        int32_t msg_len;

        PrintWriteBuffer(&msg_len);

        EH.fill_write_buffer();

    // 4. Print offset again

        cout << "\nOffset after filling buf2:\n";

        OutResponseOffset();

    // 5. print elements from w_buf: read first four bits, 
    //      then the rest 

        PrintWriteBuffer2();
    }


}

void TestEH::testKeys() {

    EH.cmd_.resize(3);
    // 1. set 1024 keys

    for(int i = 0; i < 1024; ++i) {
        EH.cmd_[1] = std::to_string(i);
        EH.cmd_[2] = "1";
        EH.do_set();
    }

    std::vector<std::string> arr = EH.do_keys();
    cout << "Size or keys: " << arr.size() << "\n";
    for (int i = 0; i < arr.size(); ++i) {
        cout << arr[i] << "\n";
    }
}
