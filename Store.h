#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include <map>
#include "main.h"

using mapIterator = std::multimap <std::string, int64_t>::iterator;
using mapTimestampIterator = std::multimap <double, int64_t>::iterator;

class Store {
    std::unordered_map<int64_t, Properties> m_store;
    bool m_isChild;

    // std::multimap           - Logarithmic in the size of the container.
    std::multimap <std::string, int64_t> m_equalTitle;
    std::multimap<double, int64_t> m_rangeTimestamp;
public:
    Store();
    Store(Store const&);
    Store& operator=(Store const&);
    Store& create_child();

    void insert(int64_t id, Properties props);
    std::optional<Properties> get(int64_t id);
    void update(int64_t id, const Properties& props);
    void remove(int64_t id);
    std::vector<int64_t> query_title(std::string title);
    std::vector<int64_t> range_query(double t1, double t2);
    void commit(Store& parent);
private:
    std::optional<std::reference_wrapper<Properties>> get_ref(int64_t id);
};

