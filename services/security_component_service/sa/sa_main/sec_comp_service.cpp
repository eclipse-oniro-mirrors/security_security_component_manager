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

#include "sec_comp_service.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "sec_comp_err.h"
#include "sec_comp_manager.h"
#include "sec_comp_log.h"
#include "system_ability_definition.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompService"};
}

REGISTER_SYSTEM_ABILITY_BY_ID(SecCompService, SA_ID_SECURITY_COMPONENT_SERVICE, true);

SecCompService::SecCompService(int32_t saId, bool runOnCreate)
    : SystemAbility(saId, runOnCreate), state_(ServiceRunningState::STATE_NOT_START)
{
    SC_LOG_INFO(LABEL, "SecCompService()");
}

SecCompService::~SecCompService()
{
    SC_LOG_INFO(LABEL, "~SecCompService()");
}

void SecCompService::OnStart()
{
    if (state_ == ServiceRunningState::STATE_RUNNING) {
        SC_LOG_INFO(LABEL, "SecCompService has already started!");
        return;
    }
    SC_LOG_INFO(LABEL, "SecCompService is starting");
    if (!RegisterAppStateObserver()) {
        SC_LOG_ERROR(LABEL, "Failed to register app state observer!");
        return;
    }
    state_ = ServiceRunningState::STATE_RUNNING;
    bool ret = Publish(this);
    if (!ret) {
        SC_LOG_ERROR(LABEL, "Failed to publish service!");
        return;
    }
    SC_LOG_INFO(LABEL, "Congratulations, SecCompService start successfully!");
}

void SecCompService::OnStop()
{
    SC_LOG_INFO(LABEL, "Stop service");
    state_ = ServiceRunningState::STATE_NOT_START;
    UnregisterAppStateObserver();
    iAppMgr_ = nullptr;
    appStateObserver_ = nullptr;
}

bool SecCompService::RegisterAppStateObserver()
{
    if (appStateObserver_ != nullptr) {
        SC_LOG_INFO(LABEL, "AppStateObserver instance already create");
        return true;
    }
    appStateObserver_ = new (std::nothrow) AppStateObserver();
    if (appStateObserver_ == nullptr) {
        SC_LOG_ERROR(LABEL, "Failed to create AppStateObserver instance");
        return false;
    }
    sptr<ISystemAbilityManager> samgrClient = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (samgrClient == nullptr) {
        SC_LOG_ERROR(LABEL, "Failed to get system ability manager");
        appStateObserver_ = nullptr;
        return false;
    }
    iAppMgr_ = iface_cast<AppExecFwk::IAppMgr>(samgrClient->GetSystemAbility(APP_MGR_SERVICE_ID));
    if (iAppMgr_ == nullptr) {
        SC_LOG_ERROR(LABEL, "Failed to get ability manager service");
        appStateObserver_ = nullptr;
        return false;
    }

    if (iAppMgr_->RegisterApplicationStateObserver(appStateObserver_) != ERR_OK) {
        SC_LOG_ERROR(LABEL, "Failed to Register app state observer");
        iAppMgr_ = nullptr;
        appStateObserver_ = nullptr;
        return false;
    }

    std::vector<AppExecFwk::AppStateData> list;
    if (iAppMgr_->GetForegroundApplications(list) == ERR_OK) {
        for (auto it = list.begin(); it != list.end(); ++it) {
            appStateObserver_->AddProcessToForegroundSet(*it);
        }
    }
    SC_LOG_ERROR(LABEL, "Register app state observer success");
    return true;
}

void SecCompService::UnregisterAppStateObserver()
{
    if (iAppMgr_ != nullptr && appStateObserver_ != nullptr) {
        iAppMgr_->UnregisterApplicationStateObserver(appStateObserver_);
    }
}

bool SecCompService::GetCallerInfo(SecCompCallerInfo& caller)
{
    AccessToken::AccessTokenID tokenId = IPCSkeleton::GetCallingTokenID();
    int32_t pid = IPCSkeleton::GetCallingPid();
    int32_t uid = IPCSkeleton::GetCallingUid();
    if (AccessToken::AccessTokenKit::GetTokenTypeFlag(tokenId) != AccessToken::TOKEN_HAP) {
        SC_LOG_ERROR(LABEL, "Get caller tokenId invalid");
        return false;
    }
    if (!appStateObserver_->IsProcessForeground(uid)) {
        SC_LOG_ERROR(LABEL, "caller uid is not in foreground");
        return false;
    }
    caller.tokenId = tokenId;
    caller.pid = pid;
    caller.uid = uid;
    return true;
}

int32_t SecCompService::RegisterSecurityComponent(SecCompType type,
    const std::string& componentInfo, int32_t& scId)
{
    SecCompCallerInfo caller;
    if (!GetCallerInfo(caller)) {
        SC_LOG_ERROR(LABEL, "Check caller failed");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    nlohmann::json jsonRes = nlohmann::json::parse(componentInfo, nullptr, false);
    if (jsonRes.is_discarded()) {
        SC_LOG_ERROR(LABEL, "component info invalid %{public}s", componentInfo.c_str());
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    return SecCompManager::GetInstance().RegisterSecurityComponent(type, jsonRes, caller, scId);
}

int32_t SecCompService::UpdateSecurityComponent(int32_t scId, const std::string& componentInfo)
{
    SecCompCallerInfo caller;
    if (!GetCallerInfo(caller)) {
        SC_LOG_ERROR(LABEL, "Check caller failed");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    nlohmann::json jsonRes = nlohmann::json::parse(componentInfo, nullptr, false);
    if (jsonRes.is_discarded()) {
        SC_LOG_ERROR(LABEL, "component info invalid %{public}s", componentInfo.c_str());
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }
    return SecCompManager::GetInstance().UpdateSecurityComponent(scId, jsonRes, caller);
}

int32_t SecCompService::UnregisterSecurityComponent(int32_t scId)
{
    SecCompCallerInfo caller;
    if (!GetCallerInfo(caller)) {
        SC_LOG_ERROR(LABEL, "Check caller failed");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    return SecCompManager::GetInstance().UnregisterSecurityComponent(scId, caller);
}

int32_t SecCompService::ReportSecurityComponentClickEvent(int32_t scId,
    const std::string& componentInfo, const SecCompClickEvent& touchInfo)
{
    SecCompCallerInfo caller;
    if (!GetCallerInfo(caller)) {
        SC_LOG_ERROR(LABEL, "Check caller failed");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    nlohmann::json jsonRes = nlohmann::json::parse(componentInfo, nullptr, false);
    if (jsonRes.is_discarded()) {
        SC_LOG_ERROR(LABEL, "component info invalid %{public}s", componentInfo.c_str());
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }
    return SecCompManager::GetInstance().ReportSecurityComponentClickEvent(scId, jsonRes, caller, touchInfo);
}

int SecCompService::Dump(int fd, const std::vector<std::u16string>& args)
{
    if (fd < 0) {
        return ERR_INVALID_VALUE;
    }

    dprintf(fd, "SecCompService Dump:\n");
    std::string arg0 = ((args.size() == 0)? "" : Str16ToStr8(args.at(0)));
    if (arg0.compare("-h") == 0) {
        dprintf(fd, "Usage:\n");
        dprintf(fd, "       -h: command help\n");
        dprintf(fd, "       -a: dump all sec component\n");
        dprintf(fd, "       -p: dump foreground processes\n");
    } else if (arg0.compare("-p") == 0) {
        std::string dumpStr;
        appStateObserver_->DumpProcess(dumpStr);
        dprintf(fd, "%s\n", dumpStr.c_str());
    }  else if (arg0.compare("-a") == 0 || arg0 == "") {
        std::string dumpStr;
        SecCompManager::GetInstance().DumpSecComp(dumpStr);
        dprintf(fd, "%s\n", dumpStr.c_str());
    }
    return ERR_OK;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
