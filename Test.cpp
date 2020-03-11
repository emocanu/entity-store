#include <string>
#include "Store.h"
#include "SimpleStore.h"
#include <random>
#include <vector>
#include "main.h"

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

template<typename T>
void update_titles_test(T& store) {
    TimeToRun t("update_titles_test");
    std::cout << "Starting update_titles_test...\n";
    int64_t id = rand() % VEC_M_LEN;
    auto rec = store.get(id);
    if (rec.has_value()) {
        std::string title = rec->title;
        std::vector<int64_t> ids = store.query_title(title);
        int64_t initialCount = ids.size();
        if (initialCount == 0) {
            std::cout << "COULD NOT CHECK, ids().size == 0";
            return;
        }

        // change title of the first id
        Properties p = { "Not that random string" };
        store.update(ids[0], p);
        ids = store.query_title(title);
        int64_t afterTitleChangedCount = ids.size();
        if (afterTitleChangedCount == initialCount - 1)
            std::cout << "PASSED ";
        else
            std::cout << "FAILED ";
        std::cout << "update title test\n";
    }

}

template<typename T>
void init_store(T& store) {
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
    std::cout << "START Store tests: \n";
    Store store;
    init_store<Store>(store);
    update_titles_test<Store>(store);
    std::cout << "\n\nSTART Simple Store tests: \n";
    SimpleStore sStore;
    init_store<SimpleStore>(sStore);
    update_titles_test<SimpleStore>(sStore);
}

// Release, x64

//START Store tests :
//init_store = 6.07839 seconds.
//Starting update_titles_test...
//Store::query_title = 0.000123147 seconds.
//Store::query_title = 5.6763e-05 seconds.
//PASSED update title test
//update_titles_test = 0.00144858 seconds.
//
//
//START Simple Store tests :
//init_store = 3.69318 seconds.
//Starting update_titles_test...
//SimpleStore::query_title = 0.030113 seconds.
//SimpleStore::query_title = 0.0306736 seconds.
//PASSED update title test
//update_titles_test = 0.0630103 seconds.