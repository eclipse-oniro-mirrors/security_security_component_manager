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
#ifndef SECURITY_COMPONENT_BASE_H
#define SECURITY_COMPONENT_BASE_H

#include "nlohmann/json.hpp"
#include "sec_comp_info.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
class SecCompBase {
public:
    SecCompBase() = default;
    virtual ~SecCompBase() = default;

    SecCompType type_ = UNKNOWN_SC_TYPE;
    SecCompRect rect_;

    virtual void FromJson(const nlohmann::json& jsonSrc);
    virtual void ToJson(nlohmann::json& jsonRes) const;
    virtual bool IsValid(void) const;
    bool operator==(const SecCompBase& other) const;
};
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
#endif  // SECURITY_COMPONENT_BASE_H

