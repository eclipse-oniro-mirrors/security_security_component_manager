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

#include "sec_comp_info_helper.h"

#include "location_button.h"
#include "paste_button.h"
#include "save_button.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompInfoHelper"};
}

SecCompBase* SecCompInfoHelper::ParseComponent(SecCompType type, const nlohmann::json& jsonComponent)
{
    SecCompBase* comp = nullptr;
    switch (type) {
        case LOCATION_COMPONENT:
            comp = ConstructComponent<LocationButton>(jsonComponent);
            break;
        case PASTE_COMPONENT:
            comp = ConstructComponent<PasteButton>(jsonComponent);
            break;
        case SAVE_COMPONENT:
            comp = ConstructComponent<SaveButton>(jsonComponent);
            break;
        default:
            SC_LOG_ERROR(LABEL, "Parse component type unknown");
            break;
    }
    if (comp != nullptr && !comp->IsValid()) {
        delete comp;
        return nullptr;
    }
    return comp;
}

bool SecCompInfoHelper::CompareSecCompInfo(const SecCompBase* comA, const SecCompBase* comB)
{
    if (comA == nullptr || comB == nullptr) {
        SC_LOG_ERROR(LABEL, "Security component info is null");
        return false;
    }

    if (comA->type_ != comB->type_) {
        SC_LOG_ERROR(LABEL, "Security component type is not equal");
        return false;
    }

    bool res = false;
    switch (comA->type_) {
        case LOCATION_COMPONENT:
            res = *reinterpret_cast<const LocationButton *>(comA) == *reinterpret_cast<const LocationButton *>(comB);
            break;
        case PASTE_COMPONENT:
            res = *reinterpret_cast<const PasteButton *>(comA) == *reinterpret_cast<const PasteButton *>(comB);
            break;
        case SAVE_COMPONENT:
            res = *reinterpret_cast<const SaveButton *>(comA) == *reinterpret_cast<const SaveButton *>(comB);
            break;
        default:
            SC_LOG_ERROR(LABEL, "Compare component component type unknown");
            break;
    }
    return res;
}

int32_t SecCompInfoHelper::RevokeTempPermission(AccessToken::AccessTokenID tokenId,
    const std::shared_ptr<SecCompBase>& componentInfo)
{
    if (componentInfo == nullptr) {
        SC_LOG_ERROR(LABEL, "revoke component is null");
        return SC_SERVICE_ERROR_PERMISSION_OPER_FAIL;
    }
    SecCompType type = componentInfo->type_;
    switch (type) {
        case LOCATION_COMPONENT:
            return AccessToken::AccessTokenKit::RevokePermission(tokenId, "ohos.permission.LOCATION",
                AccessToken::PermissionFlag::PERMISSION_DEFAULT_FLAG);
        default:
            SC_LOG_ERROR(LABEL, "revoke component type unknown");
            break;
    }
    return SC_SERVICE_ERROR_PERMISSION_OPER_FAIL;
}

int32_t SecCompInfoHelper::GrantTempPermission(AccessToken::AccessTokenID tokenId,
    const std::shared_ptr<SecCompBase>& componentInfo)
{
    if (componentInfo == nullptr) {
        SC_LOG_ERROR(LABEL, "revoke component is null");
        return SC_SERVICE_ERROR_PERMISSION_OPER_FAIL;
    }
    SecCompType type = componentInfo->type_;
    switch (type) {
        case LOCATION_COMPONENT:
            return AccessToken::AccessTokenKit::GrantPermission(tokenId, "ohos.permission.LOCATION",
                AccessToken::PermissionFlag::PERMISSION_DEFAULT_FLAG);
        default:
            SC_LOG_ERROR(LABEL, "Parse component type unknown");
            break;
    }
    return SC_SERVICE_ERROR_PERMISSION_OPER_FAIL;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
