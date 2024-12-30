// SPDX-FileCopyrightText: Copyright 2023 citron Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "audio_core/adsp/apps/audio_renderer/audio_renderer.h"
#include "audio_core/adsp/apps/opus/opus_decoder.h"
#include "common/common_types.h"

namespace Core {
class System;
} // namespace Core

namespace AudioCore {
namespace Sink {
class Sink;
}

namespace ADSP {

/**
 * Represents the ADSP embedded within the audio sysmodule.
 * This is a 32-bit Linux4Tegra kernel from nVidia, which is launched with the sysmodule on boot.
 *
 * The kernel will run the apps you write for it, Nintendo have the following:
 *
 * Gmix - Responsible for mixing final audio and sending it out to hardware. This is last place all
 *        audio samples end up, and we skip it entirely, since we have very different backends and
 *        mixing is implicitly handled by the OS (but also due to lack of research/simplicity).
 *
 * AudioRenderer - Receives command lists generated by the audio render
 *                 system on the host, processes them, and sends the samples to Gmix.
 *
 * OpusDecoder - Contains libopus, and decodes Opus audio packets into raw pcm data.
 *
 * Communication between the host and ADSP is done through mailboxes, and mapping of shared memory.
 */
class ADSP {
public:
    explicit ADSP(Core::System& system, Sink::Sink& sink);
    ~ADSP() = default;

    AudioRenderer::AudioRenderer& AudioRenderer();
    OpusDecoder::OpusDecoder& OpusDecoder();

private:
    /// AudioRenderer app
    std::unique_ptr<AudioRenderer::AudioRenderer> audio_renderer{};
    std::unique_ptr<OpusDecoder::OpusDecoder> opus_decoder{};
};

} // namespace ADSP
} // namespace AudioCore
