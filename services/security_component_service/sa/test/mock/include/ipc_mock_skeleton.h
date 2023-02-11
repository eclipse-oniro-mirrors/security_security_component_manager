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
#ifndef SEC_COMP_MOCK_IPC_SKELETON_INCLUDE_H
#define SEC_COMP_MOCK_IPC_SKELETON_INCLUDE_H
#include <sys/types.h>

namespace OHOS {
class IPCSkeleton {
public:
    IPCSkeleton() = default;
    ~IPCSkeleton() = default;
    static uint32_t GetCallingTokenID()
    {
        return tokenID;
    };

    static pid_t GetCallingPid()
    {
        return pid;
    };

    static uint32_t tokenID;
    static pid_t pid;
};
}  // namespace OHOS
#endif  // SEC_COMP_MOCK_IPC_SKELETON_INCLUDE_H
