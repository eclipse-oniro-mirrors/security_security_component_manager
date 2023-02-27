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
#include "sec_comp_enhance_adapter.h"
#include <dlfcn.h>
#include "sec_comp_err.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompEnhanceAdapter"};

#ifdef _ARM64_
static const std::string LIB_PATH = "/system/lib64/";
#else
static const std::string LIB_PATH = "/system/lib/";
#endif
static const std::string ENHANCE_INPUT_INTERFACE_LIB = LIB_PATH + "libsec_comp_input_enhance.z.so";
static const std::string ENHANCE_SRV_INTERFACE_LIB = LIB_PATH + "libsec_comp_service_enhance.z.so";
}

SecCompEnhanceInputInterface* SecCompEnhanceAdapter::inputHandler = nullptr;
bool SecCompEnhanceAdapter::isEnhanceInputHandlerInit = false;

SecCompEnhanceServiceInterface* SecCompEnhanceAdapter::srvHandler = nullptr;
bool SecCompEnhanceAdapter::isEnhanceSrvHandlerInit = false;

std::mutex SecCompEnhanceAdapter::initMtx;

void SecCompEnhanceAdapter::InitEnhanceHandler(EnhanceInterfaceType type)
{
    std::unique_lock<std::mutex> lck(initMtx);
    std::string libPath = "";
    switch (type) {
        case SEC_COMP_ENHANCE_INPUT_INTERFACE:
            libPath = ENHANCE_INPUT_INTERFACE_LIB;
            isEnhanceInputHandlerInit = true;
            break;
        case SEC_COMP_ENHANCE_SRV_INTERFACE:
            libPath = ENHANCE_SRV_INTERFACE_LIB;
            isEnhanceSrvHandlerInit = true;
            break;
        default:
            break;
    }
    if (dlopen(libPath.c_str(), RTLD_LAZY) == nullptr) {
        SC_LOG_ERROR(LABEL, "init enhance lib %{public}s failed", libPath.c_str());
    } else {
        SC_LOG_INFO(LABEL, "init enhance lib %{public}s ok", libPath.c_str());
    }
}

int32_t SecCompEnhanceAdapter::MarshallEnhanceCfg(SecCompEnhanceCfgBase* cfg, Parcel& out)
{
    if (!isEnhanceInputHandlerInit) {
        InitEnhanceHandler(SEC_COMP_ENHANCE_INPUT_INTERFACE);
    }
    if (inputHandler != nullptr) {
        if (!inputHandler->MarshallEnhanceCfg(cfg, out)) {
            return SC_ENHANCE_ERROR_OPER_FAIL;
        } else {
            return SC_OK;
        }
    }
    return SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE;
}

int32_t SecCompEnhanceAdapter::UnmarshallEnhanceCfg(Parcel& in, SecCompEnhanceCfgBase*& result)
{
    if (!isEnhanceInputHandlerInit) {
        InitEnhanceHandler(SEC_COMP_ENHANCE_INPUT_INTERFACE);
    }
    if (inputHandler != nullptr) {
        result = inputHandler->UnmarshallEnhanceCfg(in);
        return SC_OK;
    }
    return SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE;
}

int32_t SecCompEnhanceAdapter::SetEnhanceCfg(SecCompEnhanceCfgBase* cfg)
{
    if (!isEnhanceInputHandlerInit) {
        InitEnhanceHandler(SEC_COMP_ENHANCE_INPUT_INTERFACE);
    }
    if (inputHandler != nullptr) {
        return inputHandler->SetEnhanceCfg(cfg);
    }
    return SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE;
}

int32_t SecCompEnhanceAdapter::GetPointerEventEnhanceData(void *data, uint32_t dataLen,
    uint8_t** enhanceData, uint32_t& enHancedataLen)
{
    if (!isEnhanceInputHandlerInit) {
        InitEnhanceHandler(SEC_COMP_ENHANCE_INPUT_INTERFACE);
    }
    if (inputHandler != nullptr) {
        return inputHandler->GetPointerEventEnhanceData(data, dataLen, enhanceData, enHancedataLen);
    }
    return SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE;
}

int32_t SecCompEnhanceAdapter::EnableInputEnhance()
{
    if (!isEnhanceSrvHandlerInit) {
        InitEnhanceHandler(SEC_COMP_ENHANCE_SRV_INTERFACE);
    }
    if (srvHandler != nullptr) {
        return srvHandler->EnableInputEnhance();
    }
    return SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE;
}

int32_t SecCompEnhanceAdapter::DisableInputEnhance()
{
    if (!isEnhanceSrvHandlerInit) {
        InitEnhanceHandler(SEC_COMP_ENHANCE_SRV_INTERFACE);
    }
    if (srvHandler != nullptr) {
        return srvHandler->DisableInputEnhance();
    }
    return SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
