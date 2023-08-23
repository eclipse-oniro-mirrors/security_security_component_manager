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
#ifndef SECURITY_COMPONENT_INTERFACES_INNER_KITS_ACCESSTOKEN_KIT_H
#define SECURITY_COMPONENT_INTERFACES_INNER_KITS_ACCESSTOKEN_KIT_H

#include <string>
#include "access_token.h"

namespace OHOS {
namespace Security {
namespace AccessToken {
struct HapTokenInfo {
};

class AccessTokenKit {
public:
    static int RevokePermission(AccessTokenID tokenID, const std::string& permissionName, int flag)
    {
        return 0;
    };

    static int GrantPermission(AccessTokenID tokenID, const std::string& permissionName, int flag)
    {
        return 0;
    };

    static int GetHapTokenInfo(AccessTokenID tokenID, HapTokenInfo& hapTokenInfoRes)
    {
        return 0;
    };

    static AccessTokenID GetHapTokenID(int32_t userID, const std::string& bundleName, int32_t instIndex)
    {
        return 0;
    };

    static ATokenTypeEnum GetTokenTypeFlag(AccessTokenID tokenID)
    {
        AccessTokenIDInner *idInner = reinterpret_cast<AccessTokenIDInner *>(&tokenID);
        return static_cast<ATokenTypeEnum>(idInner->type);
    };
};
} // namespace SECURITY_COMPONENT_INTERFACES_INNER_KITS_ACCESSTOKEN_KIT_H
} // namespace Security
} // namespace OHOS
#endif
