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
#ifndef I_SECURITY_COMPONENT_SAVE_BUTTON_H
#define I_SECURITY_COMPONENT_SAVE_BUTTON_H

#include <string>
#include "nlohmann/json.hpp"
#include "sec_comp_base.h"
#include "sec_comp_info.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
enum class SaveDesc : int32_t {
    UNKNOWN_TEXT = -2,
    NO_TEXT = -1,
    DOWNLOAD = 0,
    DOWNLOAD_FILE,
    SAVE,
    SAVE_IMAGE,
    SAVE_FILE,
    DOWNLOAD_AND_SHARE,
    RECEIVE,
    CONTINUE_TO_RECEIVE,
    MAX_LABEL_TYPE
};

enum class SaveIcon : int32_t {
    UNKNOWN_ICON = -2,
    NO_ICON = -1,
    FILLED_ICON = 0,
    LINE_ICON = 1,
    MAX_ICON_TYPE
};

class SaveButton : public SecCompBase {
public:
    virtual bool IsParamValid() override;
    virtual bool CompareComponentBasicInfo(SecCompBase *other) const override;
private:
    bool ParseStyle(const nlohmann::json& json, const std::string& tag);
};
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
#endif  // I_SECURITY_COMPONENT_SAVE_BUTTON_H
