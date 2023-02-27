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
#include "sec_comp_enhance_cfg_parcel.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompEnhanceCfgParcel"};
}

SecCompEnhanceCfgParcel::SecCompEnhanceCfgParcel()
{
    cfg_ = nullptr;
}

SecCompEnhanceCfgParcel::~SecCompEnhanceCfgParcel()
{
    if (cfg_ != nullptr) {
        delete cfg_;
    }
}

bool SecCompEnhanceCfgParcel::Marshalling(Parcel& out) const
{
    return (SecCompEnhanceAdapter::MarshallEnhanceCfg(cfg_, out) != SC_OK);
}

SecCompEnhanceCfgParcel* SecCompEnhanceCfgParcel::Unmarshalling(Parcel& in)
{
    auto* SecCompCfgParcel = new (std::nothrow) SecCompEnhanceCfgParcel();
    if (SecCompCfgParcel == nullptr) {
        SC_LOG_ERROR(LABEL, "New SecCompCfgParcel failed");
        return nullptr;
    }

    if (SecCompEnhanceAdapter::UnmarshallEnhanceCfg(in, SecCompCfgParcel->cfg_) != SC_OK) {
        SC_LOG_ERROR(LABEL, "Unmarshall failed");
        delete SecCompCfgParcel;
        return nullptr;
    }
    return SecCompCfgParcel;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
