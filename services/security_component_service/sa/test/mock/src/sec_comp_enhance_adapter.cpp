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
#include "sec_comp_enhance_adapter.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "MockSecCompEnhanceAdapter"};
}

int32_t SecCompEnhanceAdapter::SetEnhanceCfg(uint8_t* cfg, uint32_t cfgLen)
{
    SC_LOG_DEBUG(LABEL, "SetEnhanceCfg success");
    return SC_OK;
}

int32_t SecCompEnhanceAdapter::GetPointerEventEnhanceData(void* data, uint32_t dataLen,
    uint8_t* enhanceData, uint32_t& enHancedataLen)
{
    SC_LOG_DEBUG(LABEL, "GetPointerEventEnhanceData success");
    return SC_OK;
}

int32_t SecCompEnhanceAdapter::CheckExtraInfo(const SecCompClickEvent& touchInfo)
{
    SC_LOG_DEBUG(LABEL, "CheckExtraInfo success");
    return SC_OK;
}

int32_t SecCompEnhanceAdapter::EnableInputEnhance()
{
    SC_LOG_DEBUG(LABEL, "EnableInputEnhance success");
    return SC_OK;
}

int32_t SecCompEnhanceAdapter::DisableInputEnhance()
{
    SC_LOG_DEBUG(LABEL, "DisableInputEnhance success");
    return SC_OK;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
