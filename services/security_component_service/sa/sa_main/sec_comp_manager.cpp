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

#include "sec_comp_manager.h"

#include "delay_exit_task.h"
#include "hisysevent.h"
#include "i_sec_comp_service.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "sec_comp_enhance_adapter.h"
#include "sec_comp_err.h"
#include "sec_comp_info_helper.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompManager"};
static constexpr int32_t SC_ID_START = 1000;
static constexpr int32_t MAX_INT_NUM = 0x7fffffff;
static constexpr int32_t ROOT_UID = 0;
}

SecCompManager::SecCompManager()
{
    scIdStart_ = SC_ID_START;
    SC_LOG_INFO(LABEL, "SecCompManager()");
}

SecCompManager& SecCompManager::GetInstance()
{
    static SecCompManager instance;
    return instance;
}

int32_t SecCompManager::CreateScId()
{
    std::lock_guard<std::mutex> lock(scIdMtx_);
    if (scIdStart_ == MAX_INT_NUM) {
        scIdStart_ = SC_ID_START;
    } else {
        scIdStart_++;
    }
    return scIdStart_;
}

int32_t SecCompManager::AddSecurityComponentToList(int32_t pid, const SecCompEntity& newEntity)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    if (isSaExit_) {
        SC_LOG_ERROR(LABEL, "SA is exiting, retry...");
        return SC_SERVICE_ERROR_SERVICE_NOT_EXIST;
    }

    auto iter = componentMap_.find(pid);
    if (iter != componentMap_.end()) {
        iter->second.isForeground = true;
        iter->second.compList.emplace_back(newEntity);
        SecCompEnhanceAdapter::EnableInputEnhance();
        return SC_OK;
    }

    ProcessCompInfos newProcess;
    newProcess.isForeground = true;
    newProcess.compList.emplace_back(newEntity);
    componentMap_[pid] = newProcess;
    SecCompEnhanceAdapter::EnableInputEnhance();
    return SC_OK;
}

int32_t SecCompManager::DeleteSecurityComponentFromList(int32_t pid, int32_t scId)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(pid);
    if (iter == componentMap_.end()) {
        SC_LOG_ERROR(LABEL, "Can not find registered process");
        return SC_SERVICE_ERROR_COMPONENT_NOT_EXIST;
    }
    std::vector<SecCompEntity>& list = iter->second.compList;
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->GetScId() == scId) {
            it->RevokeTempPermission();
            list.erase(it);
            if (!IsForegroundCompExist()) {
                SecCompEnhanceAdapter::DisableInputEnhance();
            }
            return SC_OK;
        }
    }
    SC_LOG_ERROR(LABEL, "Can not find component");
    return SC_SERVICE_ERROR_COMPONENT_NOT_EXIST;
}

bool SecCompManager::IsInMaliciousAppList(int32_t pid)
{
    std::lock_guard<std::mutex> lock(maliciousMtx_);
    if (IPCSkeleton::GetCallingUid() == ROOT_UID) {
        return false;
    }
    return (maliciousAppList_.find(pid) != maliciousAppList_.end());
}

void SecCompManager::AddAppToMaliciousAppList(int32_t pid)
{
    std::lock_guard<std::mutex> lock(maliciousMtx_);
    maliciousAppList_.insert(pid);
}

void SecCompManager::RemoveAppFromMaliciousAppList(int32_t pid)
{
    std::lock_guard<std::mutex> lock(maliciousMtx_);
    maliciousAppList_.erase(pid);
}

bool SecCompManager::IsMaliciousAppListEmpty()
{
    std::lock_guard<std::mutex> lock(maliciousMtx_);
    return (maliciousAppList_.size() == 0);
}

static std::string TransformCallBackResult(enum SCErrCode error)
{
    std::string errMsg = "";
    switch (error) {
        case SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE:
            errMsg = "enhance do not exist";
            break;
        case SC_ENHANCE_ERROR_VALUE_INVALID:
            errMsg = "value is invalid";
            break;
        case SC_ENHANCE_ERROR_CALLBACK_NOT_EXIST:
            errMsg = "callback do not exist";
            break;
        case SC_ENHANCE_ERROR_CALLBACK_OPER_FAIL:
            errMsg = "callback operate fail";
            break;
        case SC_SERVICE_ERROR_COMPONENT_INFO_INVALID:
            errMsg = "component info invalid";
            break;
        case SC_ENHANCE_ERROR_CALLBACK_CHECK_FAIL:
            errMsg = "callback check fail";
            break;
        default:
            errMsg = "unknown error";
            break;
    }
    return errMsg;
}

SecCompEntity* SecCompManager::GetSecurityComponentFromList(int32_t pid, int32_t scId)
{
    auto iter = componentMap_.find(pid);
    if (iter == componentMap_.end()) {
        return nullptr;
    }
    std::vector<SecCompEntity>& list = iter->second.compList;
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->GetScId() == scId) {
            return std::addressof(*it);
        }
    }
    return nullptr;
}

bool SecCompManager::IsForegroundCompExist()
{
    return std::any_of(componentMap_.begin(), componentMap_.end(), [](const auto & iter) {
        return (iter.second.isForeground) && (iter.second.compList.size() > 0);
    });
}

void SecCompManager::NotifyProcessForeground(int32_t pid)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(pid);
    if (iter == componentMap_.end()) {
        return;
    }
    iter->second.isForeground = true;
    if (IsForegroundCompExist()) {
        SecCompEnhanceAdapter::EnableInputEnhance();
    }

    SC_LOG_INFO(LABEL, "App pid %{public}d to foreground", pid);
}

void SecCompManager::NotifyProcessBackground(int32_t pid)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(pid);
    if (iter == componentMap_.end()) {
        return;
    }

    iter->second.isForeground = false;
    if (!IsForegroundCompExist()) {
        SecCompEnhanceAdapter::DisableInputEnhance();
    }
    SC_LOG_INFO(LABEL, "App pid %{public}d to background", pid);
}

void SecCompManager::NotifyProcessDied(int32_t pid)
{
    // notify enhance process died.
    SecCompEnhanceAdapter::NotifyProcessDied(pid);

    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(pid);
    if (iter == componentMap_.end()) {
        return;
    }
    SC_LOG_INFO(LABEL, "App pid %{public}d died", pid);
    std::vector<SecCompEntity>& list = iter->second.compList;
    for (auto it = list.begin(); it != list.end(); ++it) {
        it->RevokeTempPermission();
    }
    list.clear();
    componentMap_.erase(pid);

    if (!IsForegroundCompExist()) {
        SecCompEnhanceAdapter::DisableInputEnhance();
    }

    RemoveAppFromMaliciousAppList(pid);
    DelayExitTask::GetInstance().Start();
}

void SecCompManager::ExitSaProcess()
{
    OHOS::Utils::UniqueReadGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    if (!componentMap_.empty() || !IsMaliciousAppListEmpty()) {
        SC_LOG_INFO(LABEL, "Apps using security component still exist, no exit sa");
        return;
    }
    isSaExit_ = true;
    SecCompEnhanceAdapter::DisableInputEnhance();
    SecCompEnhanceAdapter::ExistEnhanceService();

    SC_LOG_INFO(LABEL, "All processes using security component died, start sa exit");
    auto systemAbilityMgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (systemAbilityMgr == nullptr) {
        SC_LOG_ERROR(LABEL, "Failed to get SystemAbilityManager.");
        return;
    }
    int32_t ret = systemAbilityMgr->UnloadSystemAbility(SA_ID_SECURITY_COMPONENT_SERVICE);
    if (ret != SC_OK) {
        SC_LOG_ERROR(LABEL, "Failed to UnloadSystemAbility service! errcode=%{public}d", ret);
        return;
    }
    SC_LOG_INFO(LABEL, "UnloadSystemAbility successfully!");
}

void SecCompManager::SendCheckInfoEnhanceSysEvent(int32_t scId,
    SecCompType type, const std::string& scene, int32_t res)
{
    if (res == SC_ENHANCE_ERROR_CHALLENGE_CHECK_FAIL) {
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "CHALLENGE_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_ID", scId, "SC_TYPE", type, "CALL_SCENE",
            scene);
    } else {
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "CALLBACK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_TYPE", type,
            "CALL_SCENE", scene, "REASON", TransformCallBackResult(static_cast<enum SCErrCode>(res)));
    }
}

int32_t SecCompManager::RegisterSecurityComponent(SecCompType type,
    const nlohmann::json& jsonComponent, const SecCompCallerInfo& caller, int32_t& scId)
{
    DelayExitTask::GetInstance().Stop();
    SC_LOG_DEBUG(LABEL, "PID: %{public}d, register security component", caller.pid);
    if (IsInMaliciousAppList(caller.pid)) {
        SC_LOG_ERROR(LABEL, "app is in MaliciousAppList, never allow it");
        return SC_ENHANCE_ERROR_IN_MALICIOUS_LIST;
    }

    SecCompBase* componentPtr = SecCompInfoHelper::ParseComponent(type, jsonComponent);
    std::shared_ptr<SecCompBase> component(componentPtr);
    if (component == nullptr) {
        SC_LOG_ERROR(LABEL, "Parse component info invalid");
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "COMPONENT_INFO_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_ID", scId, "CALL_SCENE", "REGITSTER", "SC_TYPE", type);
        return SC_SERVICE_ERROR_COMPONENT_INFO_INVALID;
    }

    int32_t enhanceRes =
        SecCompEnhanceAdapter::CheckComponentInfoEnhnace(caller.pid, component, jsonComponent);
    if (enhanceRes != SC_OK) {
        SendCheckInfoEnhanceSysEvent(INVALID_SC_ID, type, "REGISTER", enhanceRes);
        SC_LOG_ERROR(LABEL, "enhance check failed");
        AddAppToMaliciousAppList(caller.pid);
        return enhanceRes;
    }

    int32_t registerId = CreateScId();
    SecCompEntity entity(component, caller.tokenId, registerId);
    int32_t ret = AddSecurityComponentToList(caller.pid, entity);
    if (ret == SC_OK) {
        scId = registerId;
    } else {
        SC_LOG_ERROR(LABEL, "Register security component failed");
        scId = INVALID_SC_ID;
    }
    return ret;
}

int32_t SecCompManager::UpdateSecurityComponent(int32_t scId, const nlohmann::json& jsonComponent,
    const SecCompCallerInfo& caller)
{
    SC_LOG_DEBUG(LABEL, "PID: %{public}d, update security component", caller.pid);
    if (IsInMaliciousAppList(caller.pid)) {
        SC_LOG_ERROR(LABEL, "app is in MaliciousAppList, never allow it");
        return SC_ENHANCE_ERROR_IN_MALICIOUS_LIST;
    }

    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    SecCompEntity* sc = GetSecurityComponentFromList(caller.pid, scId);
    if (sc == nullptr) {
        SC_LOG_ERROR(LABEL, "Can not find target component");
        return SC_SERVICE_ERROR_COMPONENT_NOT_EXIST;
    }
    SecCompBase* report = SecCompInfoHelper::ParseComponent(sc->GetType(), jsonComponent);
    std::shared_ptr<SecCompBase> reportComponentInfo(report);
    if (reportComponentInfo == nullptr) {
        SC_LOG_ERROR(LABEL, "Update component info invalid");
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "COMPONENT_INFO_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_ID", scId, "CALL_SCENE", "UPDATE",
            "SC_TYPE", sc->GetType());
        return SC_SERVICE_ERROR_COMPONENT_INFO_INVALID;
    }

    int32_t enhanceRes =
        SecCompEnhanceAdapter::CheckComponentInfoEnhnace(caller.pid, reportComponentInfo, jsonComponent);
    if (enhanceRes != SC_OK) {
        SendCheckInfoEnhanceSysEvent(scId, sc->GetType(), "UPDATE", enhanceRes);
        SC_LOG_ERROR(LABEL, "enhance check failed");
        AddAppToMaliciousAppList(caller.pid);
        return enhanceRes;
    }

    sc->SetComponentInfo(reportComponentInfo);
    return SC_OK;
}

int32_t SecCompManager::UnregisterSecurityComponent(int32_t scId, const SecCompCallerInfo& caller)
{
    SC_LOG_DEBUG(LABEL, "PID: %{public}d, unregister security component", caller.pid);
    if (scId < 0) {
        SC_LOG_ERROR(LABEL, "ScId is invalid");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }
    return DeleteSecurityComponentFromList(caller.pid, scId);
}

int32_t SecCompManager::CheckClickSecurityComponentInfo(SecCompEntity* sc, int32_t scId,
    const nlohmann::json& jsonComponent,  const SecCompCallerInfo& caller)
{
    SecCompBase* report = SecCompInfoHelper::ParseComponent(sc->GetType(), jsonComponent);
    std::shared_ptr<SecCompBase> reportComponentInfo(report);
    if ((reportComponentInfo == nullptr) || !reportComponentInfo->GetValid()) {
        SC_LOG_ERROR(LABEL, "report component info invalid");
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "COMPONENT_INFO_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_ID", scId, "CALL_SCENE", "CLICK", "SC_TYPE",
            sc->GetType());
        return SC_SERVICE_ERROR_COMPONENT_INFO_INVALID;
    }

    int32_t enhanceRes =
        SecCompEnhanceAdapter::CheckComponentInfoEnhnace(caller.pid, reportComponentInfo, jsonComponent);
    if (enhanceRes != SC_OK) {
        SendCheckInfoEnhanceSysEvent(scId, sc->GetType(), "CLICK", enhanceRes);
        SC_LOG_ERROR(LABEL, "enhance check failed");
        AddAppToMaliciousAppList(caller.pid);
        return enhanceRes;
    }

    sc->SetComponentInfo(reportComponentInfo);
    return SC_OK;
}

int32_t SecCompManager::ReportSecurityComponentClickEvent(int32_t scId,
    const nlohmann::json& jsonComponent, const SecCompCallerInfo& caller,
    const SecCompClickEvent& touchInfo, sptr<IRemoteObject> callerToken)
{
    if (IsInMaliciousAppList(caller.pid)) {
        SC_LOG_ERROR(LABEL, "app is in MaliciousAppList, never allow it");
        return SC_ENHANCE_ERROR_IN_MALICIOUS_LIST;
    }

    OHOS::Utils::UniqueReadGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    SecCompEntity* sc = GetSecurityComponentFromList(caller.pid, scId);
    if (sc == nullptr) {
        SC_LOG_ERROR(LABEL, "Can not find target component");
        return SC_SERVICE_ERROR_COMPONENT_NOT_EXIST;
    }

    int32_t res = CheckClickSecurityComponentInfo(sc, scId, jsonComponent, caller);
    if (res != SC_OK) {
        return res;
    }

    if (!sc->CheckTouchInfo(touchInfo)) {
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "CLICK_INFO_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_ID", scId, "SC_TYPE", sc->GetType());
        AddAppToMaliciousAppList(caller.pid);
        return SC_SERVICE_ERROR_CLICK_EVENT_INVALID;
    }
    res = sc->GrantTempPermission();
    if (res != SC_OK) {
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "TEMP_GRANT_FAILED",
            HiviewDFX::HiSysEvent::EventType::FAULT, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_ID", scId, "SC_TYPE", sc->GetType());
        return res;
    }
    HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "TEMP_GRANT_SUCCESS",
        HiviewDFX::HiSysEvent::EventType::BEHAVIOR, "CALLER_UID", IPCSkeleton::GetCallingUid(),
        "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_ID", scId, "SC_TYPE", sc->GetType());
    firstUseDialog_.NotifyFirstUseDialog(caller.tokenId, sc->GetType(), callerToken);
    return res;
}

bool SecCompManager::ReduceAfterVerifySavePermission(AccessToken::AccessTokenID tokenId)
{
    if (SecCompPermManager::GetInstance().RevokeTempSavePermission(tokenId) == SC_OK) {
        return true;
    }
    return false;
}

void SecCompManager::DumpSecComp(std::string& dumpStr)
{
    OHOS::Utils::UniqueReadGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    for (auto iter = componentMap_.begin(); iter != componentMap_.end(); ++iter) {
        dumpStr.append("pid:" + std::to_string(iter->first) + "\n");
        for (auto compIter = iter->second.compList.begin(); compIter != iter->second.compList.end(); compIter ++) {
            nlohmann::json json;
            compIter->GetComponentInfo()->ToJson(json);
            dumpStr.append("    scId:" + std::to_string(compIter->GetScId()) +
                ", isGrant:" + std::to_string(compIter->IsGrant()) + ", " + json.dump() + "\n");
        }
    }
}

bool SecCompManager::Initialize()
{
    SC_LOG_DEBUG(LABEL, "Initialize!!");
    SecCompEnhanceAdapter::StartEnhanceService();

    secRunner_ = AppExecFwk::EventRunner::Create(true);
    if (!secRunner_) {
        SC_LOG_ERROR(LABEL, "failed to create a recvRunner.");
        return false;
    }

    secHandler_ = std::make_shared<SecEventHandler>(secRunner_);
    DelayExitTask::GetInstance().Init(secHandler_);
    firstUseDialog_.Init(secHandler_);

    return SecCompPermManager::GetInstance().InitEventHandler(secHandler_);
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
