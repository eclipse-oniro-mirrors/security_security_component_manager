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
#include "sec_comp_client.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompKit"};
}  // namespace

int32_t SecCompKit::RegisterSecurityComponent(SecCompType type,
    const std::string& componentInfo, int32_t& scId)
{
    int32_t res = SecCompClient::GetInstance().RegisterSecurityComponent(type, componentInfo, scId);
    if (res != SC_OK) {
        SC_LOG_ERROR(LABEL, "register security component fail, error: %{public}d", res);
    }
    return res;
}

int32_t SecCompKit::UpdateSecurityComponent(int32_t scId, const std::string& componentInfo)
{
    int32_t res = SecCompClient::GetInstance().UpdateSecurityComponent(scId, componentInfo);
    if (res != SC_OK) {
        SC_LOG_ERROR(LABEL, "update security component fail, error: %{public}d", res);
    }
    return res;
}

int32_t SecCompKit::UnregisterSecurityComponent(int32_t scId)
{
    int32_t res = SecCompClient::GetInstance().UnregisterSecurityComponent(scId);
    if (res != SC_OK) {
        SC_LOG_ERROR(LABEL, "unregister security component fail, error: %{public}d", res);
    }
    return res;
}

int32_t SecCompKit::ReportSecurityComponentClickEvent(int32_t scId,
    const std::string& componentInfo, const SecCompClickEvent& touchInfo)
{
    int32_t res =
        SecCompClient::GetInstance().ReportSecurityComponentClickEvent(scId, componentInfo, touchInfo);
    if (res != SC_OK) {
        SC_LOG_ERROR(LABEL, "report click event fail, error: %{public}d", res);
    }
    return res;
}

bool SecCompKit::ReduceAfterVerifySavePermission(AccessToken::AccessTokenID tokenId)
{
    bool res =
        SecCompClient::GetInstance().ReduceAfterVerifySavePermission(tokenId);
    if (!res) {
        SC_LOG_ERROR(LABEL, "verify temp save permission, error: %{public}d", res);
    }
    return res;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
