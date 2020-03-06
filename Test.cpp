#include <string>
#include "Store.h"
#include <random>
#include <vector>
#include <iostream>
#include <chrono>

struct TimeToRun
{
    typedef std::chrono::high_resolution_clock::time_point moment;

    TimeToRun(char const* string)
    {
        str = string;
        t_start = std::chrono::high_resolution_clock::now();
    }
    ~TimeToRun()
    {
        using namespace std::chrono;
        moment t_end = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
        std::cout << str << "\t= " << time_span.count() << " seconds.\n";
    }
private:
    moment t_start;
    char const* str = nullptr;
};

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
    TimeToRun t("update_titles_test");
    std::cout << "Starting update_titles_test...\n";
    int64_t id = rand() % VEC_M_LEN;
    auto rec = store.get(id);
    if (rec.has_value()) {
        std::string title = rec->title;
        auto ids = store.queryTitle(title);
        int64_t initialCount = std::distance(ids.first, ids.second);
        
        // change title of the first id
        Properties p = { "Not that random string" };
        store.update((ids.first)->second, p);
        ids = store.queryTitle(title);
        int64_t afterTitleChangedCount = std::distance(ids.first, ids.second);
        if (afterTitleChangedCount == initialCount - 1)
            std::cout << "PASSED ";
        else
            std::cout << "FAILED ";
        std::cout << "update title test\n";
    }

}

void init_store(Store& store) {
    TimeToRun t("init_store");
    Properties props;
    for (int i = 0; i < VEC_M_LEN; ++i) {
        props.title = title_seed.at(rand() % VEC_K_LEN);
        props.description = desc_seed.at(rand() % VEC_K_LEN);
        props.timestamp = random_double();// don't allow collisions here //timestamp_seed.at(rand() % VEC_LEN * VEC_LEN);
        store.insert(i, props);
    }
}

void main_test() {
    Store store;
    init_store(store);
    update_titles_test(store);
}

// debug
//init_store = 50.8851 seconds.
//Starting update_titles_test...
//PASSED update title test
//update_titles_test = 0.000817453 seconds.

// release
//init_store = 5.69093 seconds.
//Starting update_titles_test...
//PASSED update title test
//update_titles_test = 0.00029504 seconds.