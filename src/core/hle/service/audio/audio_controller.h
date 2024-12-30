// SPDX-FileCopyrightText: Copyright 2018 citron Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "core/hle/service/cmif_types.h"
#include "core/hle/service/service.h"
#include "core/hle/service/set/settings_types.h"

namespace Core {
class System;
}

namespace Service::Set {
class ISystemSettingsServer;
}

namespace Service::Audio {

enum class ForceMutePolicy {
    Disable = 0,
    Enable = 1,
};

enum class HeadphoneOutputLevelMode {
    Normal,
    HighPower,
};

enum class AudioOutputTarget {
    BuiltInSpeaker = 0,
    Headphones = 1,
    HDMI = 2,
    // Add other targets if needed
};

struct DeviceInfo {
    bool is_connected;
    bool is_connected_usb;
    u32 channel_count;
    // Add other device info fields as needed
};

struct AudioVolumeReportData {
    u32 volume;
    // Add other report data fields as needed
};

class IAudioController final : public ServiceFramework<IAudioController> {
public:
    explicit IAudioController(Core::System& system_);
    ~IAudioController() override;

    Result SetForceMutePolicy(ForceMutePolicy mute_policy);
    Result SetOutputTarget(AudioOutputTarget target);
    Result SetInputTargetForceEnabled(bool enabled);
    Result SetSystemOutputMasterVolume(float volume);
    Result GetSystemOutputMasterVolume(Out<float> out_volume);
    Result GetActiveOutputTarget(Out<AudioOutputTarget> out_target);
    Result GetTargetDeviceInfo(Out<DeviceInfo> out_device_info);
    Result SetHearingProtectionSafeguardTimerRemainingTimeForDebug(u32 remaining_time);
    Result GetHearingProtectionSafeguardTimerRemainingTimeForDebug(Out<u32> out_remaining_time);
    Result SetHearingProtectionSafeguardEnabled(bool enabled);
    Result IsHearingProtectionSafeguardEnabled(Out<bool> out_enabled);
    Result IsHearingProtectionSafeguardMonitoringOutputForDebug(Out<bool> out_monitoring);
    Result SetVolumeMappingTableForDev(InBuffer<BufferAttr_HipcMapAlias> volume_table);
    Result SetAnalogInputBoostGainForPrototyping(float gain);
    Result GetSystemInformationForDebug(OutBuffer<BufferAttr_HipcMapAlias> out_info);
    Result OverrideDefaultTargetForDebug(AudioOutputTarget target);
    Result SetForceOverrideExternalDeviceNameForDebug(InBuffer<BufferAttr_HipcMapAlias> device_name);
    Result ClearForceOverrideExternalDeviceNameForDebug();
    Result GetTargetVolume(Out<s32> out_target_volume);
    Result SetTargetVolume(s32 target_volume);
    Result IsTargetMute(Out<bool> out_is_muted);
    Result SetTargetMute(bool is_muted);
    Result IsTargetConnected(Out<bool> out_is_connected);
    Result SetDefaultTarget(AudioOutputTarget target);
    Result GetDefaultTarget(Out<AudioOutputTarget> out_target);
    Result SetVolumeButtonLongPressTime(u32 press_time_ms);
    Result SetNativeVolumeForDebug(float volume);
    Result NotifyAudioOutputTargetForPlayReport();
    Result NotifyAudioOutputChannelCountForPlayReport();
    Result NotifyUnsupportedUsbOutputDeviceAttachedForPlayReport();
    Result GetAudioVolumeDataForPlayReport(Out<AudioVolumeReportData> out_report_data);
    Result BindAudioVolumeUpdateEventForPlayReport(OutCopyHandle<Kernel::KReadableEvent> out_event);
    Result BindAudioOutputTargetUpdateEventForPlayReport(OutCopyHandle<Kernel::KReadableEvent> out_event);
    Result GetAudioOutputTargetForPlayReport(Out<AudioOutputTarget> out_target);
    Result GetAudioOutputChannelCountForPlayReport(Out<u32> out_channel_count);
    Result BindAudioOutputChannelCountUpdateEventForPlayReport(OutCopyHandle<Kernel::KReadableEvent> out_event);
    Result GetDefaultAudioOutputTargetForPlayReport(Out<AudioOutputTarget> out_target);

private:
    Result GetTargetVolumeMin(Out<s32> out_target_min_volume);
    Result GetTargetVolumeMax(Out<s32> out_target_max_volume);
    Result GetAudioOutputMode(Out<Set::AudioOutputMode> out_output_mode,
                              Set::AudioOutputModeTarget target);
    Result SetAudioOutputMode(Set::AudioOutputModeTarget target, Set::AudioOutputMode output_mode);
    Result GetForceMutePolicy(Out<ForceMutePolicy> out_mute_policy);
    Result GetOutputModeSetting(Out<Set::AudioOutputMode> out_output_mode,
                                Set::AudioOutputModeTarget target);
    Result SetOutputModeSetting(Set::AudioOutputModeTarget target,
                                Set::AudioOutputMode output_mode);
    Result SetHeadphoneOutputLevelMode(HeadphoneOutputLevelMode output_level_mode);
    Result GetHeadphoneOutputLevelMode(Out<HeadphoneOutputLevelMode> out_output_level_mode);
    Result NotifyHeadphoneVolumeWarningDisplayedEvent();
    Result SetSpeakerAutoMuteEnabled(bool is_speaker_auto_mute_enabled);
    Result IsSpeakerAutoMuteEnabled(Out<bool> out_is_speaker_auto_mute_enabled);
    Result AcquireTargetNotification(OutCopyHandle<Kernel::KReadableEvent> out_notification_event);

    KernelHelpers::ServiceContext service_context;

    Kernel::KEvent* notification_event;
    std::shared_ptr<Service::Set::ISystemSettingsServer> m_set_sys;
};

} // namespace Service::Audio
