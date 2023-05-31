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

#include "paste_button.h"
#include <tuple>
#include "sec_comp_err.h"
#include "sec_comp_log.h"
#include "sec_comp_tool.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "PasteButton"};
static const std::string JSON_STYLE_TAG = "style";
static const std::string JSON_TEXT_TAG = "text";
static const std::string JSON_ICON_TAG = "icon";
static const std::string JSON_BG_TAG = "bg";
}

bool PasteButton::IsParamValid()
{
    if ((static_cast<PasteDesc>(text_) <= PasteDesc::UNKNOWN_TEXT) ||
        (static_cast<PasteDesc>(text_) >= PasteDesc::MAX_LABEL_TYPE) ||
        (static_cast<PasteIcon>(icon_) <= PasteIcon::UNKNOWN_ICON) ||
        (static_cast<PasteIcon>(icon_) >= PasteIcon::MAX_ICON_TYPE)) {
        return false;
    }

    return true;
}

bool PasteButton::CompareComponentBasicInfo(SecCompBase *other) const
{
    if (!SecCompBase::CompareComponentBasicInfo(other)) {
        SC_LOG_ERROR(LABEL, "SecComp base not equal.");
        return false;
    }
    PasteButton* otherPasteButton = reinterpret_cast<PasteButton *>(other);
    if (otherPasteButton == nullptr) {
        SC_LOG_ERROR(LABEL, "other is not paste button.");
        return false;
    }
    return (icon_ == otherPasteButton->icon_) && (text_ == otherPasteButton->text_) &&
        (bg_ == otherPasteButton->bg_);
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
