#include "SimpleStore.h"
#include <algorithm>

SimpleStore::SimpleStore()
{
	m_store.reserve(10000);
}

void SimpleStore::insert(int64_t id, SimpleProperties props)
{
	auto pair = m_store.emplace(id, props);
}

std::optional<SimpleProperties> SimpleStore::get(int64_t id)
{
	if (m_store.find(id) != m_store.end())
		return std::optional<SimpleProperties>{m_store.at(id)};
	return std::nullopt;
}

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

void SimpleStore::remove(int64_t id)
{
	m_store.erase(id);
}

using MapIterator = std::unordered_map<int64_t, SimpleProperties>::const_iterator;
std::vector<int64_t> SimpleStore::queryTitle(std::string title)
{
	std::vector<int64_t> ids; // hand rolled loop
	for (MapIterator it = m_store.begin(); it != m_store.end(); ++it)
		if (it->second.title == title)
			ids.emplace_back(it->first);
	return ids;
}

//std::pair< mapTimestampIterator, mapTimestampIterator> SimpleStore::range_query(double t1, double t2)
//{
//	auto pair1 = m_rangeTimestamp.equal_range(t1);
//	auto pair2 = m_rangeTimestamp.equal_range(t2);
//	return std::make_pair(pair1.first, pair2.second);
//}
//
//std::optional<std::reference_wrapper<Properties>> SimpleStore::get_ref(int64_t id)
//{
//	if (m_store.find(id) != m_store.end())
//		return std::optional<std::reference_wrapper<SimpleProperties>>{m_store.at(id)};
//	return std::nullopt;
//}