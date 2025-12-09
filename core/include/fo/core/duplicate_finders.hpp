#pragma once

#include "fo/core/interfaces.hpp"

namespace fo::core {

class SizeHashDuplicateFinder : public IDuplicateFinder {
public:
    std::string name() const override { return "size_hash"; }
    std::vector<DuplicateGroup> group(const std::vector<FileInfo>& files, IHasher& hasher) override;
};

class SizeHashByteDuplicateFinder : public IDuplicateFinder {
public:
    std::string name() const override { return "size_hash_byte"; }
    std::vector<DuplicateGroup> group(const std::vector<FileInfo>& files, IHasher& hasher) override;
};

}
