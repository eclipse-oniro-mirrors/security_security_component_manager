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
#ifndef I_SECURITY_COMPONENT_SERVICE_H
#define I_SECURITY_COMPONENT_SERVICE_H

#include <string>
#include "iremote_broker.h"
#include "sec_comp_info.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
constexpr int32_t SA_ID_SECURITY_COMPONENT_SERVICE = 3506;

class ISecCompService : public IRemoteBroker {
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.security.ISecCompService");

    virtual int32_t RegisterSecurityComponent(SecCompType type,
        const std::string& componentInfo, int32_t& scId) = 0;
    virtual int32_t UpdateSecurityComponent(int32_t scId, const std::string& componentInfo) = 0;
    virtual int32_t UnregisterSecurityComponent(int32_t scId) = 0;
    virtual int32_t ReportSecurityComponentClickEvent(int32_t scId, const std::string& componentInfo,
        const SecCompClickEvent& touchInfo) = 0;

    enum class InterfaceCode {
        REGISTER_SECURITY_COMPONENT = 0xff01,
        UPDATE_SECURITY_COMPONENT = 0xff02,
        UNREGISTER_SECURITY_COMPONENT = 0xff03,
        REPORT_SECURITY_COMPONENT_CLICK_EVENT = 0xff04,
        GET_SECURITY_COMPONENT_ENHANCE_OBJECT = 0xff05,
    };
};
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
#endif  // I_SECURITY_COMPONENT_SERVICE_H
