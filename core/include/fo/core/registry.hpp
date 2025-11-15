#pragma once

#include "interfaces.hpp"
#include <memory>
#include <unordered_map>
#include <functional>

namespace fo::core {

template <typename T>
class Registry {
public:
    using Factory = std::function<std::unique_ptr<T>()>;

    static Registry& instance() {
        static Registry r;
        return r;
    }

    void add(const std::string& name, Factory f) {
        factories_[name] = std::move(f);
    }

    std::unique_ptr<T> create(const std::string& name) const {
        auto it = factories_.find(name);
        if (it == factories_.end()) return nullptr;
        return (it->second)();
    }

    std::vector<std::string> names() const {
        std::vector<std::string> out;
        out.reserve(factories_.size());
        for (auto& kv : factories_) out.push_back(kv.first);
        return out;
    }

private:
    std::unordered_map<std::string, Factory> factories_;
};

} // namespace fo::core
