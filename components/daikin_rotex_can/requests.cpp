#include "esphome/components/daikin_rotex_can/requests.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace daikin_rotex_can {

TRequests::TRequests()
: m_requests()
, m_pCanbus(nullptr)
{
}

void TRequests::add(esphome::daikin_rotex_can::TRequest const& request) {
    m_requests.push_back(request);
}

void TRequests::removeInvalidRequests() {
    m_requests.erase(
        std::remove_if(
            m_requests.begin(),
            m_requests.end(),
            [](TRequest const& request) { return !request.isGetSupported(); }
        ),
        m_requests.end()
    );
}

EntityBase* TRequests::get_entity(std::string const& id) {
    TRequest const* pRequest = get(id);
    if (pRequest != nullptr) {
        return pRequest->get_entity();
    } else {
        ESP_LOGE("get_entity", "Entity not found: %s", id.c_str());
    }
    return nullptr;
}

bool TRequests::sendNextPendingGet() {
    TRequest* pRequest = getNextRequestToSend();
    if (pRequest != nullptr) {
        return pRequest->sendGet(m_pCanbus);
    }
    return false;
}

void TRequests::sendSet(std::string const& request_name, float value) {
    const auto it = std::find_if(m_requests.begin(), m_requests.end(),
        [&request_name](auto& request) { return request.getName() == request_name; }
    );
    if (it != m_requests.end()) {
        it->sendSet(m_pCanbus, value);
    } else {
        ESP_LOGE("sendSet", "Unknown request: %s", request_name.c_str());
    }
}

void TRequests::handle(uint32_t can_id, TMessage const& responseData) {
    bool bHandled = false;
    const uint32_t timestamp = millis();
    for (auto& request : m_requests) {
        if (request.handle(can_id, responseData, timestamp)) {
            bHandled = true;
            break;
        }
    }
    if (!bHandled) {
        Utils::log("unhandled", "can_id<%s> data<%s>", Utils::to_hex(can_id).c_str(), Utils::to_hex(responseData).c_str());
    }
}

TRequest const* TRequests::get(std::string const& id) const {
    for (auto& request: m_requests) {
        if (request.get_id() == id) {
            return &request;
        }
    }
    return nullptr;
}

TRequest* TRequests::getNextRequestToSend() {
    const uint32_t timestamp = millis();

    for (auto& request : m_requests) {
        if (request.isGetInProgress()) {
            return nullptr;
        }
    }

    for (auto& request : m_requests) {
        if (request.isGetNeeded()) {
            return &request;
        }
    }
    return nullptr;
}


}
}