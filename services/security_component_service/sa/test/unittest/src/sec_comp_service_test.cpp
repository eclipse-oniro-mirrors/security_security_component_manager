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
static constexpr int32_t TEST_SC_ID = 1;
static AccessTokenID g_selfTokenId = 0;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr double TEST_SIZE = 100.0;
static constexpr uint32_t TEST_COLOR_YELLOW = 0x7fff00;
static constexpr uint32_t TEST_COLOR_RED = 0xff0000;
static constexpr uint32_t TEST_COLOR_BLUE = 0x0000ff;
static constexpr uint64_t TIME_CONVERSION_UNIT = 1000;

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
 * @tc.name: Onstart001
 * @tc.desc: Test OnStart
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, OnStart001, TestSize.Level1)
{
    secCompService_->state_ = ServiceRunningState::STATE_RUNNING;
    secCompService_->appStateObserver_ = nullptr;
    secCompService_->OnStart();
    ASSERT_EQ(secCompService_->appStateObserver_, nullptr);
    EXPECT_CALL(*secCompService_, Publish(testing::_)).WillOnce(testing::Return(false));

    secCompService_->state_ = ServiceRunningState::STATE_NOT_START;
    secCompService_->appStateObserver_ = new (std::nothrow) AppStateObserver();
    secCompService_->OnStart();
    ASSERT_EQ(secCompService_->state_, ServiceRunningState::STATE_RUNNING);

    secCompService_->OnStop();
    ASSERT_EQ(secCompService_->appStateObserver_, nullptr);
}

/**
 * @tc.name: RegisterAppStateObserver001
 * @tc.desc: Test RegisterAppStateObserver
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, RegisterAppStateObserver001, TestSize.Level1)
{
    // GetSystemAbilityManager get failed
    secCompService_->appStateObserver_ = nullptr;
    std::shared_ptr<SystemAbilityManagerClient> saClient = std::make_shared<SystemAbilityManagerClient>();
    ASSERT_NE(saClient, nullptr);
    SystemAbilityManagerClient::clientInstance = saClient.get();
    EXPECT_CALL(*saClient, GetSystemAbilityManager()).WillOnce(testing::Return(nullptr));
    EXPECT_FALSE(secCompService_->RegisterAppStateObserver());

    // GetSystemAbility get app mgr failed
    secCompService_->appStateObserver_ = nullptr;
    SystemAbilityManagerProxy proxy(nullptr);
    EXPECT_CALL(*saClient, GetSystemAbilityManager()).WillOnce(testing::Return(&proxy));
    EXPECT_FALSE(secCompService_->RegisterAppStateObserver());

    // RegisterApplicationStateObserver failed
    secCompService_->appStateObserver_ = nullptr;
    EXPECT_CALL(*saClient, GetSystemAbilityManager()).WillOnce(testing::Return(&proxy));
    MockIRemoteObject object;
    EXPECT_CALL(proxy, GetSystemAbility(testing::_)).WillOnce(testing::Return(&object));
    sptr<MockAppMgrProxy> appProxy = new (std::nothrow) MockAppMgrProxy(nullptr);
    MockAppMgrProxy::g_MockAppMgrProxy = appProxy;
    EXPECT_CALL(*MockAppMgrProxy::g_MockAppMgrProxy,
        RegisterApplicationStateObserver(testing::_, testing::_)).WillOnce(testing::Return(-1));
    EXPECT_FALSE(secCompService_->RegisterAppStateObserver());

    // GetForegroundApplications failed
    secCompService_->appStateObserver_ = nullptr;
    EXPECT_CALL(*saClient, GetSystemAbilityManager()).WillOnce(testing::Return(&proxy));
    EXPECT_CALL(proxy, GetSystemAbility(testing::_)).WillOnce(testing::Return(&object));
    EXPECT_CALL(*MockAppMgrProxy::g_MockAppMgrProxy,
        RegisterApplicationStateObserver(testing::_, testing::_)).WillOnce(testing::Return(0));
    EXPECT_CALL(*MockAppMgrProxy::g_MockAppMgrProxy,
        GetForegroundApplications(testing::_)).WillOnce(testing::Return(-1));
    EXPECT_TRUE(secCompService_->RegisterAppStateObserver());
    EXPECT_EQ(secCompService_->appStateObserver_->foregrandProcList_.size(), static_cast<const size_t>(0));

    // get one foreground app
    secCompService_->appStateObserver_ = nullptr;
    EXPECT_CALL(*saClient, GetSystemAbilityManager()).WillOnce(testing::Return(&proxy));
    EXPECT_CALL(proxy, GetSystemAbility(testing::_)).WillOnce(testing::Return(&object));
    EXPECT_CALL(*MockAppMgrProxy::g_MockAppMgrProxy,
        RegisterApplicationStateObserver(testing::_, testing::_)).WillOnce(testing::Return(0));
    EXPECT_CALL(*MockAppMgrProxy::g_MockAppMgrProxy, GetForegroundApplications(testing::_))
        .WillOnce([](std::vector<AppExecFwk::AppStateData>& list) {
            AppExecFwk::AppStateData data;
            data.uid = 1000;
            list.emplace_back(data);
            return 0;
        });
    EXPECT_TRUE(secCompService_->RegisterAppStateObserver());
    EXPECT_EQ(secCompService_->appStateObserver_->foregrandProcList_.size(), static_cast<const size_t>(1));
    secCompService_->UnregisterAppStateObserver();
    SystemAbilityManagerClient::clientInstance = nullptr;
}

/**
 * @tc.name: UnregisterAppStateObserver001
 * @tc.desc: Test RegisterAppStateObserver
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, UnregisterAppStateObserver001, TestSize.Level1)
{
    // GetSystemAbilityManager get failed
    secCompService_->appStateObserver_ = nullptr;
    sptr<MockAppMgrProxy> appProxy = new (std::nothrow) MockAppMgrProxy(nullptr);
    secCompService_->iAppMgr_ = appProxy;

    EXPECT_CALL(*appProxy, UnregisterApplicationStateObserver(testing::_)).Times(testing::Exactly(0));
    secCompService_->UnregisterAppStateObserver();
}

/**
 * @tc.name: GetCallerInfo001
 * @tc.desc: Test get caller info
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, GetCallerInfo001, TestSize.Level1)
{
    // not root uid
    setuid(1);
    SecCompCallerInfo caller;
    EXPECT_FALSE(secCompService_->GetCallerInfo(caller));

    // set token id to hap token, but uid is not in foreground
    EXPECT_FALSE(secCompService_->GetCallerInfo(caller));
    setuid(0);
    ASSERT_EQ(SetSelfTokenID(HAP_TOKEN_ID), 0);
    // add local uid to foreground.
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);
    EXPECT_TRUE(secCompService_->GetCallerInfo(caller));
}

/**
 * @tc.name: UnregisterSecurityComponent001
 * @tc.desc: Test unregister security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, UnregisterSecurityComponent001, TestSize.Level1)
{
    // get caller fail
    EXPECT_EQ(secCompService_->UnregisterSecurityComponent(TEST_SC_ID), SC_SERVICE_ERROR_COMPONENT_NOT_EXIST);
}

/**
 * @tc.name: UpdateSecurityComponent001
 * @tc.desc: Test update security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, UpdateSecurityComponent001, TestSize.Level1)
{
    // get caller fail
    EXPECT_EQ(secCompService_->UpdateSecurityComponent(TEST_SC_ID, ""), SC_SERVICE_ERROR_VALUE_INVALID);

    ASSERT_EQ(SetSelfTokenID(HAP_TOKEN_ID), 0);
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);
    EXPECT_EQ(secCompService_->UpdateSecurityComponent(TEST_SC_ID, "{a"), SC_SERVICE_ERROR_VALUE_INVALID);
}

/**
 * @tc.name: ReportSecurityComponentClickEvent001
 * @tc.desc: Test report security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompServiceTest, ReportSecurityComponentClickEvent001, TestSize.Level1)
{
    auto uid = getuid();
    // get caller fail
    int32_t scId;
    EXPECT_EQ(secCompService_->RegisterSecurityComponent(LOCATION_COMPONENT, "", scId),
        SC_SERVICE_ERROR_VALUE_INVALID);

    // parse component json fail
    ASSERT_EQ(SetSelfTokenID(HAP_TOKEN_ID), 0);
    setuid(100);
    AppExecFwk::AppStateData stateData = {
        .uid = getuid()
    };
    secCompService_->appStateObserver_->AddProcessToForegroundSet(stateData);

    EXPECT_EQ(secCompService_->RegisterSecurityComponent(LOCATION_COMPONENT, BuildLocationComponentInfo(), scId),
        SC_ENHANCE_ERROR_CALLBACK_NOT_EXIST);
    uint8_t data[16] = { 0 };
    struct SecCompClickEvent touch = {
        .touchX = 100,
        .touchY = 100,
        .timestamp = static_cast<uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()) / TIME_CONVERSION_UNIT,
        .extraInfo.data = data,
        .extraInfo.dataSize = 16,
    };
    EXPECT_EQ(secCompService_->ReportSecurityComponentClickEvent(scId, BuildLocationComponentInfo(), touch),
        SC_ENHANCE_ERROR_IN_MALICIOUS_LIST);
    EXPECT_EQ(secCompService_->UnregisterSecurityComponent(scId), SC_SERVICE_ERROR_COMPONENT_NOT_EXIST);
    setuid(uid);
}
