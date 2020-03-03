#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include <map>

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

using mapIterator = std::multimap <std::string, int64_t>::iterator;

class Store {
    std::unordered_map<int64_t, Properties> m_store;
    std::multimap <std::string, int64_t> m_equalTitle;
public:
    Store();
	Store(Store const&) = delete;
	Store& operator=(Store const&) = delete;

    void insert(int64_t id, Properties props);
    std::optional<Properties> get(int64_t id);
    void update(int64_t id, const Properties& props);
    void remove(int64_t id);
    std::pair< mapIterator, mapIterator> queryTitle(std::string title);
private:
    std::optional<std::reference_wrapper<Properties>> get_ref(int64_t id);
};

