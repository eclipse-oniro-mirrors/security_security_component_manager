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
#include "sec_comp_service_test.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "location_button.h"
#include "mock_system_ability_proxy.h"
#include "mock_app_mgr_proxy.h"
#include "paste_button.h"
#include "save_button.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"
#include "sec_comp_tool.h"
#include "sec_comp_enhance_adapter.h"
#include "system_ability.h"
#include "token_setproc.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;
using namespace OHOS::Security::AccessToken;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompServiceTest"};

static constexpr uint32_t HAP_TOKEN_ID = 537715419;
static constexpr int32_t SA_ID = 3506;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr double TEST_SIZE = 100.0;
static constexpr uint32_t TEST_COLOR_YELLOW = 0x7fff00;
static constexpr uint32_t TEST_COLOR_RED = 0xff0000;
static constexpr uint32_t TEST_COLOR_BLUE = 0x0000ff;
static constexpr uint64_t TIME_CONVERSION_UNIT = 1000;
static AccessTokenID g_selfTokenId = 0;

static std::string BuildLocationComponentInfo()
{
    LocationButton button;
    button.fontSize_ = TEST_SIZE;
    button.iconSize_ = TEST_SIZE;
    button.padding_.top = TEST_SIZE;
    button.padding_.right = TEST_SIZE;
    button.padding_.bottom = TEST_SIZE;
    button.padding_.left = TEST_SIZE;
    button.textIconSpace_ = TEST_SIZE;
    button.fontColor_.value = TEST_COLOR_YELLOW;
    button.iconColor_.value = TEST_COLOR_RED;
    button.bgColor_.value = TEST_COLOR_BLUE;
    button.borderWidth_ = TEST_SIZE;
    button.type_ = LOCATION_COMPONENT;
    button.rect_.x_ = TEST_COORDINATE;
    button.rect_.y_ = TEST_COORDINATE;
    button.rect_.width_ = TEST_COORDINATE;
    button.rect_.height_ = TEST_COORDINATE;
    button.windowRect_.x_ = TEST_COORDINATE;
    button.windowRect_.y_ = TEST_COORDINATE;
    button.windowRect_.width_ = TEST_COORDINATE;
    button.windowRect_.height_ = TEST_COORDINATE;
    button.text_ = static_cast<int32_t>(LocationDesc::SELECT_LOCATION);
    button.icon_ = static_cast<int32_t>(LocationIcon::LINE_ICON);
    button.bg_ = SecCompBackground::CIRCLE;

    nlohmann::json jsonRes;
    button.ToJson(jsonRes);
    return jsonRes.dump();
}

static std::string BuildSaveComponentInfo()
{
    SaveButton button;
    button.fontSize_ = TEST_SIZE;
    button.iconSize_ = TEST_SIZE;
    button.padding_.top = TEST_SIZE;
    button.padding_.right = TEST_SIZE;
    button.padding_.bottom = TEST_SIZE;
    button.padding_.left = TEST_SIZE;
    button.textIconSpace_ = TEST_SIZE;
    button.fontColor_.value = TEST_COLOR_YELLOW;
    button.iconColor_.value = TEST_COLOR_RED;
    button.bgColor_.value = TEST_COLOR_BLUE;
    button.borderWidth_ = TEST_SIZE;
    button.type_ = SAVE_COMPONENT;
    button.rect_.x_ = TEST_COORDINATE;
    button.rect_.y_ = TEST_COORDINATE;
    button.rect_.width_ = TEST_COORDINATE;
    button.rect_.height_ = TEST_COORDINATE;
    button.windowRect_.x_ = TEST_COORDINATE;
    button.windowRect_.y_ = TEST_COORDINATE;
    button.windowRect_.width_ = TEST_COORDINATE;
    button.windowRect_.height_ = TEST_COORDINATE;
    button.text_ = static_cast<int32_t>(SaveDesc::DOWNLOAD);
    button.icon_ = static_cast<int32_t>(SaveIcon::LINE_ICON);
    button.bg_ = SecCompBackground::CIRCLE;

    nlohmann::json jsonRes;
    button.ToJson(jsonRes);
    return jsonRes.dump();
}
}

void SecCompServiceTest::SetUpTestCase()
{}

void SecCompServiceTest::TearDownTestCase()
{}

void SecCompServiceTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
    if (secCompService_ != nullptr) {
        return;
    }
    SecCompService* ptr = new (std::nothrow) SecCompService(SA_ID, true);
    secCompService_ = sptr<SecCompService>(ptr);
    ASSERT_NE(nullptr, secCompService_);
    secCompService_->appStateObserver_ = new (std::nothrow) AppStateObserver();
    ASSERT_TRUE(secCompService_->appStateObserver_ != nullptr);
    g_selfTokenId = GetSelfTokenID();
}

void SecCompServiceTest::TearDown()
{
    if (secCompService_ != nullptr) {
        secCompService_->appStateObserver_ = nullptr;
    }
    secCompService_ = nullptr;
    EXPECT_EQ(0, SetSelfTokenID(g_selfTokenId));
}

/**
 * @tc.name: RegisterSecurityComponent001
 * @tc.desc: Test register security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, RegisterSecurityComponent001, TestSize.Level1)
{
    // get caller fail
    int32_t scId;
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(LOCATION_COMPONENT, "", scId),
        SC_SERVICE_ERROR_VALUE_INVALID);

    // parse component json fail
    ASSERT_EQ(SetSelfTokenID(HAP_TOKEN_ID), 0);
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(LOCATION_COMPONENT, "{a=", scId),
        SC_SERVICE_ERROR_VALUE_INVALID); // wrong json

    // register security component ok
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(LOCATION_COMPONENT, BuildLocationComponentInfo(), scId),
        SC_OK);
    EXPECT_EQ(secCompService_->UpdateSecurityComponent(scId, BuildLocationComponentInfo()),
        SC_OK);
    struct SecCompClickEvent touch = {
        .touchX = 100,
        .touchY = 100,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT
    };

    EXPECT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildLocationComponentInfo(), touch),
        SC_OK);
    EXPECT_EQ(secCompService_->UnregisterSecurityComponent(scId), SC_OK);
}

/**
 * @tc.name: RegisterSecurityComponent002
 * @tc.desc: Test register security component check touch info failed
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, RegisterSecurityComponent002, TestSize.Level1)
{
    int32_t scId;
    secCompService_->state_ = ServiceRunningState::STATE_RUNNING;
    secCompService_->Initialize();

    ASSERT_EQ(SetSelfTokenID(HAP_TOKEN_ID), 0);
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);
    // register security component ok
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(SAVE_COMPONENT, BuildSaveComponentInfo(), scId),
        SC_OK);
    struct SecCompClickEvent touch = {
        .touchX = 100,
        .touchY = 100,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count())
    };
    EXPECT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildLocationComponentInfo(), touch),
        SC_SERVICE_ERROR_CLICK_EVENT_INVALID);
    EXPECT_EQ(secCompService_->UnregisterSecurityComponent(scId), SC_OK);
}

/**
 * @tc.name: RegisterSecurityComponent003
 * @tc.desc: Test register security component permission grant failed
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, RegisterSecurityComponent003, TestSize.Level1)
{
    int32_t scId;
    secCompService_->state_ = ServiceRunningState::STATE_RUNNING;
    secCompService_->Initialize();

    ASSERT_EQ(SetSelfTokenID(0), 0);
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);
    // register security component ok
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(SAVE_COMPONENT, BuildSaveComponentInfo(), scId),
        SC_OK);
    struct SecCompClickEvent touch = {
        .touchX = 100,
        .touchY = 100,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT
    };
    EXPECT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildLocationComponentInfo(), touch),
        SC_SERVICE_ERROR_PERMISSION_OPER_FAIL);
    EXPECT_EQ(secCompService_->UnregisterSecurityComponent(scId), SC_OK);
}

/**
 * @tc.name: ReportSecurityComponentClickEvent001
 * @tc.desc: Test register security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, ReportSecurityComponentClickEvent001, TestSize.Level1)
{
    int32_t scId;
    secCompService_->state_ = ServiceRunningState::STATE_RUNNING;
    secCompService_->Initialize();

    ASSERT_EQ(SetSelfTokenID(HAP_TOKEN_ID), 0);
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);
    // register security component ok
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(SAVE_COMPONENT, BuildSaveComponentInfo(), scId),
        SC_OK);
    struct SecCompClickEvent touchInfo = {
        .touchX = 100,
        .touchY = 100,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT
    };

    ASSERT_EQ(SC_OK, secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo));

    ASSERT_TRUE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));

    ASSERT_FALSE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));

    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    sleep(6);
    ASSERT_FALSE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
    EXPECT_EQ(secCompService_->UnregisterSecurityComponent(scId), SC_OK);
}

/**
 * @tc.name: ReportSecurityComponentClickEvent002
 * @tc.desc: Test report security component click with save button
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompServiceTest, ReportSecurityComponentClickEvent002, TestSize.Level1)
{
    int32_t scId;
    secCompService_->state_ = ServiceRunningState::STATE_RUNNING;
    secCompService_->Initialize();

    ASSERT_EQ(SetSelfTokenID(HAP_TOKEN_ID), 0);
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);
    // register security component ok
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(SAVE_COMPONENT, BuildSaveComponentInfo(), scId),
        SC_OK);
    struct SecCompClickEvent touchInfo = {
        .touchX = 100,
        .touchY = 100,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT
    };

    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);

    ASSERT_TRUE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
    ASSERT_TRUE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
    ASSERT_FALSE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));

    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    sleep(3);
    touchInfo.timestamp = static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT;
    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    sleep(3);
    ASSERT_TRUE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
    ASSERT_FALSE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));

    touchInfo.timestamp = static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT;
    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    sleep(3);
    touchInfo.timestamp = static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT;;
    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    ASSERT_TRUE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
    sleep(3);
    ASSERT_TRUE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
    ASSERT_FALSE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
}

/**
 * @tc.name: ReportSecurityComponentClickEvent003
 * @tc.desc: Test report security component click twice with save button
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, ReportSecurityComponentClickEvent003, TestSize.Level1)
{
    int32_t scId;
    secCompService_->state_ = ServiceRunningState::STATE_RUNNING;
    secCompService_->Initialize();

    ASSERT_EQ(SetSelfTokenID(HAP_TOKEN_ID), 0);
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);
    // register security component ok
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(SAVE_COMPONENT, BuildSaveComponentInfo(), scId),
        SC_OK);
    struct SecCompClickEvent touchInfo = {
        .touchX = 100,
        .touchY = 100,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT
    };

    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    sleep(3);
    touchInfo.timestamp = static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT;
    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    sleep(6);
    ASSERT_FALSE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));

    touchInfo.timestamp = static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT;
    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    ASSERT_TRUE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
    ASSERT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildSaveComponentInfo(), touchInfo),
        SC_OK);
    sleep(3);
    ASSERT_TRUE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
    ASSERT_FALSE(secCompService_->ReduceAfterVerifySavePermission(HAP_TOKEN_ID));
}
