#include "esphome/components/daikin_rotex_control/requests.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace dakin_rotex_control {

TRequests::TRequests(IPublisher* pPublisher, std::vector<esphome::dakin_rotex_control::TRequest> const& requests)
: m_requests(requests)
, m_pCanBus(nullptr)
{
    for (auto& request : m_requests) {
        request.setPublisher(pPublisher);
    }
}

bool TRequests::sendNextPendingGet() {
    TRequest* pRequest = getNextRequestToSend();
    if (pRequest != nullptr) {
        pRequest->sendGet(m_pCanBus);
        return true;
    }
    return false;
}

void TRequests::sendGet(std::string const& request_name) {
    const auto it = std::find_if(m_requests.begin(), m_requests.end(),
        [& request_name](auto& request) { return request.getName() == request_name; });

    if (it != m_requests.end()) {
        it->sendGet(m_pCanBus);
    } else {
        Utils::log("requests.cpp", "sendGet(%s) -> Unknown request!", request_name.c_str());
    }
}

void TRequests::sendSet(std::string const& request_name, float value) {
    const auto it = std::find_if(m_requests.begin(), m_requests.end(),
        [& request_name](auto& request) { return request.getName() == request_name; }
    );
    if (it != m_requests.end()) {
        it->sendSet(m_pCanBus, value);
    } else {
        Utils::log("requests.cpp", "sendSet(%s) -> Unknown request!", request_name.c_str());
    }
}

void TRequests::handle(uint32_t can_id, std::vector<uint8_t> const& responseData, Accessor& accessor) {
    bool bHandled = false;
    const uint32_t timestamp = millis();
    for (auto& request : m_requests) {
        if (request.isMatch(can_id, responseData)) {
            request.handle(can_id, responseData, timestamp, accessor);
            bHandled = true;
        }
    }
    if (!bHandled) {
        Utils::log("requests.cpp", "unhandled: can_id<%s> data<%s>", Utils::to_hex(can_id).c_str(), Utils::to_hex(responseData).c_str());
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