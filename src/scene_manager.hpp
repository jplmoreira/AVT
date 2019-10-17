#pragma once

#include <vector>
#include <memory>

#include "object.hpp"

class scene_manager {
public:
    std::vector<std::unique_ptr<object>> objs;

    void prepare_scene();
};