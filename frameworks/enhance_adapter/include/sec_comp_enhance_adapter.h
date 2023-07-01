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
#ifndef SECURITY_COMPONENT_ENHANCE_ADAPTER_H
#define SECURITY_COMPONENT_ENHANCE_ADAPTER_H
#include <mutex>
#include "parcel.h"
#include "sec_comp_info.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
enum EnhanceInterfaceType {
    SEC_COMP_ENHANCE_INPUT_INTERFACE = 0,
    SEC_COMP_ENHANCE_SRV_INTERFACE = 1,
};

class SecCompEnhanceCfgBase {
};

class SecCompEnhanceInputInterface {
public:
    virtual int32_t SetEnhanceCfg(uint8_t* cfg, uint32_t cfgLen) = 0;
    virtual int32_t GetPointerEventEnhanceData(void* data, uint32_t dataLen,
        uint8_t* enhanceData, uint32_t& enHancedataLen) = 0;
};

class SecCompEnhanceServiceInterface {
public:
    virtual int32_t EnableInputEnhance() = 0;
    virtual int32_t DisableInputEnhance() = 0;
    virtual int32_t CheckExtraInfo(const SecCompClickEvent& touchInfo) = 0;
};

struct SecCompEnhanceAdapter {
    static void InitEnhanceHandler(EnhanceInterfaceType type);
    static int32_t SetEnhanceCfg(uint8_t* cfg, uint32_t cfgLen);
    static int32_t GetPointerEventEnhanceData(void* data, uint32_t dataLen,
        uint8_t* enhanceData, uint32_t& enHancedataLen);
    static int32_t CheckExtraInfo(const SecCompClickEvent& touchInfo);

    static int32_t EnableInputEnhance();
    static int32_t DisableInputEnhance();

    static SecCompEnhanceInputInterface* inputHandler;
    static bool isEnhanceInputHandlerInit;

    static SecCompEnhanceServiceInterface* srvHandler;
    static bool isEnhanceSrvHandlerInit;

    static std::mutex initMtx;
};
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
#endif  // SECURITY_COMPONENT_ENHANCE_ADAPTER_H
