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
#ifndef I_SECURITY_COMPONENT_LOCATION_BUTTON_H
#define I_SECURITY_COMPONENT_LOCATION_BUTTON_H

#include <string>
#include "nlohmann/json.hpp"
#include "sec_comp_base.h"
#include "sec_comp_info.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
class LocationButton : public SecCompBase {
public:
    SecCompFontFamily font_ = UNKNOWN_FONT_FAMILY_TYPE;
    SecCompLabel label_ = UNKNOWN_LABEL;
    SecCompIcon icon_ = UNKNOWN_ICON;
    uint32_t fontSize_ = 0;
    SecCompColor fontColor_;
    SecCompColor bgColor_;
    SecCompButtonType buttonType_ = UNKNOWN_BUTTON_TYPE;

    virtual void FromJson(const nlohmann::json& jsonSrc) override;
    virtual void ToJson(nlohmann::json& jsonRes) const override;
    virtual bool IsValid(void) const override;
    bool operator==(const LocationButton& other) const;
};
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
#endif  // I_SECURITY_COMPONENT_LOCATION_BUTTON_H
