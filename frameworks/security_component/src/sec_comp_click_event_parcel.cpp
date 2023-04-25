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
#include "sec_comp_click_event_parcel.h"
#include "sec_comp_log.h"
#include "securec.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompClickEventParcel"};
}

bool SecCompClickEventParcel::Marshalling(Parcel& out) const
{
    if (!(out.WriteDouble(this->touchInfoParams_.touchX)) || !(out.WriteDouble(this->touchInfoParams_.touchY))) {
        SC_LOG_ERROR(LABEL, "Write touch xy pointer fail");
        return false;
    }

    if (!(out.WriteUint64(this->touchInfoParams_.timestamp))) {
        SC_LOG_ERROR(LABEL, "Write touch timestamp fail");
        return false;
    }

    if (!(out.WriteUint32(this->touchInfoParams_.extraInfo.dataSize))) {
        SC_LOG_ERROR(LABEL, "Write extraInfo dataSize fail");
        return false;
    }

    if (this->touchInfoParams_.extraInfo.dataSize != 0 &&
        !(out.WriteBuffer(this->touchInfoParams_.extraInfo.data, this->touchInfoParams_.extraInfo.dataSize))) {
        SC_LOG_ERROR(LABEL, "Write touch extraInfo data fail");
        return false;
    }

    return true;
}

SecCompClickEventParcel* SecCompClickEventParcel::Unmarshalling(Parcel& in)
{
    SecCompClickEventParcel* touchInfoParcel = new (std::nothrow) SecCompClickEventParcel();
    if (touchInfoParcel == nullptr) {
        SC_LOG_ERROR(LABEL, "Alloc policy parcel fail");
        return nullptr;
    }

    SecCompClickEvent touchInfo;
    if (!in.ReadDouble(touchInfo.touchX) || !in.ReadDouble(touchInfo.touchY)) {
        SC_LOG_ERROR(LABEL, "Read touch xy porinter fail");
        delete touchInfoParcel;
        return nullptr;
    }

    if (!in.ReadUint64(touchInfo.timestamp)) {
        SC_LOG_ERROR(LABEL, "Read timestamp fail");
        delete touchInfoParcel;
        return nullptr;
    }

    if (!in.ReadUint32(touchInfo.extraInfo.dataSize)) {
        SC_LOG_ERROR(LABEL, "Read extraInfo data size fail");
        delete touchInfoParcel;
        return nullptr;
    }

    if (touchInfo.extraInfo.dataSize == 0) {
        touchInfoParcel->touchInfoParams_ = touchInfo;
        return touchInfoParcel;
    } else if (touchInfo.extraInfo.dataSize > MAX_EXTRA_SIZE) {
        SC_LOG_ERROR(LABEL, "Read extraInfo data size invalid");
        delete touchInfoParcel;
        return nullptr;
    }

    touchInfo.extraInfo.data = const_cast<uint8_t*>(in.ReadBuffer(touchInfo.extraInfo.dataSize));
    if (touchInfo.extraInfo.data == nullptr) {
        SC_LOG_ERROR(LABEL, "Read extraInfo data failed");
        delete touchInfoParcel;
        return nullptr;
    }

    touchInfoParcel->touchInfoParams_ = touchInfo;
    return touchInfoParcel;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
