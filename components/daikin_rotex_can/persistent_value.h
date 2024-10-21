#pragma once

#include "esphome/core/entity_base.h"
#include "esphome/core/preferences.h"

namespace esphome {
namespace daikin_rotex_can {

template<class T>
class PersistentValue {
public:
    PersistentValue(T && default_value)
    : m_default_value(std::move(default_value))
    , m_value(m_default_value)
    {
    }

    T const& value() const { return m_value; }

    void load(EntityBase* p_entity_base) {
        m_pref = global_preferences->make_preference<T>(p_entity_base->get_object_id_hash());
        if (!m_pref.load(&m_value)) {
            m_value = m_default_value;
        }
    }

    void save(T const& value) {
        m_value = value;
        m_pref.save(&m_value);
    }
private:
    T m_default_value;
    T m_value;
    ESPPreferenceObject m_pref;
};

}
}