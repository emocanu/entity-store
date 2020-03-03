#include "Store.h"

Store::Store()
{
	m_store.reserve(10000);
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
			get_of_id->get().timestamp = props.timestamp;
			// add in m_rangeTimestamp for the new timestamp
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
	}
	m_store.erase(id);
}

std::pair< mapIterator, mapIterator> Store::queryTitle(std::string title)
{
	return m_equalTitle.equal_range(title);
}

std::optional<std::reference_wrapper<Properties>> Store::get_ref(int64_t id)
{
	if (m_store.find(id) != m_store.end())
		return std::optional<std::reference_wrapper<Properties>>{m_store.at(id)};
	return std::nullopt;
}