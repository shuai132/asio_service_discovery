#pragma once

#include <string>
#include <functional>
#include <cstdint>

#include "service_discoverer.hpp"

class ServiceDiscovery {
    using FindServiceHandle = std::function<void(std::string ip, uint16_t port)>;
    using service_discoverer = betabugs::networking::service_discoverer;

public:
    ServiceDiscovery(asio::io_context& io_service, const std::string& service_name, FindServiceHandle handle)
            : discoverer_(io_service, service_name
            , [this](const service_discoverer::services& services){
                // 找到的第一个IP作为发现的ip
                if (services.find(lastService_) != services.cend()) {
                    // 已经更新过
                    return;
                } else {
                    for(const auto& service : services) {
                        lastService_ = service;
                        findServiceHandle_(service.endpoint.address().to_string(), service.endpoint.port());
                        return;
                    }
                }
            })
            , findServiceHandle_(std::move(handle))
    {
    }

private:
    service_discoverer discoverer_;
    FindServiceHandle findServiceHandle_;
    service_discoverer::service lastService_;
};
