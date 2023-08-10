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
#include <gtest/gtest.h>

#include "i_sec_comp_probe.h"
#include "location_button.h"
#include "location_button_sample_build.h"
#define private public
#include "sec_comp_caller_authorization.h"
#undef private
#include "sec_comp_err.h"
#include "sec_comp_info.h"
#include "sec_comp_kit.h"
#include "sec_comp_log.h"
#include "sec_comp_tool.h"
#include "sec_comp_ui_register.h"

using namespace testing::ext;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompReportClickWithoutHmacTest"};
static constexpr double TEST_COORDINATE = 100.0;

class MockUiSecCompProbe : public ISecCompProbe {
public:
    int32_t GetComponentInfo(int32_t nodeId, std::string& componentInfo) override
    {
        componentInfo = mockComponentInfo;
        return mockRes;
    }
    std::string mockComponentInfo;
    int32_t mockRes;
};

static MockUiSecCompProbe g_probe;
static void InitUiRegister()
{
    std::vector<uintptr_t> callerList;
    SecCompUiRegister registerCallback(callerList, &g_probe);
    SecCompCallerAuthorization::GetInstance().kitCallerList_.clear();
    SecCompCallerAuthorization::GetInstance().isInit_ = false;
}
}  // namespace

class SecCompReportClickWithoutHmacTest : public testing::Test {
public:
    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp() override;

    void TearDown() override;
};

void SecCompReportClickWithoutHmacTest::SetUpTestCase()
{
    InitUiRegister();
    SC_LOG_INFO(LABEL, "SecCompReportClickWithoutHmacTest.");
}

void SecCompReportClickWithoutHmacTest::TearDownTestCase()
{
    SC_LOG_INFO(LABEL, "SecCompReportClickWithoutHmacTest.");
}

void SecCompReportClickWithoutHmacTest::SetUp()
{
    SC_LOG_INFO(LABEL, "SetUp ok.");
}

void SecCompReportClickWithoutHmacTest::TearDown()
{
    SC_LOG_INFO(LABEL, "TearDown.");
}

/**
 * @tc.name: ReportClickWithoutHmac001
 * @tc.desc: test report click event without hmac data
 * @tc.type: FUNC
 * @tc.require: AR000HO9JM
 */
HWTEST_F(SecCompReportClickWithoutHmacTest, ReportClickWithoutHmac001, TestSize.Level1)
{
    nlohmann::json jsonRes;
    LocationButtonSampleBuild::BuildLocationComponentInfo(jsonRes);
    std::string locationInfo = jsonRes.dump();
    g_probe.mockComponentInfo = locationInfo;
    g_probe.mockRes = 0;

    int32_t scId;
    ASSERT_EQ(SC_OK, SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, locationInfo, scId));
    ASSERT_NE(-1, scId);

    struct SecCompClickEvent touch = {
        .touchX = TEST_COORDINATE,
        .touchY = TEST_COORDINATE,
        .timestamp = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count())
    };
    EXPECT_EQ(SC_SERVICE_ERROR_CLICK_EVENT_INVALID,
        SecCompKit::ReportSecurityComponentClickEvent(scId, locationInfo, touch, nullptr));
}
