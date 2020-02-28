#include "Store.h"

Store::Store()
{
	m_store.reserve(10000);
}

void Store::insert(int64_t id, Properties props)
{
	m_store.try_emplace(id, props);
}

std::optional<std::reference_wrapper<Properties>> Store::get(int64_t id)
{
	if (m_store.find(id) != m_store.end())
		return std::optional<std::reference_wrapper<Properties>>{m_store.at(id)};
	return std::nullopt;
}

void Store::update(int64_t id, const Properties& props)
{
	auto get_of_id = get(id);
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

void Store::remove(int64_t id)
{
	m_store.erase(id);
}
