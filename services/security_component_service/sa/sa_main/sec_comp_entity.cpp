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
#include "sec_comp_entity.h"
#include <chrono>
#include "hisysevent.h"
#include "ipc_skeleton.h"
#include "sec_comp_err.h"
#include "sec_comp_enhance_adapter.h"
#include "sec_comp_info_helper.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompEntity"};
static constexpr uint64_t MAX_TOUCH_INTERVAL = 1000000L; // 1000ms
static constexpr uint64_t TIME_CONVERSION_UNIT = 1000;
}

int32_t SecCompEntity::RevokeTempPermission()
{
    if (!isGrant_) {
        SC_LOG_ERROR(LABEL, "security component is not granted");
        return SC_OK;
    }
    isGrant_ = false;
    return SecCompInfoHelper::RevokeTempPermission(tokenId_, componentInfo_);
}

int32_t SecCompEntity::GrantTempPermission()
{
    isGrant_ = true;
    return SecCompInfoHelper::GrantTempPermission(tokenId_, componentInfo_);
}

bool SecCompEntity::CompareComponentBasicInfo(SecCompBase* other) const
{
    return componentInfo_->CompareComponentBasicInfo(other);
}

bool SecCompEntity::CheckTouchInfo(const SecCompClickEvent& touchInfo) const
{
    auto current = static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT;
    if (touchInfo.timestamp < current - MAX_TOUCH_INTERVAL || touchInfo.timestamp > current) {
        SC_LOG_ERROR(LABEL, "touch timestamp invalid touchInfo. timestamp: %{public}lu, current: %{public}lu",
            touchInfo.timestamp, current);
        return false;
    }

    if (!componentInfo_->rect_.IsInRect(touchInfo.touchX, touchInfo.touchY)) {
        SC_LOG_ERROR(LABEL, "touch point is not in component rect");
        return false;
    }

    if (SecCompEnhanceAdapter::CheckExtraInfo(touchInfo) != SC_OK) {
        SC_LOG_ERROR(LABEL, "HMAC checkout failed"
            "touchX:%{public}f, touchY:%{public}f, timestamp:%{public}lu, dataSize:%{public}d",
            touchInfo.touchX, touchInfo.touchY, touchInfo.timestamp, touchInfo.extraInfo.dataSize);
        HiSysEventWrite(HiviewDFX::HiSysEvent::Domain::SEC_COMPONENT, "CLICK_INFO_CHECK_FAILED",
            HiviewDFX::HiSysEvent::EventType::SECURITY, "CALLER_UID", IPCSkeleton::GetCallingUid(),
            "CALLER_PID", IPCSkeleton::GetCallingPid(), "SC_ID", scId_, "SC_TYPE", componentInfo_->type_);
        return false;
    }
    return true;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
