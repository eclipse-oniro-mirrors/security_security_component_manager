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
#ifndef SECURITY_COMPONENT_STUB_H
#define SECURITY_COMPONENT_STUB_H

#include <map>
#include "i_sec_comp_service.h"
#include "iremote_stub.h"
#include "nocopyable.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
class SecCompStub : public IRemoteStub<ISecCompService> {
public:
    SecCompStub();
    virtual ~SecCompStub();

    int OnRemoteRequest(uint32_t code, MessageParcel& data, MessageParcel& reply, MessageOption& option) override;

private:
    int32_t RegisterSecurityComponentInner(MessageParcel& data, MessageParcel& reply);
    int32_t UpdateSecurityComponentInner(MessageParcel& data, MessageParcel& reply);
    int32_t UnregisterSecurityComponentInner(MessageParcel& data, MessageParcel& reply);
    int32_t ReportSecurityComponentClickEventInner(MessageParcel& data, MessageParcel& reply);
    int32_t VerifySavePermissionInner(MessageParcel& data, MessageParcel& reply);
    int32_t GetEnhanceRemoteObjectInner(MessageParcel& data, MessageParcel& reply);
    int32_t PreRegisterSecCompProcessInner(MessageParcel& data, MessageParcel& reply);
    bool IsMediaLibraryCalling();

    using RequestFuncType = int32_t (SecCompStub::*)(MessageParcel& data, MessageParcel& reply);
    std::map<uint32_t, RequestFuncType> requestFuncMap_;
    AccessToken::AccessTokenID mediaLibraryTokenId_ = 0;
};
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
#endif  // SECURITY_COMPONENT_STUB_H
