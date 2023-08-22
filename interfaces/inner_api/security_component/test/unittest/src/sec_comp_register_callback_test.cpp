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
#define private public
#include "sec_comp_caller_authorization.h"
#undef private
#include "sec_comp_client.h"
#include "sec_comp_enhance_adapter.h"
#include "sec_comp_err.h"
#include "sec_comp_info.h"
#include "sec_comp_kit.h"
#include "sec_comp_log.h"
#include "sec_comp_tool.h"
#include "sec_comp_ui_register.h"
#include "test_common.h"
#include "token_setproc.h"

using namespace testing::ext;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompRegisterCallbackTest"};

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

class SecCompRegisterCallbackTest : public testing::Test {
public:
    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp() override;

    void TearDown() override;
};

void SecCompRegisterCallbackTest::SetUpTestCase()
{
    InitUiRegister();
    SC_LOG_INFO(LABEL, "SecCompRegisterCallbackTest.");
}

void SecCompRegisterCallbackTest::TearDownTestCase()
{
    SC_LOG_INFO(LABEL, "SecCompRegisterCallbackTest.");
}

void SecCompRegisterCallbackTest::SetUp()
{
    SC_LOG_INFO(LABEL, "SetUp ok.");
}

void SecCompRegisterCallbackTest::TearDown()
{
    SC_LOG_INFO(LABEL, "TearDown.");
}

/**
 * @tc.name: RegisterWithoutPreprocess001
 * @tc.desc: test register without preprocess
 * @tc.type: FUNC
 * @tc.require: AR000HO9JM
 */
HWTEST_F(SecCompRegisterCallbackTest, RegisterWithoutPreprocess001, TestSize.Level1)
{
    nlohmann::json jsonRes;
    TestCommon::BuildLocationComponentInfo(jsonRes);
    std::string locationInfo = jsonRes.dump();

    SecCompEnhanceAdapter::InitEnhanceHandler(SEC_COMP_ENHANCE_CLIENT_INTERFACE);
    int32_t scId;
    ASSERT_EQ(SC_ENHANCE_ERROR_CHALLENGE_CHECK_FAIL,
        SecCompClient::GetInstance().RegisterSecurityComponent(LOCATION_COMPONENT, locationInfo, scId));
    ASSERT_EQ(-1, scId);
}

/**
 * @tc.name: Register001
 * @tc.desc: test register security component success.
 * @tc.type: FUNC
 * @tc.require: AR000HO9JM
 */
HWTEST_F(SecCompRegisterCallbackTest, Register001, TestSize.Level1)
{
    nlohmann::json jsonRes;
    TestCommon::BuildLocationComponentInfo(jsonRes);
    std::string locationInfo = jsonRes.dump();
    g_probe.mockComponentInfo = locationInfo;
    g_probe.mockRes = 0;

    int32_t scId;
    ASSERT_EQ(SC_OK, SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, locationInfo, scId));
    ASSERT_NE(-1, scId);

    EXPECT_EQ(SC_OK, SecCompKit::UnregisterSecurityComponent(scId));
}

/**
 * @tc.name: Register002
 * @tc.desc: test register callback failed.
 * @tc.type: FUNC
 * @tc.require: AR000HO9JM
 */
HWTEST_F(SecCompRegisterCallbackTest, Register002, TestSize.Level1)
{
    nlohmann::json jsonRes;
    TestCommon::BuildLocationComponentInfo(jsonRes);
    std::string locationInfo = jsonRes.dump();
    g_probe.mockComponentInfo = locationInfo;
    g_probe.mockRes = -1;

    int32_t scId;
    ASSERT_EQ(SC_ENHANCE_ERROR_CALLBACK_OPER_FAIL,
        SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, locationInfo, scId));
    ASSERT_EQ(-1, scId);
}

/**
 * @tc.name: Register003
 * @tc.desc: test register in MaliciousAppList.
 * @tc.type: FUNC
 * @tc.require: AR000HO9JM
 */
HWTEST_F(SecCompRegisterCallbackTest, Register003, TestSize.Level1)
{
    nlohmann::json jsonRes;
    TestCommon::BuildLocationComponentInfo(jsonRes);
    std::string locationInfo = jsonRes.dump();
    g_probe.mockComponentInfo = locationInfo;
    g_probe.mockRes = 0;

    int32_t scId;
    ASSERT_EQ(SC_OK, SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, locationInfo, scId));
}

/**
 * @tc.name: RegisterSecurityComponent001
 * @tc.desc: Test register security component check touch info failed
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompRegisterCallbackTest, RegisterSecurityComponent001, TestSize.Level1)
{
    system("param set sec.comp.enhance 1");
    nlohmann::json jsonRes;
    TestCommon::BuildSaveComponentInfo(jsonRes);
    std::string saveInfo = jsonRes.dump();
    int32_t scId;

    EXPECT_EQ(SC_OK, SecCompKit::RegisterSecurityComponent(SAVE_COMPONENT, saveInfo, scId));
    uint8_t data[TestCommon::MAX_HMAC_SIZE] = { 0 };
    struct SecCompClickEvent touchInfo = {
        .touchX = TestCommon::TEST_COORDINATE,
        .touchY = TestCommon::TEST_COORDINATE,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count())
    };
    touchInfo.extraInfo.dataSize = TestCommon::MAX_HMAC_SIZE;
    touchInfo.extraInfo.data = data;
    EXPECT_EQ(SC_SERVICE_ERROR_CLICK_EVENT_INVALID,
        SecCompKit::ReportSecurityComponentClickEvent(scId, saveInfo, touchInfo, nullptr));
    EXPECT_EQ(SC_OK, SecCompKit::UnregisterSecurityComponent(scId));
    system("param set sec.comp.enhance 0");
}

/**
 * @tc.name: RegisterSecurityComponent002
 * @tc.desc: Test register security component permission grant failed
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompRegisterCallbackTest, RegisterSecurityComponent002, TestSize.Level1)
{
    system("param set sec.comp.enhance 1");
    nlohmann::json jsonRes;
    TestCommon::BuildSaveComponentInfo(jsonRes);
    std::string saveInfo = jsonRes.dump();
    int32_t scId;

    EXPECT_EQ(SC_OK, SecCompKit::RegisterSecurityComponent(SAVE_COMPONENT, saveInfo, scId));
    uint8_t data[TestCommon::MAX_HMAC_SIZE] = { 0 };
    struct SecCompClickEvent touchInfo = {
        .touchX = TestCommon::TEST_COORDINATE,
        .touchY = TestCommon::TEST_COORDINATE,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TestCommon::TIME_CONVERSION_UNIT
    };
    touchInfo.extraInfo.dataSize = TestCommon::MAX_HMAC_SIZE;
    touchInfo.extraInfo.data = data;
    EXPECT_EQ(SC_OK, SecCompKit::ReportSecurityComponentClickEvent(scId, saveInfo, touchInfo, nullptr));
    EXPECT_EQ(SC_OK, SecCompKit::UnregisterSecurityComponent(scId));
    system("param set sec.comp.enhance 0");
}

/**
 * @tc.name: ReportSecurityComponentClickEvent003
 * @tc.desc: Test register security component success
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompRegisterCallbackTest, ReportSecurityComponentClickEvent003, TestSize.Level1)
{
    system("param set sec.comp.enhance 1");
    nlohmann::json jsonRes;
    TestCommon::BuildSaveComponentInfo(jsonRes);
    std::string saveInfo = jsonRes.dump();
    int32_t scId;

    EXPECT_EQ(SC_OK, SecCompKit::RegisterSecurityComponent(SAVE_COMPONENT, saveInfo, scId));
    uint8_t data[TestCommon::MAX_HMAC_SIZE] = { 0 };
    struct SecCompClickEvent touchInfo = {
        .touchX = TestCommon::TEST_COORDINATE,
        .touchY = TestCommon::TEST_COORDINATE,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TestCommon::TIME_CONVERSION_UNIT
    };
    touchInfo.extraInfo.dataSize = TestCommon::MAX_HMAC_SIZE;
    touchInfo.extraInfo.data = data;

    ASSERT_EQ(SC_OK, SecCompKit::ReportSecurityComponentClickEvent(scId, saveInfo, touchInfo, nullptr));
    uint32_t selfTokenId = GetSelfTokenID();
    ASSERT_TRUE(SecCompKit::ReduceAfterVerifySavePermission(selfTokenId));
    system("param set sec.comp.enhance 0");
}

/**
 * @tc.name: ExceptCall001
 * @tc.desc: Test update security component success
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompRegisterCallbackTest, ExceptCall001, TestSize.Level1)
{
    nlohmann::json jsonRes;
    TestCommon::BuildSaveComponentInfo(jsonRes);
    std::string saveInfo = jsonRes.dump();
    int32_t scId;
    ASSERT_EQ(SC_OK, SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, saveInfo, scId));
    ASSERT_NE(-1, scId);
    ASSERT_EQ(SC_OK, SecCompKit::UpdateSecurityComponent(scId, saveInfo));
}

/**
 * @tc.name: ReportClickWithoutHmac001
 * @tc.desc: Test report click event permission denied
 * @tc.type: FUNC
 * @tc.require: AR000HO9JM
 */
HWTEST_F(SecCompRegisterCallbackTest, ReportClickWithoutHmac001, TestSize.Level1)
{
    system("param set sec.comp.enhance 1");
    nlohmann::json jsonRes;
    TestCommon::BuildLocationComponentInfo(jsonRes);
    std::string locationInfo = jsonRes.dump();
    g_probe.mockComponentInfo = locationInfo;
    g_probe.mockRes = 0;

    int32_t scId;
    ASSERT_EQ(SC_OK, SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, locationInfo, scId));
    ASSERT_NE(-1, scId);
    uint8_t data[TestCommon::MAX_HMAC_SIZE] = { 0 };
    struct SecCompClickEvent touch = {
        .touchX = TestCommon::TEST_COORDINATE,
        .touchY = TestCommon::TEST_COORDINATE,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TestCommon::TIME_CONVERSION_UNIT
    };
    touch.extraInfo.dataSize = TestCommon::MAX_HMAC_SIZE;
    touch.extraInfo.data = data;
    EXPECT_EQ(SC_SERVICE_ERROR_PERMISSION_OPER_FAIL,
        SecCompKit::ReportSecurityComponentClickEvent(scId, locationInfo, touch, nullptr));
    system("param set sec.comp.enhance 0");
}
