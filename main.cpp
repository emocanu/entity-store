#include <cassert>
#include <iostream>
#include "Store.h"

int main(int argc, const char *argv[])
{
    Store store;

    Properties p1 = {"Buy Milk", "made of almonds!", 2392348.12233};
    store.insert(2166, p1);

    Properties p2 = { "Buy Chocolate" };
    store.update(2166, p2);

    auto retrieved = store.get(2166);

    store.remove(2166);

    return 0;
}
