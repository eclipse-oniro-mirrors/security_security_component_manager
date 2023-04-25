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

#include "i_sec_comp_service.h"
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
}

SecCompManager::SecCompManager()
{
    scIdStart_ = SC_ID_START;
    scValidCount_ = 0;
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

int32_t SecCompManager::AddProcessComponent(std::vector<SecCompEntity>& componentList,
    const SecCompEntity& newEntity)
{
    componentList.emplace_back(newEntity);
    if (scValidCount_ == 0) {
        SecCompEnhanceAdapter::EnableInputEnhance();
    }
    scValidCount_++;
    return SC_OK;
}

int32_t SecCompManager::AddSecurityComponentToList(int32_t uid, const SecCompEntity& newEntity)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(uid);
    if (iter != componentMap_.end()) {
        return AddProcessComponent(iter->second, newEntity);
    }

    std::vector<SecCompEntity> newComponentList;
    AddProcessComponent(newComponentList, newEntity);
    componentMap_[uid] = newComponentList;
    return SC_OK;
}

int32_t SecCompManager::DeleteSecurityComponentFromList(int32_t uid, int32_t scId)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(uid);
    if (iter == componentMap_.end()) {
        SC_LOG_ERROR(LABEL, "Can not find registered process");
        return SC_SERVICE_ERROR_COMPONENT_NOT_EXIST;
    }
    std::vector<SecCompEntity>& list = iter->second;
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->GetScId() == scId) {
            it->RevokeTempPermission();
            if (it->GetEffective()) {
                scValidCount_--;
            }

            list.erase(it);
            return SC_OK;
        }
    }
    SC_LOG_ERROR(LABEL, "Can not find component");
    return SC_SERVICE_ERROR_COMPONENT_NOT_EXIST;
}

SecCompEntity* SecCompManager::GetSecurityComponentFromList(int32_t uid, int32_t scId)
{
    auto iter = componentMap_.find(uid);
    if (iter == componentMap_.end()) {
        return nullptr;
    }
    std::vector<SecCompEntity>& list = iter->second;
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->GetScId() == scId) {
            return std::addressof(*it);
        }
    }
    return nullptr;
}

void SecCompManager::NotifyProcessForeground(int32_t uid)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(uid);
    if (iter == componentMap_.end()) {
        return;
    }

    SC_LOG_INFO(LABEL, "App uid %{public}d to foreground", uid);
    std::vector<SecCompEntity>& list = iter->second;
    for (auto it = list.begin(); it != list.end(); ++it) {
        it->SetEffective(true);
        scValidCount_++;
    }
    if (!list.empty()) {
        SecCompEnhanceAdapter::EnableInputEnhance();
    }
}

void SecCompManager::NotifyProcessBackground(int32_t uid)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(uid);
    if (iter == componentMap_.end()) {
        return;
    }

    SC_LOG_INFO(LABEL, "App uid %{public}d to background", uid);
    std::vector<SecCompEntity>& list = iter->second;
    for (auto it = list.begin(); it != list.end(); ++it) {
        it->RevokeTempPermission();
        it->SetEffective(false);
        scValidCount_--;
    }
    if (scValidCount_ <= 0) {
        SecCompEnhanceAdapter::DisableInputEnhance();
    }
}

void SecCompManager::NotifyProcessDied(int32_t uid)
{
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    auto iter = componentMap_.find(uid);
    if (iter == componentMap_.end()) {
        return;
    }
    SC_LOG_INFO(LABEL, "App uid %{public}d died", uid);
    std::vector<SecCompEntity>& list = iter->second;
    for (auto it = list.begin(); it != list.end(); ++it) {
        it->RevokeTempPermission();
        if (it->GetEffective()) {
            scValidCount_--;
        }
    }
    list.clear();
    if (scValidCount_ <= 0) {
        SecCompEnhanceAdapter::DisableInputEnhance();
    }
    componentMap_.erase(uid);
    ExitSaAfterAllProcessDie();
}

void SecCompManager::ExitSaAfterAllProcessDie()
{
    if (!componentMap_.empty()) {
        return;
    }
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

int32_t SecCompManager::RegisterSecurityComponent(SecCompType type,
    const nlohmann::json& jsonComponent, const SecCompCallerInfo& caller, int32_t& scId)
{
    SecCompBase* componentPtr = SecCompInfoHelper::ParseComponent(type, jsonComponent);
    std::shared_ptr<SecCompBase> component(componentPtr);
    if (component == nullptr) {
        SC_LOG_ERROR(LABEL, "Parse component info invalid");
        return SC_SERVICE_ERROR_COMPONENT_INFO_INVALID;
    }

    int32_t registerId = CreateScId();
    SecCompEntity entity(component, caller.tokenId, registerId);
    entity.SetEffective(true);
    int32_t ret = AddSecurityComponentToList(caller.uid, entity);
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
    OHOS::Utils::UniqueWriteGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    SecCompEntity* sc = GetSecurityComponentFromList(caller.uid, scId);
    if (sc == nullptr) {
        SC_LOG_ERROR(LABEL, "Can not find target component");
        return SC_SERVICE_ERROR_COMPONENT_NOT_EXIST;
    }
    SecCompBase* report = SecCompInfoHelper::ParseComponent(sc->GetType(), jsonComponent);
    std::shared_ptr<SecCompBase> reportComponentInfo(report);
    if (reportComponentInfo == nullptr) {
        SC_LOG_ERROR(LABEL, "Update component info invalid");
        return SC_SERVICE_ERROR_COMPONENT_INFO_INVALID;
    }
    sc->SetComponentInfo(reportComponentInfo);
    return SC_OK;
}

int32_t SecCompManager::UnregisterSecurityComponent(int32_t scId, const SecCompCallerInfo& caller)
{
    if (scId < 0) {
        SC_LOG_ERROR(LABEL, "ScId is invalid");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }
    int32_t res = DeleteSecurityComponentFromList(caller.uid, scId);
    if (scValidCount_ <= 0) {
        SecCompEnhanceAdapter::DisableInputEnhance();
    }
    return res;
}

int32_t SecCompManager::ReportSecurityComponentClickEvent(int32_t scId,
    const nlohmann::json& jsonComponent, const SecCompCallerInfo& caller, const SecCompClickEvent& touchInfo)
{
    OHOS::Utils::UniqueReadGuard<OHOS::Utils::RWLock> lk(this->componentInfoLock_);
    SecCompEntity* sc = GetSecurityComponentFromList(caller.uid, scId);
    if (sc == nullptr) {
        SC_LOG_ERROR(LABEL, "Can not find target component");
        return SC_SERVICE_ERROR_COMPONENT_NOT_EXIST;
    }

    SecCompBase* report = SecCompInfoHelper::ParseComponent(sc->GetType(), jsonComponent);
    std::shared_ptr<SecCompBase> reportComponentInfo(report);
    if ((reportComponentInfo == nullptr) || !reportComponentInfo->GetValid()) {
        SC_LOG_ERROR(LABEL, "report component info invalid");
        return SC_SERVICE_ERROR_COMPONENT_INFO_INVALID;
    }

    // BasicInfo is the attributes of security component defined in ets, such as fontsize.
    // we only compare these because they can be obtained during registration/update,
    // but other infos such as position may not be notified because changs occur at the parent node.
    if (!sc->CompareComponentBasicInfo(reportComponentInfo.get())) {
        SC_LOG_ERROR(LABEL, "Report component basic compare register component failed");
        return SC_SERVICE_ERROR_COMPONENT_INFO_NOT_EQUAL;
    }

    sc->SetComponentInfo(reportComponentInfo);
    if (!sc->CheckTouchInfo(touchInfo)) {
        SC_LOG_ERROR(LABEL, "touchInfo is invalid");
        return SC_SERVICE_ERROR_CLICK_EVENT_INVALID;
    }

    return sc->GrantTempPermission();
}

void SecCompManager::DumpSecComp(std::string& dumpStr) const
{
    for (auto iter = componentMap_.begin(); iter != componentMap_.end(); ++iter) {
        dumpStr.append("uid:" + std::to_string(iter->first) + "\n");
        for (auto compIter = iter->second.begin(); compIter != iter->second.end(); compIter ++) {
            nlohmann::json json;
            compIter->GetComponentInfo()->ToJson(json);
            dumpStr.append("    scId:" + std::to_string(compIter->GetScId()) +
                ", isGrant:" + std::to_string(compIter->IsGrant()) + ", " + json.dump() + "\n");
        }
    }
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
