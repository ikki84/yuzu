// Copyright 2018 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "common/common_types.h"
#include "video_core/engines/maxwell_3d.h"
#include "video_core/shader/shader_ir.h"

namespace VideoCommon::Shader {
class ShaderIR;
}

namespace OpenGL::GLShader {

struct ShaderEntries;

using Maxwell = Tegra::Engines::Maxwell3D::Regs;
using ProgramResult = std::pair<std::string, ShaderEntries>;
using SamplerEntry = VideoCommon::Shader::Sampler;

class ConstBufferEntry : public VideoCommon::Shader::ConstBuffer {
public:
    explicit ConstBufferEntry(u32 max_offset, bool is_indirect, u32 index)
        : VideoCommon::Shader::ConstBuffer{max_offset, is_indirect}, index{index} {}

    u32 GetIndex() const {
        return index;
    }

private:
    u32 index{};
};

class GlobalMemoryEntry {
public:
    explicit GlobalMemoryEntry(u32 cbuf_index, u32 cbuf_offset)
        : cbuf_index{cbuf_index}, cbuf_offset{cbuf_offset} {}

    u32 GetCbufIndex() const {
        return cbuf_index;
    }

    u32 GetCbufOffset() const {
        return cbuf_offset;
    }

private:
    u32 cbuf_index{};
    u32 cbuf_offset{};
};

struct ShaderEntries {
    std::vector<ConstBufferEntry> const_buffers;
    std::vector<SamplerEntry> samplers;
    std::vector<GlobalMemoryEntry> global_memory_entries;
    std::array<bool, Maxwell::NumClipDistances> clip_distances{};
    std::size_t shader_length{};
};

std::string GetCommonDeclarations();

ProgramResult Decompile(const VideoCommon::Shader::ShaderIR& ir, Maxwell::ShaderStage stage,
                        const std::string& suffix);

} // namespace OpenGL::GLShader