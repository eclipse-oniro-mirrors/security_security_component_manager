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

#include <cstdint>
#include <ctime>

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
static constexpr uint32_t MS_PER_SECOND = 1000;
static constexpr uint32_t NS_PER_MILLISECOND = 1000000;
}

uint64_t GetCurrentTime()
{
    struct timespec current = {0};
    int ret = clock_gettime(CLOCK_REALTIME, &current);
    if (ret != 0) {
        return 0;
    }
    return static_cast<int64_t>(current.tv_sec) * MS_PER_SECOND + current.tv_nsec / NS_PER_MILLISECOND;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
