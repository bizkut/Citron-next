// SPDX-FileCopyrightText: Copyright 2024 citron Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "core/hle/service/am/service/cradle_firmware_updater.h"
#include "core/hle/service/am/service/global_state_controller.h"
#include "core/hle/service/cmif_serialization.h"

namespace Service::AM {

IGlobalStateController::IGlobalStateController(Core::System& system_)
    : ServiceFramework{system_, "IGlobalStateController"},
      m_context{system_, "IGlobalStateController"}, m_hdcp_authentication_failed_event{m_context} {
    // clang-format off
    static const FunctionInfo functions[] = {
        {0, D<&IGlobalStateController::RequestToEnterSleep>, "RequestToEnterSleep"},
        {1, D<&IGlobalStateController::EnterSleep>, "EnterSleep"},
        {2, D<&IGlobalStateController::StartSleepSequence>, "StartSleepSequence"},
        {3, D<&IGlobalStateController::StartShutdownSequence>, "StartShutdownSequence"},
        {4, D<&IGlobalStateController::StartRebootSequence>, "StartRebootSequence"},
        {9, D<&IGlobalStateController::IsAutoPowerDownRequested>, "IsAutoPowerDownRequested"},
        {10, D<&IGlobalStateController::LoadAndApplyIdlePolicySettings>, "LoadAndApplyIdlePolicySettings"},
        {11, D<&IGlobalStateController::NotifyCecSettingsChanged>, "NotifyCecSettingsChanged"},
        {12, D<&IGlobalStateController::SetDefaultHomeButtonLongPressTime>, "SetDefaultHomeButtonLongPressTime"},
        {13, D<&IGlobalStateController::UpdateDefaultDisplayResolution>, "UpdateDefaultDisplayResolution"},
        {14, D<&IGlobalStateController::ShouldSleepOnBoot>, "ShouldSleepOnBoot"},
        {15, D<&IGlobalStateController::GetHdcpAuthenticationFailedEvent>, "GetHdcpAuthenticationFailedEvent"},
        {30, D<&IGlobalStateController::OpenCradleFirmwareUpdater>, "OpenCradleFirmwareUpdater"},
    };
    // clang-format on

    RegisterHandlers(functions);
}

IGlobalStateController::~IGlobalStateController() = default;

Result IGlobalStateController::LoadAndApplyIdlePolicySettings() {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    R_SUCCEED();
}

Result IGlobalStateController::ShouldSleepOnBoot(Out<bool> out_should_sleep_on_boot) {
    LOG_INFO(Service_AM, "called");
    *out_should_sleep_on_boot = false;
    R_SUCCEED();
}

Result IGlobalStateController::GetHdcpAuthenticationFailedEvent(
    OutCopyHandle<Kernel::KReadableEvent> out_event) {
    LOG_INFO(Service_AM, "called");
    *out_event = m_hdcp_authentication_failed_event.GetHandle();
    R_SUCCEED();
}

Result IGlobalStateController::OpenCradleFirmwareUpdater(
    Out<SharedPointer<ICradleFirmwareUpdater>> out_cradle_firmware_updater) {
    LOG_INFO(Service_AM, "called");
    *out_cradle_firmware_updater = std::make_shared<ICradleFirmwareUpdater>(system);
    R_SUCCEED();
}

Result IGlobalStateController::RequestToEnterSleep() {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    R_SUCCEED();
}

Result IGlobalStateController::EnterSleep() {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    R_SUCCEED();
}

Result IGlobalStateController::StartSleepSequence() {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    R_SUCCEED();
}

Result IGlobalStateController::StartShutdownSequence() {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    R_SUCCEED();
}

Result IGlobalStateController::StartRebootSequence() {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    R_SUCCEED();
}

Result IGlobalStateController::IsAutoPowerDownRequested(Out<bool> out_is_requested) {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    *out_is_requested = false;
    R_SUCCEED();
}

Result IGlobalStateController::NotifyCecSettingsChanged() {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    R_SUCCEED();
}

Result IGlobalStateController::SetDefaultHomeButtonLongPressTime(u64 time_in_ms) {
    LOG_WARNING(Service_AM, "(STUBBED) called, time_in_ms={}", time_in_ms);
    R_SUCCEED();
}

Result IGlobalStateController::UpdateDefaultDisplayResolution() {
    LOG_WARNING(Service_AM, "(STUBBED) called");
    R_SUCCEED();
}

} // namespace Service::AM
