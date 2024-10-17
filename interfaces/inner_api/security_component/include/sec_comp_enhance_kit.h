/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
#ifndef SECURITY_COMPONENT_ENHANCE_KITS_H
#define SECURITY_COMPONENT_ENHANCE_KITS_H

#include <cstdint>

namespace OHOS {
namespace Security {
namespace SecurityComponent {
struct __attribute__((visibility("default"))) SecCompEnhanceKit {
    static void InitClientEnhance();
    static int32_t SetEnhanceCfg(uint8_t* cfg, uint32_t cfgLen);
    static int32_t GetPointerEventEnhanceData(void* data, uint32_t dataLen,
        uint8_t* enhanceData, uint32_t& enHancedataLen);
};
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
#endif  // SECURITY_COMPONENT_ENHANCE_KITS_H
