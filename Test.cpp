#include <string>
#include "Store.h"
#include <random>
#include <vector>
#include <cassert>
#include <iostream>

const int STR_LEN = 16;

std::string random_string() {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char str[STR_LEN];
    for (int i = 0; i < STR_LEN - 1; ++i) {
        str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    str[STR_LEN-1] = 0;
    return str;
}

double random_double() {
    double lower_bound = 0.0;
    double upper_bound = 100000.0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(lower_bound, upper_bound);
    return dis(gen);
}

const int VEC_K_LEN = 1000;
const int VEC_M_LEN = 1000 * 1000;
using vec_string = std::vector<std::string>;

vec_string generate_1k_strings() {
    vec_string v;
    v.reserve(VEC_K_LEN);
    for (size_t i = 0; i < VEC_K_LEN; ++i)
        v.emplace_back(random_string());
    return v;
}

// OPTIONAL
std::vector<double> generate_1m_doubles() {
    std::vector<double> v;
    v.reserve(VEC_M_LEN);
    for (size_t i = 0; i < VEC_M_LEN; ++i)
        v.emplace_back(random_double());
    return v;
}

static vec_string title_seed = generate_1k_strings();
static vec_string desc_seed = generate_1k_strings();
static std::vector<double> timestamp_seed = generate_1m_doubles();


void update_titles_test(Store& store) {
    int64_t id = rand() % VEC_M_LEN;
    auto rec = store.get(id);
    if (rec.has_value()) {
        std::string title = rec->title;
        auto ids = store.queryTitle(title);
        for (auto it = ids.first; it != ids.second; ++it)
            std::cout << it->first << "\t";
        int64_t initialCount = std::distance(ids.first, ids.second);
        // change title of the first id
        Properties p = { "Not that random string" };
        store.update((ids.first)->second, p);
        ids = store.queryTitle(title);
        std::cout << "\n\n";
        for (auto it = ids.first; it != ids.second; ++it)
            std::cout << it->first << "\t";
        int64_t afterTitleChangedCount = std::distance(ids.first, ids.second);
        assert(afterTitleChangedCount = initialCount - 1);
    }

}

void main_test() {
    Store store;
    Properties props;
    for (int i = 0; i < VEC_M_LEN; ++i) {
        props.title = title_seed.at(rand()% VEC_K_LEN);
        props.description = desc_seed.at(rand() % VEC_K_LEN);
        props.timestamp = random_double();// don't allow collisions here //timestamp_seed.at(rand() % VEC_LEN * VEC_LEN);
        store.insert(i, props);
    }
    update_titles_test(store);
}