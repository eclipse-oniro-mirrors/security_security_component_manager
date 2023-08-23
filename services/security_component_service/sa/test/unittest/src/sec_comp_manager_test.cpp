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

#include "sec_comp_manager_test.h"

#include "sec_comp_log.h"
#include "location_button.h"
#include "save_button.h"
#include "sec_comp_err.h"
#include "service_test_common.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;
using namespace OHOS::Security::AccessToken;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompManagerTest"};

static LocationButton BuildInvalidLocationComponent()
{
    LocationButton button;
    button.fontSize_ = ServiceTestCommon::TEST_INVALID_SIZE;
    button.iconSize_ = ServiceTestCommon::TEST_INVALID_SIZE;
    button.padding_.top = ServiceTestCommon::TEST_INVALID_SIZE;
    button.padding_.right = ServiceTestCommon::TEST_INVALID_SIZE;
    button.padding_.bottom = ServiceTestCommon::TEST_INVALID_SIZE;
    button.padding_.left = ServiceTestCommon::TEST_INVALID_SIZE;
    button.textIconSpace_ = ServiceTestCommon::TEST_INVALID_SIZE;
    button.fontColor_.value = ServiceTestCommon::TEST_COLOR_WHITE;
    button.iconColor_.value = ServiceTestCommon::TEST_COLOR_WHITE;
    button.bgColor_.value = ServiceTestCommon::TEST_COLOR_WHITE;
    button.borderWidth_ = ServiceTestCommon::TEST_INVALID_SIZE;
    button.type_ = LOCATION_COMPONENT;
    button.rect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    button.rect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    button.rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    button.rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    button.windowRect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    button.windowRect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    button.windowRect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    button.windowRect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    button.text_ = UNKNOWN_TEXT;
    button.icon_ = UNKNOWN_ICON;
    button.bg_ = SecCompBackground::UNKNOWN_BG;
    return button;
}

static LocationButton BuildValidLocationComponent()
{
    LocationButton button;
    button.fontSize_ = ServiceTestCommon::TEST_SIZE;
    button.iconSize_ = ServiceTestCommon::TEST_SIZE;
    button.padding_.top = ServiceTestCommon::TEST_SIZE;
    button.padding_.right = ServiceTestCommon::TEST_SIZE;
    button.padding_.bottom = ServiceTestCommon::TEST_SIZE;
    button.padding_.left = ServiceTestCommon::TEST_SIZE;
    button.textIconSpace_ = ServiceTestCommon::TEST_SIZE;
    button.fontColor_.value = ServiceTestCommon::TEST_COLOR_YELLOW;
    button.iconColor_.value = ServiceTestCommon::TEST_COLOR_RED;
    button.bgColor_.value = ServiceTestCommon::TEST_COLOR_BLUE;
    button.borderWidth_ = ServiceTestCommon::TEST_SIZE;
    button.type_ = LOCATION_COMPONENT;
    button.rect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    button.rect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    button.rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    button.rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    button.windowRect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    button.windowRect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    button.windowRect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    button.windowRect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    button.text_ = static_cast<int32_t>(LocationDesc::SELECT_LOCATION);
    button.icon_ = static_cast<int32_t>(LocationIcon::LINE_ICON);
    button.bg_ = SecCompBackground::CIRCLE;
    return button;
}
}

void SecCompManagerTest::SetUpTestCase()
{}

void SecCompManagerTest::TearDownTestCase()
{}

void SecCompManagerTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
}

void SecCompManagerTest::TearDown()
{
    SecCompManager::GetInstance().componentMap_.clear();
}


/**
 * @tc.name: CreateScId001
 * @tc.desc: Test create sc id
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, CreateScId001, TestSize.Level1)
{
    SecCompManager::GetInstance().scIdStart_ = ServiceTestCommon::MAX_INT_NUM;
    ASSERT_EQ(ServiceTestCommon::SC_ID_START, SecCompManager::GetInstance().CreateScId());

    SecCompManager::GetInstance().scIdStart_ = ServiceTestCommon::SC_ID_START;
    ASSERT_EQ(ServiceTestCommon::SC_ID_START + 1, SecCompManager::GetInstance().CreateScId());
}

/**
 * @tc.name: AddSecurityComponentToList001
 * @tc.desc: Test add security component to list
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, AddSecurityComponentToList001, TestSize.Level1)
{
    std::shared_ptr<LocationButton> compPtr = std::make_shared<LocationButton>();
    ASSERT_NE(nullptr, compPtr);
    compPtr->rect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    SecCompEntity entity(compPtr, ServiceTestCommon::TEST_TOKEN_ID, ServiceTestCommon::TEST_SC_ID_1);

    ASSERT_EQ(SC_OK, SecCompManager::GetInstance().AddSecurityComponentToList(1, entity));

    std::shared_ptr<LocationButton> compPtrNew = std::make_shared<LocationButton>();
    ASSERT_NE(nullptr, compPtrNew);
    compPtrNew->rect_.x_ = ServiceTestCommon::TEST_COORDINATE * 2; // not overlap
    compPtrNew->rect_.y_ = ServiceTestCommon::TEST_COORDINATE * 2; // not overlap
    compPtrNew->rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    compPtrNew->rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    SecCompEntity entityNew(compPtrNew, ServiceTestCommon::TEST_TOKEN_ID, ServiceTestCommon::TEST_SC_ID_2);
    ASSERT_EQ(SC_OK, SecCompManager::GetInstance().AddSecurityComponentToList(1, entityNew));
}

/**
 * @tc.name: DeleteSecurityComponentFromList001
 * @tc.desc: Test delete security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, DeleteSecurityComponentFromList001, TestSize.Level1)
{
    ASSERT_EQ(SC_SERVICE_ERROR_COMPONENT_NOT_EXIST, SecCompManager::GetInstance().DeleteSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_1));

    std::shared_ptr<LocationButton> compPtr = std::make_shared<LocationButton>();
    ASSERT_NE(nullptr, compPtr);
    compPtr->rect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    SecCompEntity entity(compPtr, ServiceTestCommon::TEST_TOKEN_ID, ServiceTestCommon::TEST_SC_ID_1);
    ASSERT_EQ(SC_OK, SecCompManager::GetInstance().AddSecurityComponentToList(ServiceTestCommon::TEST_PID_1, entity));

    ASSERT_EQ(SC_SERVICE_ERROR_COMPONENT_NOT_EXIST, SecCompManager::GetInstance().DeleteSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_2));

    ASSERT_EQ(SC_OK, SecCompManager::GetInstance().DeleteSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_1));
}

/**
 * @tc.name: GetSecurityComponentFromList001
 * @tc.desc: Test get security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, GetSecurityComponentFromList001, TestSize.Level1)
{
    ASSERT_EQ(nullptr, SecCompManager::GetInstance().GetSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_1));

    std::shared_ptr<LocationButton> compPtr = std::make_shared<LocationButton>();
    ASSERT_NE(nullptr, compPtr);
    compPtr->rect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    SecCompEntity entity(compPtr, ServiceTestCommon::TEST_TOKEN_ID, ServiceTestCommon::TEST_SC_ID_1);
    ASSERT_EQ(SC_OK, SecCompManager::GetInstance().AddSecurityComponentToList(ServiceTestCommon::TEST_PID_1, entity));
    ASSERT_EQ(nullptr, SecCompManager::GetInstance().GetSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_2));
    ASSERT_NE(nullptr, SecCompManager::GetInstance().GetSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_1));
}

/**
 * @tc.name: NotifyProcessBackground001
 * @tc.desc: Test notify process background
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, NotifyProcessBackground001, TestSize.Level1)
{
    SecCompManager::GetInstance().NotifyProcessBackground(ServiceTestCommon::TEST_PID_1);

    std::shared_ptr<LocationButton> compPtr = std::make_shared<LocationButton>();
    ASSERT_NE(nullptr, compPtr);
    compPtr->rect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    SecCompEntity entity(compPtr, ServiceTestCommon::TEST_TOKEN_ID, ServiceTestCommon::TEST_SC_ID_1);
    ASSERT_EQ(SC_OK, SecCompManager::GetInstance().AddSecurityComponentToList(ServiceTestCommon::TEST_PID_1, entity));
    auto component = SecCompManager::GetInstance().GetSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_1);
    ASSERT_NE(nullptr, component);
    SecCompManager::GetInstance().NotifyProcessForeground(ServiceTestCommon::TEST_PID_1);
    ASSERT_TRUE(SecCompManager::GetInstance().IsForegroundCompExist());

    SecCompManager::GetInstance().NotifyProcessBackground(ServiceTestCommon::TEST_PID_1);
    ASSERT_FALSE(SecCompManager::GetInstance().IsForegroundCompExist());
}

/**
 * @tc.name: NotifyProcessDied001
 * @tc.desc: Test notify process died
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, NotifyProcessDied001, TestSize.Level1)
{
    std::shared_ptr<LocationButton> compPtr = std::make_shared<LocationButton>();
    ASSERT_NE(nullptr, compPtr);
    compPtr->rect_.x_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.y_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr->rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    SecCompEntity entity(compPtr, ServiceTestCommon::TEST_TOKEN_ID, ServiceTestCommon::TEST_SC_ID_1);
    ASSERT_EQ(SC_OK, SecCompManager::GetInstance().AddSecurityComponentToList(ServiceTestCommon::TEST_PID_1, entity));

    std::shared_ptr<LocationButton> compPtr2 = std::make_shared<LocationButton>();
    ASSERT_NE(nullptr, compPtr2);
    compPtr2->rect_.x_ = ServiceTestCommon::TEST_COORDINATE * 2; // not overlap
    compPtr2->rect_.y_ = ServiceTestCommon::TEST_COORDINATE * 2; // not overlap
    compPtr2->rect_.width_ = ServiceTestCommon::TEST_COORDINATE;
    compPtr2->rect_.height_ = ServiceTestCommon::TEST_COORDINATE;
    SecCompEntity entity2(compPtr2, ServiceTestCommon::TEST_TOKEN_ID, ServiceTestCommon::TEST_SC_ID_2);
    ASSERT_EQ(SC_OK, SecCompManager::GetInstance().AddSecurityComponentToList(ServiceTestCommon::TEST_PID_2, entity2));

    SecCompManager::GetInstance().NotifyProcessDied(ServiceTestCommon::TEST_PID_3);
    ASSERT_NE(nullptr, SecCompManager::GetInstance().GetSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_1));

    SecCompManager::GetInstance().NotifyProcessDied(ServiceTestCommon::TEST_PID_1);
    ASSERT_EQ(nullptr, SecCompManager::GetInstance().GetSecurityComponentFromList(
        ServiceTestCommon::TEST_PID_1, ServiceTestCommon::TEST_SC_ID_1));
}

/**
 * @tc.name: RegisterSecurityComponent001
 * @tc.desc: Test register security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, RegisterSecurityComponent001, TestSize.Level1)
{
    nlohmann::json jsonInvalid;
    LocationButton buttonInvalid = BuildInvalidLocationComponent();
    buttonInvalid.ToJson(jsonInvalid);
    int32_t scId;
    SecCompCallerInfo caller = {
        .tokenId = ServiceTestCommon::TEST_TOKEN_ID,
        .pid = ServiceTestCommon::TEST_PID_1
    };
    ASSERT_EQ(SC_SERVICE_ERROR_COMPONENT_INFO_INVALID,
        SecCompManager::GetInstance().RegisterSecurityComponent(LOCATION_COMPONENT, jsonInvalid, caller, scId));

    nlohmann::json jsonValid;
    LocationButton buttonValid = BuildValidLocationComponent();
    buttonValid.ToJson(jsonValid);

    // callback check failed
    ASSERT_EQ(SC_ENHANCE_ERROR_CALLBACK_NOT_EXIST,
        SecCompManager::GetInstance().RegisterSecurityComponent(LOCATION_COMPONENT, jsonValid, caller, scId));
    SecCompManager::GetInstance().maliciousAppList_.clear();
}

/**
 * @tc.name: UpdateSecurityComponent001
 * @tc.desc: Test update security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, UpdateSecurityComponent001, TestSize.Level1)
{
    nlohmann::json jsonValid;
    LocationButton buttonValid = BuildValidLocationComponent();
    buttonValid.ToJson(jsonValid);
    SecCompCallerInfo caller = {
        .tokenId = ServiceTestCommon::TEST_TOKEN_ID,
        .pid = ServiceTestCommon::TEST_PID_1
    };
    ASSERT_NE(SC_OK, SecCompManager::GetInstance().UpdateSecurityComponent(
        ServiceTestCommon::TEST_SC_ID_1, jsonValid, caller));
}

/**
 * @tc.name: UnregisterSecurityComponent001
 * @tc.desc: Test unregister security component
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, UnregisterSecurityComponent001, TestSize.Level1)
{
    SecCompCallerInfo caller = {
        .tokenId = ServiceTestCommon::TEST_TOKEN_ID,
        .pid = ServiceTestCommon::TEST_PID_1
    };

    ASSERT_EQ(SC_SERVICE_ERROR_VALUE_INVALID,
        SecCompManager::GetInstance().UnregisterSecurityComponent(ServiceTestCommon::TEST_INVALID_SC_ID, caller));
}

/**
 * @tc.name: ReportSecurityComponentClickEvent001
 * @tc.desc: Test report security component click
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompManagerTest, ReportSecurityComponentClickEvent001, TestSize.Level1)
{
    SecCompCallerInfo caller = {
        .tokenId = ServiceTestCommon::TEST_TOKEN_ID,
        .pid = ServiceTestCommon::TEST_PID_1
    };
    SecCompClickEvent touchInfo;
    nlohmann::json jsonVaild;
    LocationButton buttonValid = BuildValidLocationComponent();
    buttonValid.ToJson(jsonVaild);
    ASSERT_NE(SC_OK,
        SecCompManager::GetInstance().ReportSecurityComponentClickEvent(1, jsonVaild, caller, touchInfo, nullptr));
}
