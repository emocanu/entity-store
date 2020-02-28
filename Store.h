#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>

struct Properties {
    std::string title = "";
    std::string description = "";
    double timestamp = 0.0;
    // can define here copy ctor
};

struct Todo {
    int64_t id;
    Properties props;
};

class Store {
    std::unordered_map<int64_t, Properties> m_store;
public:
    Store();
	Store(Store const&) = delete;
	Store& operator=(Store const&) = delete;

    void insert(int64_t id, Properties props);
    std::optional<std::reference_wrapper<Properties>> get(int64_t id);
    void update(int64_t id, const Properties& props);
    void remove(int64_t id);
};

