#include <cassert>
#include <iostream>
#include "Store.h"
#include "SimpleStore.h"

extern void main_test();

int main(int argc, const char *argv[])
{
    SimpleStore store;

    Properties p1 = {"Buy Milk", "made of almonds!", 2392348.12233};
    store.insert(2166, p1);

    Properties p2 = { "Buy Chocolate" };
    store.update(2166, p2);

    auto retrieved = store.get(2166);

    Properties p3 = { "Buy Chocolate", "made of cocoa!", 1392348.12233 };
    store.insert(2165, p3);

    //auto ids = store.query_title("Buy Chocolate");

    //for (auto it = ids.first; it != ids.second; ++it)
    //    std::cout << it->second << "\n";
    
    store.remove(2166);

    SimpleStore child = store.create_child();

    main_test();

    return 0;
}
