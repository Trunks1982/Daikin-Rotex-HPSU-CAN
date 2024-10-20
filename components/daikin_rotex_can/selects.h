#pragma once

#include "esphome/components/select/select.h"
#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"
#include "esphome/components/daikin_rotex_can/BidiMap.h"

namespace esphome {
namespace daikin_rotex_can {

class GenericSelect : public select::Select, public TEntity, public Parented<DaikinRotexCanComponent> {
    using TCustomSelectLambda = std::function<bool(std::string const& id, uint16_t key)>;
public:
    GenericSelect() = default;
    void set_id(std::string const& id) { m_id = id; }
    void set_map(std::string const& str_map);
    void set_custom_select_lambda(TCustomSelectLambda&& lambda) { m_custom_select_lambda = std::move(lambda); }

    std::string findNextByKey(uint16_t value, std::string const& fallback) const;
    uint16_t getKey(std::string const& value) const;

    void publish_select_key(uint16_t key);
protected:
    void control(const std::string &value) override;
    virtual TVariant handleValue(uint16_t value) override;

private:
    std::string m_id;
    BidiMap m_map;
    TCustomSelectLambda m_custom_select_lambda;
};

inline std::string GenericSelect::findNextByKey(uint16_t value, std::string const& fallback) const { 
    auto it = m_map.findNextByKey(value);
    return it != m_map.end() ? it->second : Utils::format("INVALID<%f>", value);
};

}  // namespace ld2410
}  // namespace esphome
