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

#include "sec_comp_stub_test.h"
#include "sec_comp_log.h"
#include "sec_comp_err.h"
#include "sec_comp_click_event_parcel.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompStubTest"};
}

void SecCompStubTest::SetUpTestCase()
{}

void SecCompStubTest::TearDownTestCase()
{}

void SecCompStubTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
    if (stub_ != nullptr) {
        return;
    }

    stub_ = new (std::nothrow) SecCompStubMock();
    ASSERT_NE(nullptr, stub_);
}

void SecCompStubTest::TearDown()
{
    stub_ = nullptr;
}

/**
 * @tc.name: OnRemoteRequest001
 * @tc.desc: Test on remote request
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompStubTest, OnRemoteRequest001, TestSize.Level1)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    data.WriteInterfaceToken(u"wrong");
    ASSERT_EQ(stub_->OnRemoteRequest(static_cast<uint32_t>(ISecCompService::InterfaceCode::REGISTER_SECURITY_COMPONENT),
        data, reply, option), SC_SERVICE_ERROR_IPC_REQUEST_FAIL);

    data.WriteInterfaceToken(u"ohos.security.ISecCompService");
    ASSERT_EQ(stub_->OnRemoteRequest(static_cast<uint32_t>(ISecCompService::InterfaceCode::REGISTER_SECURITY_COMPONENT),
        data, reply, option), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteInterfaceToken(u"ohos.security.ISecCompService");
    ASSERT_EQ(stub_->OnRemoteRequest(0, data, reply, option), 0);
}

/**
 * @tc.name: RegisterSecurityComponentInner001
 * @tc.desc: Test register security component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompStubTest, RegisterSecurityComponentInner001, TestSize.Level1)
{
    MessageParcel data;
    MessageParcel reply;

    ASSERT_EQ(stub_->RegisterSecurityComponentInner(data, reply), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteUint32(UNKNOWN_SC_TYPE);
    ASSERT_EQ(stub_->RegisterSecurityComponentInner(data, reply), SC_SERVICE_ERROR_VALUE_INVALID);

    data.WriteUint32(MAX_SC_TYPE);
    ASSERT_EQ(stub_->RegisterSecurityComponentInner(data, reply), SC_SERVICE_ERROR_VALUE_INVALID);

    data.WriteUint32(LOCATION_COMPONENT);
    ASSERT_EQ(stub_->RegisterSecurityComponentInner(data, reply), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteUint32(LOCATION_COMPONENT);
    data.WriteString("");
    ASSERT_EQ(stub_->RegisterSecurityComponentInner(data, reply), SC_OK);
}

/**
 * @tc.name: UpdateSecurityComponentInner001
 * @tc.desc: Test update security component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompStubTest, UpdateSecurityComponentInner001, TestSize.Level1)
{
    MessageParcel data;
    MessageParcel reply;

    ASSERT_EQ(stub_->UpdateSecurityComponentInner(data, reply), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteInt32(-1);
    ASSERT_EQ(stub_->UpdateSecurityComponentInner(data, reply), SC_SERVICE_ERROR_VALUE_INVALID);

    data.WriteInt32(1);
    ASSERT_EQ(stub_->UpdateSecurityComponentInner(data, reply), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteInt32(1);
    data.WriteString("");
    ASSERT_EQ(stub_->UpdateSecurityComponentInner(data, reply), SC_OK);
}

/**
 * @tc.name: UnregisterSecurityComponentInner001
 * @tc.desc: Test unregister security component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompStubTest, UnregisterSecurityComponentInner001, TestSize.Level1)
{
    MessageParcel data;
    MessageParcel reply;
    ASSERT_EQ(stub_->UnregisterSecurityComponentInner(data, reply), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteInt32(-1);
    ASSERT_EQ(stub_->UnregisterSecurityComponentInner(data, reply), SC_SERVICE_ERROR_VALUE_INVALID);

    data.WriteInt32(1);
    ASSERT_EQ(stub_->UnregisterSecurityComponentInner(data, reply), SC_OK);
}

/**
 * @tc.name: ReportSecurityComponentClickEventInner001
 * @tc.desc: Test report click event
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompStubTest, ReportSecurityComponentClickEventInner001, TestSize.Level1)
{
    MessageParcel data;
    MessageParcel reply;
    ASSERT_EQ(stub_->ReportSecurityComponentClickEventInner(data, reply), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteInt32(-1);
    ASSERT_EQ(stub_->ReportSecurityComponentClickEventInner(data, reply), SC_SERVICE_ERROR_VALUE_INVALID);

    data.WriteInt32(1);
    ASSERT_EQ(stub_->ReportSecurityComponentClickEventInner(data, reply), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteInt32(1);
    data.WriteString("");
    ASSERT_EQ(stub_->ReportSecurityComponentClickEventInner(data, reply), SC_SERVICE_ERROR_PARCEL_OPERATE_FAIL);

    data.WriteInt32(1);
    data.WriteString("");
    SecCompClickEvent touchInfo;
    sptr<SecCompClickEventParcel> parcel = new (std::nothrow) SecCompClickEventParcel();
    parcel->touchInfoParams_ = touchInfo;
    data.WriteParcelable(parcel);
    ASSERT_EQ(stub_->ReportSecurityComponentClickEventInner(data, reply), SC_OK);
}
