/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "sec_comp_stub_mock_test.h"

#include "sec_comp_dialog_callback.h"
#include "sec_comp_enhance_adapter.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"
#include "sec_comp_click_event_parcel.h"
#include "service_test_common.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompStubMockTest"};
}

void SecCompStubMockTest::SetUpTestCase()
{}

void SecCompStubMockTest::TearDownTestCase()
{}

void SecCompStubMockTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
    if (stub_ != nullptr) {
        return;
    }

    stub_ = new (std::nothrow) SecCompStubMock();
    ASSERT_NE(nullptr, stub_);
}

void SecCompStubMockTest::TearDown()
{
    stub_ = nullptr;
}

/**
 * @tc.name: OnRemoteRequestMock001
 * @tc.desc: Test on remote request
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompStubMockTest, OnRemoteRequestMock001, TestSize.Level0)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    data.WriteInterfaceToken(u"wrong");
    ASSERT_EQ(ERR_TRANSACTION_FAILED, stub_->OnRemoteRequest(static_cast<uint32_t>(
        ISecCompServiceIpcCode::COMMAND_REGISTER_SECURITY_COMPONENT), data, reply, option));
    data.FlushBuffer();
    reply.FlushBuffer();

    data.WriteInterfaceToken(u"OHOS.Security.SecurityComponent.ISecCompService");
    ASSERT_EQ(305, stub_->OnRemoteRequest(1000, data, reply, option));
}