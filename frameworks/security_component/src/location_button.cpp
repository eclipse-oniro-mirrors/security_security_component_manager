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

#include "location_button.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "LocationButton"};
static const std::string JSON_FONT_FAMILY = "font";
static const std::string JSON_LABEL_TYPE = "label";
static const std::string JSON_ICON = "icon";
static const std::string JSON_FONT_SIZE = "fontSize";
static const std::string JSON_FONT_COLOR = "fontColor";
static const std::string JSON_BG_COLOR = "bgColor";
static const std::string JSON_BUTTON_TYPE = "buttonType";
}

void LocationButton::FromJson(const nlohmann::json& jsonSrc)
{
    SecCompBase::FromJson(jsonSrc);
    if (jsonSrc.find(JSON_FONT_FAMILY) != jsonSrc.end() && jsonSrc.at(JSON_FONT_FAMILY).is_number()) {
        int32_t font = jsonSrc.at(JSON_FONT_FAMILY).get<int32_t>();
        if (IsComponentFontFamilyValid(font)) {
            font_ = static_cast<SecCompFontFamily>(font);
        }
    }

    if (jsonSrc.find(JSON_LABEL_TYPE) != jsonSrc.end() && jsonSrc.at(JSON_LABEL_TYPE).is_number()) {
        int32_t label = jsonSrc.at(JSON_LABEL_TYPE).get<int32_t>();
        if (IsComponentLabelValid(label)) {
            label_ = static_cast<SecCompLabel>(label);
        }
    }

    if (jsonSrc.find(JSON_ICON) != jsonSrc.end() && jsonSrc.at(JSON_ICON).is_number()) {
        int32_t icon = jsonSrc.at(JSON_ICON).get<int32_t>();
        if (IsComponentIconValid(icon)) {
            icon_ = static_cast<SecCompIcon>(icon);
        }
    }

    if (jsonSrc.find(JSON_FONT_SIZE) != jsonSrc.end() && jsonSrc.at(JSON_FONT_SIZE).is_number()) {
        fontSize_ = jsonSrc.at(JSON_FONT_SIZE).get<int32_t>();
    }

    if (jsonSrc.find(JSON_FONT_COLOR) != jsonSrc.end() && jsonSrc.at(JSON_FONT_COLOR).is_number()) {
        fontColor_.value = jsonSrc.at(JSON_FONT_COLOR).get<uint32_t>();
    }

    if (jsonSrc.find(JSON_BG_COLOR) != jsonSrc.end() && jsonSrc.at(JSON_BG_COLOR).is_number()) {
        bgColor_.value = jsonSrc.at(JSON_BG_COLOR).get<uint32_t>();
    }

    if (jsonSrc.find(JSON_BUTTON_TYPE) != jsonSrc.end() && jsonSrc.at(JSON_BUTTON_TYPE).is_number()) {
        int32_t type = jsonSrc.at(JSON_BUTTON_TYPE).get<int32_t>();
        if (IsComponentButtonTypeValid(type)) {
            buttonType_ = static_cast<SecCompButtonType>(type);
        }
    }
}

void LocationButton::ToJson(nlohmann::json& jsonRes) const
{
    SecCompBase::ToJson(jsonRes);
    jsonRes[JSON_FONT_FAMILY] = font_;
    jsonRes[JSON_LABEL_TYPE] = label_;
    jsonRes[JSON_ICON] = icon_;
    jsonRes[JSON_FONT_SIZE] = fontSize_;
    jsonRes[JSON_FONT_COLOR] = fontColor_.value;
    jsonRes[JSON_BG_COLOR] = bgColor_.value;
    jsonRes[JSON_BUTTON_TYPE] = buttonType_;
}

bool LocationButton::IsValid(void) const
{
    bool locationValid = IsComponentFontFamilyValid(font_) && IsComponentLabelValid(label_) &&
        IsComponentIconValid(icon_) && IsComponentFontSizeValid(fontSize_) &&
        IsComponentButtonTypeValid(buttonType_);

    return locationValid && SecCompBase::IsValid();
}

bool LocationButton::operator==(const LocationButton& other) const
{
    if (font_ != other.font_ || label_ != other.label_ || icon_ != other.icon_ ||
        fontSize_ != other.fontSize_ || fontColor_.value != other.fontColor_.value ||
        bgColor_.value != other.bgColor_.value || buttonType_ != other.buttonType_) {
        SC_LOG_INFO(LABEL, "LocationButton is not equal");
        return false;
    }

    return SecCompBase::operator==(other);
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
