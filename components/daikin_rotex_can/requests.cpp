#include "esphome/components/daikin_rotex_can/requests.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace daikin_rotex_can {

TRequests::TRequests(std::vector<esphome::daikin_rotex_can::TRequest> const& requests)
: m_requests(requests)
, m_pCanbus(nullptr)
{
}

void TRequests::add(esphome::daikin_rotex_can::TRequest const& request) {
    m_requests.push_back(request);
}

void TRequests::removeInvalidRequests(Accessor const& accessor) {
    m_requests.erase(
        std::remove_if(
            m_requests.begin(),
            m_requests.end(),
            [&accessor](TRequest const& request) { return !request.isGetSupported(accessor); }
        ),
        m_requests.end()
    );
}

bool TRequests::sendNextPendingGet(Accessor const& accessor) {
    TRequest* pRequest = getNextRequestToSend();
    if (pRequest != nullptr) {
        pRequest->sendGet(accessor, m_pCanbus);
        return true;
    }
    return false;
}

void TRequests::sendGet(Accessor const& accessor, std::string const& request_name) {
    const auto it = std::find_if(
        m_requests.begin(),
        m_requests.end(),
        [& request_name, & accessor](auto& request) {
            return !request.isSetter() && request.getName(accessor) == request_name && request.isGetSupported(accessor);
        }
    );

    if (it != m_requests.end()) {
        it->sendGet(accessor, m_pCanbus);
    } else {
        Utils::log("requests.cpp", "sendGet(%s) -> Unknown request!", request_name.c_str());
    }
}

void TRequests::sendSet(Accessor const& accessor, std::string const& request_name, float value) {
    const auto it = std::find_if(m_requests.begin(), m_requests.end(),
        [&request_name, &accessor](auto& request) { return request.isSetter() && request.getName(accessor) == request_name; }
    );
    if (it != m_requests.end()) {
        it->sendSet(accessor, m_pCanbus, value);
    } else {
        Utils::log("requests.cpp", "sendSet(%s) -> Unknown request!", request_name.c_str());
    }
}

void TRequests::handle(Accessor& accessor, uint32_t can_id, std::vector<uint8_t> const& responseData) {
    bool bHandled = false;
    const uint32_t timestamp = millis();
    for (auto& request : m_requests) {
        if (!request.isSetter() && request.isMatch(can_id, responseData)) {
            request.handle(accessor, can_id, responseData, timestamp);
            bHandled = true;
        }
    }
    if (!bHandled) {
        Utils::log("unhandled", "can_id<%s> data<%s>", Utils::to_hex(can_id).c_str(), Utils::to_hex(responseData).c_str());
    }
}

TRequest* TRequests::getNextRequestToSend() {
    const uint32_t timestamp = millis();
    const uint32_t interval = static_cast<uint32_t>(10/*id(update_interval).state*/) * 1000;

    for (auto& request : m_requests) {
        if (request.hasSendGet()) {
            if ((timestamp > (request.getLastUpdate() + interval)) && !request.inProgress()) {
                return &request;
            }
        }
    }
    return nullptr;
}

}
}