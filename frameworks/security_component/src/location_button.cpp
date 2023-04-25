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
#include <tuple>
#include "sec_comp_err.h"
#include "sec_comp_log.h"
#include "sec_comp_tool.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "LocationButton"};
static const std::string JSON_STYLE_TAG = "style";
static const std::string JSON_TEXT_TAG = "text";
static const std::string JSON_ICON_TAG = "icon";
static const std::string JSON_BG_TAG = "bg";
}

bool LocationButton::ParseStyle(const nlohmann::json& json, const std::string& tag)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_object()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }
    auto jsonStyle = json.at(tag);
    if (!ParseTypeValue<LocationDesc>(jsonStyle, JSON_TEXT_TAG, text_,
        LocationDesc::UNKNOWN_TEXT, LocationDesc::MAX_LABEL_TYPE)) {
        return false;
    }

    if (!ParseTypeValue<LocationIcon>(jsonStyle, JSON_ICON_TAG, icon_,
        LocationIcon::UNKNOWN_ICON, LocationIcon::MAX_ICON_TYPE)) {
        return false;
    }

    if (!ParseTypeValue<LocationBackground>(jsonStyle, JSON_BG_TAG, bg_,
        LocationBackground::UNKNOWN_BG, LocationBackground::MAX_BG_TYPE)) {
        return false;
    }

    return true;
}

bool LocationButton::FromJson(const nlohmann::json& jsonSrc)
{
    SC_LOG_DEBUG(LABEL, "LocationButton info %{public}s.", jsonSrc.dump().c_str());
    SecCompBase::FromJson(jsonSrc);
    if (!ParseStyle(jsonSrc, JSON_STYLE_TAG)) {
        SC_LOG_ERROR(LABEL, "Parse style %{public}s.", jsonSrc.dump().c_str());
        return false;
    }
    return true;
}

void LocationButton::ToJson(nlohmann::json& jsonRes) const
{
    SecCompBase::ToJson(jsonRes);
    jsonRes[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, text_ },
        { JSON_ICON_TAG, icon_ },
        { JSON_BG_TAG, bg_ },
    };
}

std::string LocationButton::ToJsonStr() const
{
    nlohmann::json json;
    ToJson(json);
    return json.dump();
}

bool LocationButton::CompareComponentBasicInfo(SecCompBase *other) const
{
    if (!SecCompBase::CompareComponentBasicInfo(other)) {
        SC_LOG_ERROR(LABEL, "SecComp base not equal.");
        return false;
    }
    LocationButton* otherLocationButton = reinterpret_cast<LocationButton *>(other);
    if (otherLocationButton == nullptr) {
        SC_LOG_ERROR(LABEL, "other is not location button.");
        return false;
    }
    return (icon_ == otherLocationButton->icon_) && (text_ == otherLocationButton->text_) &&
        (bg_ == otherLocationButton->bg_);
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
