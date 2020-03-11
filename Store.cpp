#include "Store.h"
#include "main.h"

Store::Store()
	: m_isChild(false)
{
	m_store.reserve(1000000);
}

Store::Store(Store const& other)
{
	// TODO copy also the multimaps, or better, do this in the simple map
	m_store = other.m_store;
	m_isChild = true;
}

Store& Store::operator=(Store const& other)
{
	if (this != &other) {  
		m_store = other.m_store;
	}
	return *this;
}

Store& Store::create_child()
{
	return *this;
}

void Store::insert(int64_t id, Properties props)
{
	auto pair = m_store.emplace(id, props);
	if (pair.second) {
		m_equalTitle.insert({ props.title, id });
		m_rangeTimestamp.emplace(std::make_pair(props.timestamp, id));
	}
}

std::optional<Properties> Store::get(int64_t id)
{
	if (m_store.find(id) != m_store.end())
		return std::optional<Properties>{m_store.at(id)};
	return std::nullopt;
}

void Store::update(int64_t id, const Properties& props)
{
	auto get_of_id = get_ref(id);
	if (get_of_id.has_value())
	{
		if (props.title.size() != 0) {

			// remove from m_equalTitle for the title to be replaced
			auto iterpair = m_equalTitle.equal_range(get_of_id->get().title);
			for (auto it = iterpair.first; it != iterpair.second; ++it) {
				if (it->second == id) {
					m_equalTitle.erase(it);
					break;
				}
			}

			get_of_id->get().title = props.title;
			
			// add in m_equalTitle for the new title
			m_equalTitle.insert({ props.title, id });
		}
		if (props.description.size() != 0)
			get_of_id->get().description = props.description;
		if (props.timestamp != 0.0) {
			// remove from m_rangeTimestamp for the timestamp to be replaced
			auto iterpair = m_rangeTimestamp.equal_range(get_of_id->get().timestamp);
			for (auto it = iterpair.first; it != iterpair.second; ++it) {
				if (it->second == id) {
					m_rangeTimestamp.erase(it);
					break;
				}
			}

			get_of_id->get().timestamp = props.timestamp;

			// add in m_rangeTimestamp for the new timestamp
			m_rangeTimestamp.emplace(std::make_pair(props.timestamp, id));
		}
	}
}

void Store::remove(int64_t id)
{
	// get the record for id and remove the corresponding [title, id]
	auto get_of_id = get(id);
	if (get_of_id.has_value())
	{
		auto iterpair = m_equalTitle.equal_range(get_of_id->title);
		for (auto it = iterpair.first; it != iterpair.second; ++it) {
			if (it->second == id) {
				m_equalTitle.erase(it);
				break;
			}
		}
		// need to remove from m_rangeTimestamp also
		auto iterpairt = m_rangeTimestamp.equal_range(get_of_id->timestamp);
		for (auto it = iterpairt.first; it != iterpairt.second; ++it) {
			if (it->second == id) {
				m_rangeTimestamp.erase(it);
				break;
			}
		}
	}
	m_store.erase(id);
}

std::vector<int64_t> Store::query_title(std::string title)
{
	TimeToRun t("Store::query_title");
	std::vector<int64_t> ret;
	ret.reserve(1000);

	auto pair = m_equalTitle.equal_range(title);
	for (auto it = pair.first; it != pair.second; ++it)
		ret.emplace_back(it->second);
	
	return ret;
}

std::vector<int64_t> Store::range_query(double t1, double t2)
{
	std::vector<int64_t> ret;
	ret.reserve(1000);

	for (mapTimestampIterator it = m_rangeTimestamp.lower_bound(t1);
			it != m_rangeTimestamp.upper_bound(t2);
			++it)
		ret.emplace_back(it->second);

	return ret;
}

void Store::commit(Store& parent)
{
	// in the child store a history of modified ids
	// then do a merge of the maps
}



std::optional<std::reference_wrapper<Properties>> Store::get_ref(int64_t id)
{
	if (m_store.find(id) != m_store.end())
		return std::optional<std::reference_wrapper<Properties>>{m_store.at(id)};
	return std::nullopt;
}