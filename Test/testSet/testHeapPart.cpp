#include "../../Server/Set/Set.hpp"
#include "../../Server/Clock.hpp"
#include <string>
#include <iostream>
#include <unistd.h>

using std::cout;

int main() {

    Set set;
    // insert keys into set

    for (int i = 0; i < 10; ++i) {
        set.insertSet(std::to_string(i), i);
    }

    // change tll of some keys to 30 seconds in front

    for (int i = 0; i < 10; ++i) {
        cout << "TTL: " << i << " ";
        set.setTTL(std::to_string(i), (i + 1) * 1000);
        cout << "\n";
    }

    // then get ttl of these keys 

    cout << "Current time: " << getMonotonicTime() / 1000 << "\n";
    for (int i = 0; i < 10; ++i) {
        auto el = set.getTTL(std::to_string(i));
        if (el.has_value()) {
            cout << i << " val := " << el.value() << "\n";
        }
    }
    cout << "\n";

    // find min heap

    cout << "Min heap: " << set.minHeap() << "\n";

    // then sleep for 1 second

    sleep(5);

    // then clean heap 

    
    set.cleanHeap();

    // checl that it is empty

    for (int i = 0; i < 10; ++i) {
        auto el = set.getTTL(std::to_string(i));
        if (el.has_value()) {
            cout << i << " val := " << el.value() << "\n";
        } else {
            cout << i << " has no val\n";
        }
    }
    // cout << "Time: " << getMonotonicTime() / 1000 << "\n";
    // sleep(1);
    // cout << "Time + 1 sec: " << getMonotonicTime() / 1000 << "\n";

}


