#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include <map>

struct SimpleProperties {
    std::string title = "";
    std::string description = "";
    double timestamp = 0.0;
};

struct Todo {
    int64_t id;
    SimpleProperties props;
};

class SimpleStore {
    std::unordered_map<int64_t, SimpleProperties> m_store;
public:
    SimpleStore();
    SimpleStore(SimpleStore const&) = delete;
    SimpleStore& operator=(SimpleStore const&) = delete;

    void insert(int64_t id, SimpleProperties props);
    std::optional<SimpleProperties> get(int64_t id);
    void update(int64_t id, const SimpleProperties& props);
    void remove(int64_t id);

    std::vector<int64_t> queryTitle(std::string title);
private:
    std::optional<std::reference_wrapper<SimpleProperties>> get_ref(int64_t id);
};

