/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "sec_comp_kit.h"

#include "hisysevent.h"
#include "ipc_skeleton.h"
#include "sec_comp_caller_authorization.h"
#include "sec_comp_client.h"
#include "sec_comp_dialog_callback.h"
#include "sec_comp_enhance_adapter.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompKit"};
}  // namespace

int32_t SecCompKit::RegisterSecurityComponent(SecCompType type,
    std::string& componentInfo, int32_t& scId)
{
    if (!SecCompCallerAuthorization::GetInstance().IsKitCaller(
        reinterpret_cast<uintptr_t>(__builtin_return_address(0)))) {
        SC_LOG_ERROR(LABEL, "register security component fail, caller invalid");
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "CALLER_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingRealPid(), "CALL_SCENE", "REGITSTER");
        return SC_SERVICE_ERROR_CALLER_INVALID;
    }

    if (!SecCompEnhanceAdapter::EnhanceDataPreprocess(componentInfo)) {
        SC_LOG_ERROR(LABEL, "Preprocess security component fail");
        return SC_ENHANCE_ERROR_VALUE_INVALID;
    }

    int32_t res = SecCompClient::GetInstance().RegisterSecurityComponent(type, componentInfo, scId);
    if (res != SC_OK) {
        SC_LOG_ERROR(LABEL, "register security component fail, error: %{public}d", res);
        return res;
    }
    SecCompEnhanceAdapter::RegisterScIdEnhance(scId);
    HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "REGISTER_SUCCESS",
        HiviewDFX::HiSysEvent::EventType::BEHAVIOR, "CALLER_UID", IPCSkeleton::GetCallingUid(),
        "CALLER_PID", IPCSkeleton::GetCallingRealPid(), "SC_ID", scId, "SC_TYPE", type);
    return res;
}

int32_t SecCompKit::UpdateSecurityComponent(int32_t scId, std::string& componentInfo)
{
    if (!SecCompCallerAuthorization::GetInstance().IsKitCaller(
        reinterpret_cast<uintptr_t>(__builtin_return_address(0)))) {
        SC_LOG_ERROR(LABEL, "update security component fail, caller invalid");
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "CALLER_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingRealPid(), "CALL_SCENE", "UPDATE");
        return SC_SERVICE_ERROR_CALLER_INVALID;
    }

    if (!SecCompEnhanceAdapter::EnhanceDataPreprocess(scId, componentInfo)) {
        SC_LOG_ERROR(LABEL, "Preprocess security component fail");
        return SC_ENHANCE_ERROR_VALUE_INVALID;
    }

    int32_t res = SecCompClient::GetInstance().UpdateSecurityComponent(scId, componentInfo);
    if (res != SC_OK) {
        SC_LOG_ERROR(LABEL, "update security component fail, error: %{public}d", res);
    }
    return res;
}

int32_t SecCompKit::UnregisterSecurityComponent(int32_t scId)
{
    int32_t res = SecCompClient::GetInstance().UnregisterSecurityComponent(scId);
    SecCompEnhanceAdapter::UnregisterScIdEnhance(scId);
    if (res != SC_OK) {
        SC_LOG_ERROR(LABEL, "unregister security component fail, error: %{public}d", res);
        return res;
    }
    HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "UNREGISTER_SUCCESS",
        HiviewDFX::HiSysEvent::EventType::BEHAVIOR, "CALLER_UID", IPCSkeleton::GetCallingUid(),
        "CALLER_PID", IPCSkeleton::GetCallingRealPid(), "SC_ID", scId);
    return res;
}

int32_t SecCompKit::ReportSecurityComponentClickEvent(int32_t scId,
    std::string& componentInfo, const SecCompClickEvent& clickInfo,
    sptr<IRemoteObject> callerToken, OnFirstUseDialogCloseFunc&& callback)
{
    if (!SecCompCallerAuthorization::GetInstance().IsKitCaller(
        reinterpret_cast<uintptr_t>(__builtin_return_address(0)))) {
        SC_LOG_ERROR(LABEL, "report click event fail, caller invalid");
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "CALLER_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingRealPid(), "CALL_SCENE", "CLICK");
        return SC_SERVICE_ERROR_CALLER_INVALID;
    }

    if (callback == nullptr) {
        SC_LOG_ERROR(LABEL, "DialogCloseCallback is null");
        return SC_ENHANCE_ERROR_VALUE_INVALID;
    }

    sptr<IRemoteObject> callbackRemote = new (std::nothrow) SecCompDialogCallback(std::move(callback));
    if (callbackRemote == nullptr) {
        SC_LOG_ERROR(LABEL, "New SecCompDialogCallback fail");
        return SC_SERVICE_ERROR_MEMORY_OPERATE_FAIL;
    }

    if (!SecCompEnhanceAdapter::EnhanceDataPreprocess(scId, componentInfo)) {
        SC_LOG_ERROR(LABEL, "Preprocess security component fail");
        return SC_ENHANCE_ERROR_VALUE_INVALID;
    }

    int32_t res =
        SecCompClient::GetInstance().ReportSecurityComponentClickEvent(scId, componentInfo,
        clickInfo, callerToken, callbackRemote);
    if (res != SC_OK) {
        SC_LOG_ERROR(LABEL, "report click event fail, error: %{public}d", res);
    }
    return res;
}

bool SecCompKit::VerifySavePermission(AccessToken::AccessTokenID tokenId)
{
    bool res =
        SecCompClient::GetInstance().VerifySavePermission(tokenId);
    if (!res) {
        SC_LOG_ERROR(LABEL, "verify temp save permission, error: %{public}d", res);
    }
    return res;
}

sptr<IRemoteObject> SecCompKit::GetEnhanceRemoteObject(bool isLoad)
{
    return SecCompClient::GetInstance().GetEnhanceRemoteObject(isLoad);
}

int32_t SecCompKit::PreRegisterSecCompProcess()
{
    return SecCompClient::GetInstance().PreRegisterSecCompProcess();
}

bool SecCompKit::IsServiceExist()
{
    return SecCompClient::GetInstance().IsServiceExist();
}

bool SecCompKit::LoadService()
{
    return SecCompClient::GetInstance().LoadService();
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
