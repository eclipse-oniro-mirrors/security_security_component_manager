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
}

void from_json(const nlohmann::json& j, SecCompRect& p)
{
    SecCompRect res;
    if (j.is_discarded()) {
        SC_LOG_ERROR(LABEL, "component info invalid");
        return ;
    }

    if (j.find(JSON_RECT_X) != j.end() && j.at(JSON_RECT_X).is_number_float()) {
        res.x_ = j.at(JSON_RECT_X).get<double>();
    } else {
        SC_LOG_ERROR(LABEL, "x is invalid");
        return;
    }

    if (j.find(JSON_RECT_Y) != j.end() && j.at(JSON_RECT_Y).is_number_float()) {
        res.y_ = j.at(JSON_RECT_Y).get<double>();
    } else {
        SC_LOG_ERROR(LABEL, "y is invalid.");
        return;
    }

    if (j.find(JSON_RECT_WIDTH) != j.end() && j.at(JSON_RECT_WIDTH).is_number_float()) {
        res.width_ = j.at(JSON_RECT_WIDTH).get<double>();
    } else {
        SC_LOG_ERROR(LABEL, "width is invalid.");
        return;
    }

    if (j.find(JSON_RECT_HEIGHT) != j.end() && j.at(JSON_RECT_HEIGHT).is_number_float()) {
        res.height_ = j.at(JSON_RECT_HEIGHT).get<double>();
    } else {
        SC_LOG_ERROR(LABEL, "height is invalid.");
        return;
    }

    p = res;
}

void SecCompBase::FromJson(const nlohmann::json& jsonSrc)
{
    if (jsonSrc.find(JSON_SC_TYPE) != jsonSrc.end() && jsonSrc.at(JSON_SC_TYPE).is_number()) {
        int32_t type = jsonSrc.at(JSON_SC_TYPE).get<int32_t>();
        if (IsComponentTypeValid(type)) {
            type_ = static_cast<SecCompType>(type);
        } else {
            SC_LOG_ERROR(LABEL, "type is invalid.");
            return;
        }
    } else {
        SC_LOG_ERROR(LABEL, "get type fail.");
        return;
    }

    if (jsonSrc.find(JSON_RECT) != jsonSrc.end() && jsonSrc.at(JSON_RECT).is_object()) {
        rect_ = jsonSrc.at(JSON_RECT).get<SecCompRect>();
    } else {
        SC_LOG_ERROR(LABEL, "rect is invalid.");
        return;
    }
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
}

bool SecCompBase::IsValid(void) const
{
    return IsComponentTypeValid(type_) && rect_.IsValid();
}

bool SecCompBase::operator==(const SecCompBase& other) const
{
    bool result = (type_ == other.type_ && rect_.x_ == other.rect_.x_ && rect_.y_ == other.rect_.y_ &&
        rect_.width_ == other.rect_.width_ && rect_.height_ == other.rect_.height_);
    if (!result) {
        SC_LOG_INFO(LABEL, "SecCompBase is not equal");
    }
    return result;
}
}  // namespace base
}  // namespace Security
}  // namespace OHOS

