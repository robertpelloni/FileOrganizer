#pragma once

#include "fo/core/interfaces.hpp"

namespace fo::providers {

    class DHash : public fo::core::IHasher {
    public:
        DHash();
        ~DHash() override;

        std::string name() const override;
        std::string fast64(const std::filesystem::path& file_path) override;
    };

} // namespace fo::providers
