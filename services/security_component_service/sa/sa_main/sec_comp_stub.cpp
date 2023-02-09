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
#include "sec_comp_stub.h"

#include "ipc_skeleton.h"
#include "sec_comp_click_event_parcel.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompStub"};
}  // namespace

int32_t SecCompStub::OnRemoteRequest(
    uint32_t code, MessageParcel& data, MessageParcel& reply, MessageOption& option)
{
    std::u16string descripter = SecCompStub::GetDescriptor();
    std::u16string remoteDescripter = data.ReadInterfaceToken();
    if (descripter != remoteDescripter) {
        SC_LOG_ERROR(LABEL, "Deal remote request fail, descriptor is not matched");
        return SC_SERVICE_ERROR_IPC_REQUEST_FAIL;
    }

    auto itFunc = requestFuncMap_.find(code);
    if (itFunc != requestFuncMap_.end()) {
        auto requestFunc = itFunc->second;
        if (requestFunc != nullptr) {
            return (this->*requestFunc)(data, reply);
        } else {
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
        }
    }
    return SC_OK;
}

int32_t SecCompStub::RegisterSecurityComponentInner(MessageParcel& data, MessageParcel& reply)
{
    uint32_t type;
    if (!data.ReadUint32(type)) {
        SC_LOG_ERROR(LABEL, "Read component type fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    if (type <= UNKNOWN_SC_TYPE || type >= MAX_SC_TYPE) {
        SC_LOG_ERROR(LABEL, "Security component type invalid");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    std::string componentInfo;
    if (!data.ReadString(componentInfo)) {
        SC_LOG_ERROR(LABEL, "Read component info fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    int32_t scId = INVALID_SC_ID;
    int32_t res = this->RegisterSecurityComponent(static_cast<SecCompType>(type), componentInfo, scId);
    if (!reply.WriteInt32(res)) {
        SC_LOG_ERROR(LABEL, "Register security component result fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    if (!reply.WriteInt32(scId)) {
        SC_LOG_ERROR(LABEL, "Register security component result fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    return SC_OK;
}

int32_t SecCompStub::UpdateSecurityComponentInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t scId;
    if (!data.ReadInt32(scId)) {
        SC_LOG_ERROR(LABEL, "Read component id fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    if (scId < 0) {
        SC_LOG_ERROR(LABEL, "Security component id invalid");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    std::string componentInfo;
    if (!data.ReadString(componentInfo)) {
        SC_LOG_ERROR(LABEL, "Read component info fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    int32_t res = this->UpdateSecurityComponent(scId, componentInfo);
    if (!reply.WriteInt32(res)) {
        SC_LOG_ERROR(LABEL, "Update security component result fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    return res;
}

int32_t SecCompStub::UnregisterSecurityComponentInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t scId;
    if (!data.ReadInt32(scId)) {
        SC_LOG_ERROR(LABEL, "Read component id fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    if (scId < 0) {
        SC_LOG_ERROR(LABEL, "Security component id invalid");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    int32_t res = this->UnregisterSecurityComponent(scId);
    if (!reply.WriteInt32(res)) {
        SC_LOG_ERROR(LABEL, "Unregister security component result fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }
    return SC_OK;
}

int32_t SecCompStub::ReportSecurityComponentClickEventInner(MessageParcel& data, MessageParcel& reply)
{
    int32_t scId;
    if (!data.ReadInt32(scId)) {
        SC_LOG_ERROR(LABEL, "Read component id fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    if (scId < 0) {
        SC_LOG_ERROR(LABEL, "Security component id invalid");
        return SC_SERVICE_ERROR_VALUE_INVALID;
    }

    std::string componentInfo;
    if (!data.ReadString(componentInfo)) {
        SC_LOG_ERROR(LABEL, "Read component info fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }
    sptr<SecCompClickEventParcel> touchInfoParcel = data.ReadParcelable<SecCompClickEventParcel>();
    if (touchInfoParcel == nullptr) {
        SC_LOG_ERROR(LABEL, "Read touchInfo info fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }

    int32_t res = this->ReportSecurityComponentClickEvent(scId, componentInfo, touchInfoParcel->touchInfoParams_);
    if (!reply.WriteInt32(res)) {
        SC_LOG_ERROR(LABEL, "Register security component result fail");
        return SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL;
    }
    return SC_OK;
}

SecCompStub::SecCompStub()
{
    requestFuncMap_[static_cast<uint32_t>(ISecCompService::InterfaceCode::REGISTER_SECURITY_COMPONENT)] =
        &SecCompStub::RegisterSecurityComponentInner;
    requestFuncMap_[static_cast<uint32_t>(ISecCompService::InterfaceCode::UPDATE_SECURITY_COMPONENT)] =
        &SecCompStub::UpdateSecurityComponentInner;
    requestFuncMap_[static_cast<uint32_t>(ISecCompService::InterfaceCode::UNREGISTER_SECURITY_COMPONENT)] =
        &SecCompStub::UnregisterSecurityComponentInner;
    requestFuncMap_[static_cast<uint32_t>(ISecCompService::InterfaceCode::REPORT_SECURITY_COMPONENT_CLICK_EVENT)] =
        &SecCompStub::ReportSecurityComponentClickEventInner;
}

SecCompStub::~SecCompStub()
{
    SC_LOG_ERROR(LABEL, "~SecCompStub");
    requestFuncMap_.clear();
    SC_LOG_ERROR(LABEL, "~SecCompStub end");
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
