#pragma once

#include "esphome/components/daikin_rotex_can/entity.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include <vector>

namespace esphome {
namespace daikin_rotex_can {

class CanSelect;

class TEntityManager {
public:
    TEntityManager();
    void add(esphome::daikin_rotex_can::TEntity* pRequest);

    void removeInvalidRequests();

    void setCanbus(esphome::esp32_can::ESP32Can* pCanbus);
    esphome::esp32_can::ESP32Can* getCanbus() const;

    uint32_t size() const;
    TEntity const* get(uint32_t index) const;
    TEntity const* get(std::string const& id) const;

    EntityBase* get_entity_base(std::string const& id);
    EntityBase const* get_entity_base(std::string const& id) const;

    const std::vector<TEntity*>& get_entities() const { return m_entities; }

    sensor::Sensor* get_sensor(std::string const& id);
    sensor::Sensor const* get_sensor(std::string const& id) const;

    text_sensor::TextSensor* get_text_sensor(std::string const& id);
    CanSelect* get_select(std::string const& id);

    bool sendNextPendingGet();
    void sendSet(std::string const& request_name, float value);
    void handle(uint32_t can_id, TMessage const& responseData);

private:
    TEntity* getNextRequestToSend();

    std::vector<TEntity*> m_entities;
    esphome::esp32_can::ESP32Can* m_pCanbus;
};

inline void TEntityManager::setCanbus(esphome::esp32_can::ESP32Can* pCanbus) {
    m_pCanbus = pCanbus;
}

inline esphome::esp32_can::ESP32Can* TEntityManager::getCanbus() const {
    return m_pCanbus;
}

inline uint32_t TEntityManager::size() const {
    return m_entities.size();
}

inline TEntity const* TEntityManager::get(uint32_t index) const {
    return (index < m_entities.size()) ? m_entities[index] : nullptr;
}

inline sensor::Sensor* TEntityManager::get_sensor(std::string const& id) {
    return Utils::toSensor(get_entity_base(id));
}

inline sensor::Sensor const* TEntityManager::get_sensor(std::string const& id) const {
    return Utils::toSensor(get_entity_base(id));
}

inline text_sensor::TextSensor* TEntityManager::get_text_sensor(std::string const& id) {
    return Utils::toTextSensor(get_entity_base(id));
}

inline CanSelect* TEntityManager::get_select(std::string const& id) {
    return Utils::toSelect(get_entity_base(id));
}

}
}