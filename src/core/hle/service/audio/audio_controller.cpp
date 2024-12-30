// SPDX-FileCopyrightText: Copyright 2018 citron Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/logging/log.h"
#include "core/hle/service/audio/audio_controller.h"
#include "core/hle/service/cmif_serialization.h"
#include "core/hle/service/ipc_helpers.h"
#include "core/hle/service/set/system_settings_server.h"
#include "core/hle/service/sm/sm.h"

namespace Service::Audio {

IAudioController::IAudioController(Core::System& system_)
    : ServiceFramework{system_, "audctl"}, service_context{system_, "audctl"} {
    // Create notification event
    notification_event = service_context.CreateEvent("AudioController:NotificationEvent");

    // clang-format off
    static const FunctionInfo functions[] = {
        {0, D<&IAudioController::GetTargetVolume>, "GetTargetVolume"},
        {1, D<&IAudioController::SetTargetVolume>, "SetTargetVolume"},
        {2, D<&IAudioController::GetTargetVolumeMin>, "GetTargetVolumeMin"},
        {3, D<&IAudioController::GetTargetVolumeMax>, "GetTargetVolumeMax"},
        {4, D<&IAudioController::IsTargetMute>, "IsTargetMute"},
        {5, D<&IAudioController::SetTargetMute>, "SetTargetMute"},
        {6, D<&IAudioController::IsTargetConnected>, "IsTargetConnected"},
        {7, D<&IAudioController::SetDefaultTarget>, "SetDefaultTarget"},
        {8, D<&IAudioController::GetDefaultTarget>, "GetDefaultTarget"},
        {9, D<&IAudioController::GetAudioOutputMode>, "GetAudioOutputMode"},
        {10, D<&IAudioController::SetAudioOutputMode>, "SetAudioOutputMode"},
        {11, nullptr, "SetForceMutePolicy"},
        {12, D<&IAudioController::GetForceMutePolicy>, "GetForceMutePolicy"},
        {13, D<&IAudioController::GetOutputModeSetting>, "GetOutputModeSetting"},
        {14, D<&IAudioController::SetOutputModeSetting>, "SetOutputModeSetting"},
        {15, nullptr, "SetOutputTarget"},
        {16, nullptr, "SetInputTargetForceEnabled"},
        {17, D<&IAudioController::SetHeadphoneOutputLevelMode>, "SetHeadphoneOutputLevelMode"},
        {18, D<&IAudioController::GetHeadphoneOutputLevelMode>, "GetHeadphoneOutputLevelMode"},
        {19, nullptr, "AcquireAudioVolumeUpdateEventForPlayReport"},
        {20, nullptr, "AcquireAudioOutputDeviceUpdateEventForPlayReport"},
        {21, nullptr, "GetAudioOutputTargetForPlayReport"},
        {22, D<&IAudioController::NotifyHeadphoneVolumeWarningDisplayedEvent>, "NotifyHeadphoneVolumeWarningDisplayedEvent"},
        {23, nullptr, "SetSystemOutputMasterVolume"},
        {24, nullptr, "GetSystemOutputMasterVolume"},
        {25, nullptr, "GetAudioVolumeDataForPlayReport"},
        {26, nullptr, "UpdateHeadphoneSettings"},
        {27, D<&IAudioController::SetVolumeMappingTableForDev>, "SetVolumeMappingTableForDev"},
        {28, nullptr, "GetAudioOutputChannelCountForPlayReport"},
        {29, nullptr, "BindAudioOutputChannelCountUpdateEventForPlayReport"},
        {30, D<&IAudioController::SetSpeakerAutoMuteEnabled>, "SetSpeakerAutoMuteEnabled"},
        {31, D<&IAudioController::IsSpeakerAutoMuteEnabled>, "IsSpeakerAutoMuteEnabled"},
        {32, nullptr, "GetActiveOutputTarget"},
        {33, nullptr, "GetTargetDeviceInfo"},
        {34, D<&IAudioController::AcquireTargetNotification>, "AcquireTargetNotification"},
        {35, D<&IAudioController::SetHearingProtectionSafeguardTimerRemainingTimeForDebug>, "SetHearingProtectionSafeguardTimerRemainingTimeForDebug"},
        {36, D<&IAudioController::GetHearingProtectionSafeguardTimerRemainingTimeForDebug>, "GetHearingProtectionSafeguardTimerRemainingTimeForDebug"},
        {37, D<&IAudioController::SetHearingProtectionSafeguardEnabled>, "SetHearingProtectionSafeguardEnabled"},
        {38, D<&IAudioController::IsHearingProtectionSafeguardEnabled>, "IsHearingProtectionSafeguardEnabled"},
        {39, D<&IAudioController::IsHearingProtectionSafeguardMonitoringOutputForDebug>, "IsHearingProtectionSafeguardMonitoringOutputForDebug"},
        {40, D<&IAudioController::GetSystemInformationForDebug>, "GetSystemInformationForDebug"},
        {41, D<&IAudioController::SetVolumeButtonLongPressTime>, "SetVolumeButtonLongPressTime"},
        {42, D<&IAudioController::SetNativeVolumeForDebug>, "SetNativeVolumeForDebug"},
        {10000, D<&IAudioController::NotifyAudioOutputTargetForPlayReport>, "NotifyAudioOutputTargetForPlayReport"},
        {10001, D<&IAudioController::NotifyAudioOutputChannelCountForPlayReport>, "NotifyAudioOutputChannelCountForPlayReport"},
        {10002, D<&IAudioController::NotifyUnsupportedUsbOutputDeviceAttachedForPlayReport>, "NotifyUnsupportedUsbOutputDeviceAttachedForPlayReport"},
        {10100, D<&IAudioController::GetAudioVolumeDataForPlayReport>, "GetAudioVolumeDataForPlayReport"},
        {10101, D<&IAudioController::BindAudioVolumeUpdateEventForPlayReport>, "BindAudioVolumeUpdateEventForPlayReport"},
        {10102, D<&IAudioController::BindAudioOutputTargetUpdateEventForPlayReport>, "BindAudioOutputTargetUpdateEventForPlayReport"},
        {10103, D<&IAudioController::GetAudioOutputTargetForPlayReport>, "GetAudioOutputTargetForPlayReport"},
        {10104, D<&IAudioController::GetAudioOutputChannelCountForPlayReport>, "GetAudioOutputChannelCountForPlayReport"},
        {10105, D<&IAudioController::BindAudioOutputChannelCountUpdateEventForPlayReport>, "BindAudioOutputChannelCountUpdateEventForPlayReport"},
        {10106, D<&IAudioController::GetDefaultAudioOutputTargetForPlayReport>, "GetDefaultAudioOutputTargetForPlayReport"},
        {50000, D<&IAudioController::SetAnalogInputBoostGainForPrototyping>, "SetAnalogInputBoostGainForPrototyping"},
        {50001, D<&IAudioController::OverrideDefaultTargetForDebug>, "OverrideDefaultTargetForDebug"},
        {50003, D<&IAudioController::SetForceOverrideExternalDeviceNameForDebug>, "SetForceOverrideExternalDeviceNameForDebug"},
        {50004, D<&IAudioController::ClearForceOverrideExternalDeviceNameForDebug>, "ClearForceOverrideExternalDeviceNameForDebug"},
    };
    // clang-format on

    RegisterHandlers(functions);

    m_set_sys =
        system.ServiceManager().GetService<Service::Set::ISystemSettingsServer>("set:sys", true);
}

IAudioController::~IAudioController() {
    service_context.CloseEvent(notification_event);
};

Result IAudioController::GetTargetVolumeMin(Out<s32> out_target_min_volume) {
    LOG_DEBUG(Audio, "called.");

    // This service function is currently hardcoded on the
    // actual console to this value (as of 8.0.0).
    *out_target_min_volume = 0;
    R_SUCCEED();
}

Result IAudioController::GetTargetVolumeMax(Out<s32> out_target_max_volume) {
    LOG_DEBUG(Audio, "called.");

    // This service function is currently hardcoded on the
    // actual console to this value (as of 8.0.0).
    *out_target_max_volume = 15;
    R_SUCCEED();
}

Result IAudioController::GetAudioOutputMode(Out<Set::AudioOutputMode> out_output_mode,
                                            Set::AudioOutputModeTarget target) {
    const auto result = m_set_sys->GetAudioOutputMode(out_output_mode, target);

    LOG_INFO(Service_SET, "called, target={}, output_mode={}", target, *out_output_mode);
    R_RETURN(result);
}

Result IAudioController::SetAudioOutputMode(Set::AudioOutputModeTarget target,
                                            Set::AudioOutputMode output_mode) {
    LOG_INFO(Service_SET, "called, target={}, output_mode={}", target, output_mode);

    R_RETURN(m_set_sys->SetAudioOutputMode(target, output_mode));
}

Result IAudioController::GetForceMutePolicy(Out<ForceMutePolicy> out_mute_policy) {
    LOG_WARNING(Audio, "(STUBBED) called");

    // Removed on FW 13.2.1+
    *out_mute_policy = ForceMutePolicy::Disable;
    R_SUCCEED();
}

Result IAudioController::GetOutputModeSetting(Out<Set::AudioOutputMode> out_output_mode,
                                              Set::AudioOutputModeTarget target) {
    LOG_WARNING(Audio, "(STUBBED) called, target={}", target);

    *out_output_mode = Set::AudioOutputMode::ch_7_1;
    R_SUCCEED();
}

Result IAudioController::SetOutputModeSetting(Set::AudioOutputModeTarget target,
                                              Set::AudioOutputMode output_mode) {
    LOG_INFO(Service_SET, "called, target={}, output_mode={}", target, output_mode);
    R_SUCCEED();
}

Result IAudioController::SetHeadphoneOutputLevelMode(HeadphoneOutputLevelMode output_level_mode) {
    LOG_WARNING(Audio, "(STUBBED) called, output_level_mode={}", output_level_mode);
    R_SUCCEED();
}

Result IAudioController::GetHeadphoneOutputLevelMode(
    Out<HeadphoneOutputLevelMode> out_output_level_mode) {
    LOG_INFO(Audio, "called");

    *out_output_level_mode = HeadphoneOutputLevelMode::Normal;
    R_SUCCEED();
}

Result IAudioController::NotifyHeadphoneVolumeWarningDisplayedEvent() {
    LOG_WARNING(Service_Audio, "(STUBBED) called");
    R_SUCCEED();
}

Result IAudioController::SetSpeakerAutoMuteEnabled(bool is_speaker_auto_mute_enabled) {
    LOG_INFO(Audio, "called, is_speaker_auto_mute_enabled={}", is_speaker_auto_mute_enabled);

    R_RETURN(m_set_sys->SetSpeakerAutoMuteFlag(is_speaker_auto_mute_enabled));
}

Result IAudioController::IsSpeakerAutoMuteEnabled(Out<bool> out_is_speaker_auto_mute_enabled) {
    const auto result = m_set_sys->GetSpeakerAutoMuteFlag(out_is_speaker_auto_mute_enabled);

    LOG_INFO(Audio, "called, is_speaker_auto_mute_enabled={}", *out_is_speaker_auto_mute_enabled);
    R_RETURN(result);
}

Result IAudioController::AcquireTargetNotification(
    OutCopyHandle<Kernel::KReadableEvent> out_notification_event) {
    LOG_WARNING(Service_AM, "(STUBBED) called");

    *out_notification_event = &notification_event->GetReadableEvent();
    R_SUCCEED();
}

Result IAudioController::GetTargetVolume(Out<s32> out_target_volume) {
    LOG_DEBUG(Audio, "called.");
    // Default volume level, similar to GetTargetVolumeMax()
    *out_target_volume = 15;
    R_SUCCEED();
}

Result IAudioController::SetTargetVolume(s32 target_volume) {
    LOG_INFO(Audio, "called, target_volume={}", target_volume);
    R_SUCCEED();
}

Result IAudioController::IsTargetMute(Out<bool> out_is_muted) {
    LOG_DEBUG(Audio, "called.");
    *out_is_muted = false;
    R_SUCCEED();
}

Result IAudioController::SetTargetMute(bool is_muted) {
    LOG_INFO(Audio, "called, is_muted={}", is_muted);
    R_SUCCEED();
}

Result IAudioController::IsTargetConnected(Out<bool> out_is_connected) {
    LOG_DEBUG(Audio, "called.");
    *out_is_connected = true;
    R_SUCCEED();
}

Result IAudioController::SetDefaultTarget(AudioOutputTarget target) {
    LOG_INFO(Audio, "called, target={}", target);
    R_SUCCEED();
}

Result IAudioController::GetDefaultTarget(Out<AudioOutputTarget> out_target) {
    LOG_DEBUG(Audio, "called.");
    *out_target = AudioOutputTarget::BuiltInSpeaker;
    R_SUCCEED();
}

Result IAudioController::SetVolumeButtonLongPressTime(u32 press_time_ms) {
    LOG_INFO(Audio, "called, press_time_ms={}", press_time_ms);
    R_SUCCEED();
}

Result IAudioController::SetNativeVolumeForDebug(float volume) {
    LOG_INFO(Audio, "called, volume={}", volume);
    R_SUCCEED();
}

Result IAudioController::NotifyAudioOutputTargetForPlayReport() {
    LOG_WARNING(Audio, "(STUBBED) called");
    R_SUCCEED();
}

Result IAudioController::NotifyAudioOutputChannelCountForPlayReport() {
    LOG_WARNING(Audio, "(STUBBED) called");
    R_SUCCEED();
}

Result IAudioController::NotifyUnsupportedUsbOutputDeviceAttachedForPlayReport() {
    LOG_WARNING(Audio, "(STUBBED) called");
    R_SUCCEED();
}

Result IAudioController::GetAudioVolumeDataForPlayReport(Out<AudioVolumeReportData> out_report_data) {
    LOG_WARNING(Audio, "(STUBBED) called");
    std::memset(out_report_data, 0, sizeof(AudioVolumeReportData));
    out_report_data->volume = 15;  // Default max volume
    R_SUCCEED();
}

Result IAudioController::BindAudioVolumeUpdateEventForPlayReport(
    OutCopyHandle<Kernel::KReadableEvent> out_event) {
    LOG_WARNING(Audio, "(STUBBED) called");
    *out_event = &notification_event->GetReadableEvent();
    R_SUCCEED();
}

Result IAudioController::BindAudioOutputTargetUpdateEventForPlayReport(
    OutCopyHandle<Kernel::KReadableEvent> out_event) {
    LOG_WARNING(Audio, "(STUBBED) called");
    *out_event = &notification_event->GetReadableEvent();
    R_SUCCEED();
}

Result IAudioController::GetAudioOutputTargetForPlayReport(Out<AudioOutputTarget> out_target) {
    LOG_DEBUG(Audio, "called");
    *out_target = AudioOutputTarget::BuiltInSpeaker;
    R_SUCCEED();
}

Result IAudioController::GetAudioOutputChannelCountForPlayReport(Out<u32> out_channel_count) {
    LOG_DEBUG(Audio, "called");
    *out_channel_count = 2;  // Default to stereo
    R_SUCCEED();
}

Result IAudioController::BindAudioOutputChannelCountUpdateEventForPlayReport(
    OutCopyHandle<Kernel::KReadableEvent> out_event) {
    LOG_WARNING(Audio, "(STUBBED) called");
    *out_event = &notification_event->GetReadableEvent();
    R_SUCCEED();
}

Result IAudioController::GetDefaultAudioOutputTargetForPlayReport(Out<AudioOutputTarget> out_target) {
    LOG_DEBUG(Audio, "called");
    *out_target = AudioOutputTarget::BuiltInSpeaker;
    R_SUCCEED();
}

Result IAudioController::SetHearingProtectionSafeguardTimerRemainingTimeForDebug(u32 remaining_time) {
    LOG_WARNING(Audio, "(STUBBED) called, remaining_time={}", remaining_time);
    R_SUCCEED();
}

Result IAudioController::GetHearingProtectionSafeguardTimerRemainingTimeForDebug(Out<u32> out_remaining_time) {
    LOG_WARNING(Audio, "(STUBBED) called");
    *out_remaining_time = 0;
    R_SUCCEED();
}

Result IAudioController::SetHearingProtectionSafeguardEnabled(bool enabled) {
    LOG_WARNING(Audio, "(STUBBED) called, enabled={}", enabled);
    R_SUCCEED();
}

Result IAudioController::IsHearingProtectionSafeguardEnabled(Out<bool> out_enabled) {
    LOG_WARNING(Audio, "(STUBBED) called");
    *out_enabled = false;
    R_SUCCEED();
}

Result IAudioController::IsHearingProtectionSafeguardMonitoringOutputForDebug(Out<bool> out_monitoring) {
    LOG_WARNING(Audio, "(STUBBED) called");
    *out_monitoring = false;
    R_SUCCEED();
}

Result IAudioController::SetVolumeMappingTableForDev(InBuffer<BufferAttr_HipcMapAlias> volume_table) {
    LOG_WARNING(Audio, "(STUBBED) called, table_size={}", volume_table.size());
    R_SUCCEED();
}

Result IAudioController::SetAnalogInputBoostGainForPrototyping(float gain) {
    LOG_WARNING(Audio, "(STUBBED) called, gain={}", gain);
    R_SUCCEED();
}

Result IAudioController::GetSystemInformationForDebug(OutBuffer<BufferAttr_HipcMapAlias> out_info) {
    LOG_WARNING(Audio, "(STUBBED) called");
    std::string debug_info = "Audio Debug Information";
    std::memcpy(out_info.data(), debug_info.data(),
                std::min(debug_info.size(), static_cast<size_t>(out_info.size())));
    R_SUCCEED();
}

Result IAudioController::OverrideDefaultTargetForDebug(AudioOutputTarget target) {
    LOG_WARNING(Audio, "(STUBBED) called, target={}", target);
    R_SUCCEED();
}

Result IAudioController::SetForceOverrideExternalDeviceNameForDebug(InBuffer<BufferAttr_HipcMapAlias> device_name) {
    LOG_WARNING(Audio, "(STUBBED) called, device_name_size={}", device_name.size());
    R_SUCCEED();
}

Result IAudioController::ClearForceOverrideExternalDeviceNameForDebug() {
    LOG_WARNING(Audio, "(STUBBED) called");
    R_SUCCEED();
}

} // namespace Service::Audio
