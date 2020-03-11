#include "SimpleStore.h"
#include "main.h"
#include <algorithm>

SimpleStore::SimpleStore()
{
	m_store.reserve(1000000);
}

// Time complexity: amortized constant on average, worst case linear in the size of the m_store
void SimpleStore::insert(int64_t id, SimpleProperties props)
{
	auto pair = m_store.emplace(id, props);
}

// Time complexity: constant on average, worst case linear in the size of the m_store
std::optional<SimpleProperties> SimpleStore::get(int64_t id)
{
	if (m_store.find(id) != m_store.end())
		return std::optional<SimpleProperties>{m_store.at(id)};
	return std::nullopt;
}

// Time complexity: see get_ref
void SimpleStore::update(int64_t id, const SimpleProperties& props)
{
	auto get_of_id = get_ref(id);
	if (get_of_id.has_value())
	{
		if (props.title.size() != 0)
			get_of_id->get().title = props.title;
		if (props.description.size() != 0)
			get_of_id->get().description = props.description;
		if (props.timestamp != 0.0)
			get_of_id->get().timestamp = props.timestamp;
	}
}

// Time complexity: constant on average, worst case linear in the size of the m_store
void SimpleStore::remove(int64_t id)
{
	m_store.erase(id);
}

using MapIterator = std::unordered_map<int64_t, SimpleProperties>::const_iterator;

// Time complexity: linear, O(n)
std::vector<int64_t> SimpleStore::query_title(std::string title)
{
	TimeToRun t("SimpleStore::query_title");
	std::vector<int64_t> ids; 
	ids.reserve(1000);
	for (MapIterator it = m_store.begin(); it != m_store.end(); ++it) // hand rolled loop
		if (it->second.title == title)
			ids.emplace_back(it->first);
	return ids;
}

// Time complexity: linear, O(n)
std::vector<int64_t> SimpleStore::range_query(double t1, double t2)
{
	std::vector<int64_t> ids; // hand rolled loop
	for (MapIterator it = m_store.begin(); it != m_store.end(); ++it)
		if (it->second.timestamp > t1 && it->second.timestamp < t2)
			ids.emplace_back(it->first);
	return ids;
}

// Helper function, Time complexity: constant on average, worst case linear in the size of the m_store
std::optional<std::reference_wrapper<SimpleProperties>> SimpleStore::get_ref(int64_t id)
{
	if (m_store.find(id) != m_store.end())
		return std::optional<std::reference_wrapper<SimpleProperties>>{m_store.at(id)};
	return std::nullopt;
}