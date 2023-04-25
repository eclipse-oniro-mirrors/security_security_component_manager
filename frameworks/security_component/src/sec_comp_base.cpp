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

#include "sec_comp_base.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompBase"};
static const std::string JSON_RECT = "rect";
static const std::string JSON_SC_TYPE = "type";
static const std::string JSON_RECT_X = "x";
static const std::string JSON_RECT_Y = "y";
static const std::string JSON_RECT_WIDTH = "width";
static const std::string JSON_RECT_HEIGHT = "height";

static const std::string JSON_SIZE_TAG = "size";
static const std::string JSON_FONT_SIZE_TAG = "fontSize";
static const std::string JSON_ICON_SIZE_TAG = "iconSize";
static const std::string JSON_PADDING_SIZE_TAG = "paddingSize";
static const std::string JSON_PADDING_LEFT_TAG = "left";
static const std::string JSON_PADDING_TOP_TAG = "top";
static const std::string JSON_PADDING_RIGHT_TAG = "right";
static const std::string JSON_PADDING_BOTTOM_TAG = "bottom";
static const std::string JSON_TEXT_ICON_PADDING_TAG = "textIconPadding";
static const std::string JSON_RECT_WIDTH_TAG = "width";
static const std::string JSON_RECT_HEIGHT_TAG = "height";

static const std::string JSON_COLORS_TAG = "colors";
static const std::string JSON_FONT_COLOR_TAG = "fontColor";
static const std::string JSON_ICON_COLOR_TAG = "iconColor";
static const std::string JSON_BG_COLOR_TAG = "bgColor";

static const std::string JSON_BORDER_TAG = "border";
static const std::string JSON_BORDER_WIDTH_TAG = "borderWidth";
static const std::string JSON_PARENT_TAG = "parent";
static const std::string JSON_PARENT_EFFECT_TAG = "parentEffect";
}

bool SecCompBase::ParseDimension(const nlohmann::json& json, const std::string& tag, DimensionT& res)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_number_float()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }

    res = json.at(tag).get<double>();
    return true;
}

bool SecCompBase::ParseColor(const nlohmann::json& json, const std::string& tag, SecCompColor& res)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_number()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }

    res.value = json.at(tag).get<uint32_t>();
    return true;
}

bool SecCompBase::ParseBool(const nlohmann::json& json, const std::string& tag, bool& res)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_boolean()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }

    res = json.at(tag).get<bool>();
    return true;
}

bool SecCompBase::ParsePadding(const nlohmann::json& json, const std::string& tag, PaddingSize& res)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_object()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }

    auto jsonPadding = json.at(tag);
    if (!ParseDimension(jsonPadding, JSON_PADDING_TOP_TAG, res.top)) {
        return false;
    }
    if (!ParseDimension(jsonPadding, JSON_PADDING_RIGHT_TAG, res.right)) {
        return false;
    }
    if (!ParseDimension(jsonPadding, JSON_PADDING_BOTTOM_TAG, res.bottom)) {
        return false;
    }
    if (!ParseDimension(jsonPadding, JSON_PADDING_LEFT_TAG, res.left)) {
        return false;
    }
    return true;
}

bool SecCompBase::ParseColors(const nlohmann::json& json, const std::string& tag)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_object()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }
    auto jsonColors = json.at(tag);
    if (!ParseColor(jsonColors, JSON_FONT_COLOR_TAG, fontColor_)) {
        return false;
    }
    if (!ParseColor(jsonColors, JSON_ICON_COLOR_TAG, iconColor_)) {
        return false;
    }
    if (!ParseColor(jsonColors, JSON_BG_COLOR_TAG, bgColor_)) {
        return false;
    }
    return true;
}

bool SecCompBase::ParseBorders(const nlohmann::json& json, const std::string& tag)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_object()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }
    auto jsonBorder = json.at(tag);
    return ParseDimension(jsonBorder, JSON_BORDER_WIDTH_TAG, borderWidth_);
}

bool SecCompBase::ParseSize(const nlohmann::json& json, const std::string& tag)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_object()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }

    auto jsonSize = json.at(tag);
    if (!ParseDimension(jsonSize, JSON_FONT_SIZE_TAG, fontSize_)) {
        return false;
    }

    if (!ParseDimension(jsonSize, JSON_ICON_SIZE_TAG, iconSize_)) {
        return false;
    }

    if (!ParseDimension(jsonSize, JSON_TEXT_ICON_PADDING_TAG, textIconPadding_)) {
        return false;
    }

    if (!ParsePadding(jsonSize, JSON_PADDING_SIZE_TAG, padding_)) {
        return false;
    }

    return true;
}

bool SecCompBase::ParseParent(const nlohmann::json& json, const std::string& tag)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_object()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }
    auto jsonParent = json.at(tag);
    return ParseBool(jsonParent, JSON_PARENT_EFFECT_TAG, parentEffect_);
}

bool SecCompBase::ParseRect(const nlohmann::json& json, const std::string& tag)
{
    if ((json.find(tag) == json.end()) || !json.at(tag).is_object()) {
        SC_LOG_ERROR(LABEL, "has not %{public}s.", tag.c_str());
        return false;
    }

    auto jsonSize = json.at(tag);
    if (!ParseDimension(jsonSize, JSON_RECT_X, rect_.x_)) {
        return false;
    }

    if (!ParseDimension(jsonSize, JSON_RECT_Y, rect_.y_)) {
        return false;
    }

    if (!ParseDimension(jsonSize, JSON_RECT_WIDTH, rect_.width_)) {
        return false;
    }

    if (!ParseDimension(jsonSize, JSON_RECT_HEIGHT, rect_.height_)) {
        return false;
    }

    return true;
}

bool SecCompBase::FromJson(const nlohmann::json& jsonSrc)
{
    if (!ParseTypeValue<SecCompType>(jsonSrc, JSON_SC_TYPE, type_,
        SecCompType::UNKNOWN_SC_TYPE, SecCompType::MAX_SC_TYPE)) {
        return false;
    }

    if (!ParseRect(jsonSrc, JSON_RECT)) {
        return false;
    }
    if (!ParseSize(jsonSrc, JSON_SIZE_TAG)) {
        return false;
    }

    if (!ParseColors(jsonSrc, JSON_COLORS_TAG)) {
        return false;
    }

    if (!ParseBorders(jsonSrc, JSON_BORDER_TAG)) {
        return false;
    }
    if (!ParseParent(jsonSrc, JSON_PARENT_TAG)) {
        return false;
    }

    return true;
}

void SecCompBase::ToJson(nlohmann::json& jsonRes) const
{
    jsonRes[JSON_SC_TYPE] = type_;
    jsonRes[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, rect_.x_},
        {JSON_RECT_Y, rect_.y_},
        {JSON_RECT_WIDTH, rect_.width_},
        {JSON_RECT_HEIGHT, rect_.height_}
    };
    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, padding_.top },
        { JSON_PADDING_RIGHT_TAG, padding_.right },
        { JSON_PADDING_BOTTOM_TAG, padding_.bottom },
        { JSON_PADDING_LEFT_TAG, padding_.left },
    };

    jsonRes[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, fontSize_ },
        { JSON_ICON_SIZE_TAG, iconSize_ },
        { JSON_TEXT_ICON_PADDING_TAG, textIconPadding_ },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };

    jsonRes[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, fontColor_.value },
        { JSON_ICON_COLOR_TAG, iconColor_.value },
        { JSON_BG_COLOR_TAG, bgColor_.value }
    };

    jsonRes[JSON_BORDER_TAG] = nlohmann::json {
        { JSON_BORDER_WIDTH_TAG, borderWidth_ },
    };
    jsonRes[JSON_PARENT_TAG] = nlohmann::json {
        { JSON_PARENT_EFFECT_TAG, parentEffect_ },
    };
}

bool SecCompBase::CompareComponentBasicInfo(SecCompBase *other) const
{
    if (other == nullptr) {
        SC_LOG_ERROR(LABEL, "other is invalid.");
        return false;
    }

    auto leftValue = std::tie(type_, fontSize_, iconSize_, textIconPadding_, padding_.top, padding_.right,
        padding_.bottom, padding_.left, fontColor_.value, bgColor_.value, iconColor_.value, borderWidth_);
    auto rightValue = std::tie(other->type_, other->fontSize_, other->iconSize_, other->textIconPadding_,
        other->padding_.top, other->padding_.right, other->padding_.bottom, other->padding_.left,
        other->fontColor_.value, other->bgColor_.value, other->iconColor_.value, other->borderWidth_);

    return (leftValue == rightValue);
}
}  // namespace base
}  // namespace Security
}  // namespace OHOS

