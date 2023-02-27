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
#include "sec_comp_err.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;
using namespace OHOS::Security::AccessToken;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompManagerTest"};

static constexpr uint32_t TEST_FONT_SIZE = 100;
static constexpr uint32_t TEST_DIFF_FONT_SIZE = 16;
static constexpr uint32_t TEST_INVALID_FONT_SIZE = 0;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr uint32_t TEST_COLOR = 0xffffffff;

static constexpr int32_t TEST_UID_1 = 1;
static constexpr int32_t TEST_UID_2 = 2;
static constexpr int32_t TEST_UID_3 = 3;

static constexpr AccessTokenID TEST_TOKEN_ID_1 = 1;
static constexpr AccessTokenID TEST_TOKEN_ID_2 = 2;
static constexpr int32_t TEST_SC_ID_1 = 1;
static constexpr int32_t TEST_SC_ID_2 = 2;
static constexpr int32_t TEST_INVALID_SC_ID = -1;

static constexpr int32_t MAX_INT_NUM = 0x7fffffff;
static constexpr int32_t SC_ID_START = 1000;

static LocationButton BuildInvalidLocationComponent()
{
    LocationButton button;
    button.fontSize_ = TEST_INVALID_FONT_SIZE;
    button.font_ = FONT_HO_SANS;
    button.bgColor_.value = TEST_COLOR;
    button.fontColor_.value = TEST_COLOR;
    button.icon_ = DEFAULT_ICON;
    button.label_ = DEFAULT_LABEL;
    button.rect_.x_ = TEST_COORDINATE;
    button.rect_.y_ = TEST_COORDINATE;
    button.rect_.width_ = TEST_COORDINATE;
    button.rect_.height_ = TEST_COORDINATE;
    button.type_ = LOCATION_COMPONENT;
    button.buttonType_ = CAPSULE;
    return button;
}

static LocationButton BuildValidLocationComponent()
{
    LocationButton button;
    button.fontSize_ = TEST_FONT_SIZE;
    button.font_ = FONT_HO_SANS;
    button.bgColor_.value = TEST_COLOR;
    button.fontColor_.value = TEST_COLOR;
    button.icon_ = DEFAULT_ICON;
    button.label_ = DEFAULT_LABEL;
    button.rect_.x_ = TEST_COORDINATE;
    button.rect_.y_ = TEST_COORDINATE;
    button.rect_.width_ = TEST_COORDINATE;
    button.rect_.height_ = TEST_COORDINATE;
    button.type_ = LOCATION_COMPONENT;
    button.buttonType_ = CAPSULE;
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
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, CreateScId001, TestSize.Level1)
{
    SecCompManager::GetInstance().scIdStart_ = MAX_INT_NUM;
    ASSERT_EQ(SecCompManager::GetInstance().CreateScId(), SC_ID_START);

    SecCompManager::GetInstance().scIdStart_ = SC_ID_START;
    ASSERT_EQ(SecCompManager::GetInstance().CreateScId(), SC_ID_START + 1);
}

/**
 * @tc.name: AddProcessComponent001
 * @tc.desc: Test add process component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, AddProcessComponent001, TestSize.Level1)
{
    std::shared_ptr<SecCompBase> compPtr = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtr, nullptr);
    compPtr->rect_.x_ = TEST_COORDINATE;
    compPtr->rect_.y_ = TEST_COORDINATE;
    compPtr->rect_.width_ = TEST_COORDINATE;
    compPtr->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entity(compPtr, TEST_TOKEN_ID_1, TEST_SC_ID_1);
    std::vector<SecCompEntity> componentList;
    ASSERT_EQ(SecCompManager::GetInstance().AddProcessComponent(componentList, entity), SC_OK);

    std::shared_ptr<SecCompBase> compPtrNew = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtrNew, nullptr);
    compPtrNew->rect_.x_ = TEST_COORDINATE + 1; // offset 1 pixel
    compPtrNew->rect_.y_ = TEST_COORDINATE;
    compPtrNew->rect_.width_ = TEST_COORDINATE;
    compPtrNew->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entityNew(compPtrNew, TEST_TOKEN_ID_2, TEST_SC_ID_2);
    ASSERT_EQ(SecCompManager::GetInstance().AddProcessComponent(componentList, entityNew),
        SC_SERVICE_ERROR_COMPONENT_RECT_OVERLAP);
}

/**
 * @tc.name: AddSecurityComponentToList001
 * @tc.desc: Test add security component to list
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, AddSecurityComponentToList001, TestSize.Level1)
{
    std::shared_ptr<SecCompBase> compPtr = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtr, nullptr);
    compPtr->rect_.x_ = TEST_COORDINATE;
    compPtr->rect_.y_ = TEST_COORDINATE;
    compPtr->rect_.width_ = TEST_COORDINATE;
    compPtr->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entity(compPtr, TEST_TOKEN_ID_1, TEST_SC_ID_1);

    ASSERT_EQ(SecCompManager::GetInstance().AddSecurityComponentToList(1, entity), SC_OK);
    ASSERT_EQ(SecCompManager::GetInstance().AddSecurityComponentToList(1, entity),
        SC_SERVICE_ERROR_COMPONENT_RECT_OVERLAP);

    std::shared_ptr<SecCompBase> compPtrNew = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtrNew, nullptr);
    compPtrNew->rect_.x_ = TEST_COORDINATE * 2; // not overlap
    compPtrNew->rect_.y_ = TEST_COORDINATE * 2; // not overlap
    compPtrNew->rect_.width_ = TEST_COORDINATE;
    compPtrNew->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entityNew(compPtrNew, TEST_TOKEN_ID_1, TEST_SC_ID_2);
    ASSERT_EQ(SecCompManager::GetInstance().AddSecurityComponentToList(1, entityNew), SC_OK);
}

/**
 * @tc.name: DeleteSecurityComponentFromList001
 * @tc.desc: Test delete security component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, DeleteSecurityComponentFromList001, TestSize.Level1)
{
    ASSERT_EQ(SecCompManager::GetInstance().DeleteSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_1),
        SC_SERVICE_ERROR_COMPONENT_NOT_EXIST);

    std::shared_ptr<SecCompBase> compPtr = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtr, nullptr);
    compPtr->rect_.x_ = TEST_COORDINATE;
    compPtr->rect_.y_ = TEST_COORDINATE;
    compPtr->rect_.width_ = TEST_COORDINATE;
    compPtr->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entity(compPtr, TEST_TOKEN_ID_1, TEST_SC_ID_1);
    ASSERT_EQ(SecCompManager::GetInstance().AddSecurityComponentToList(TEST_UID_1, entity), SC_OK);

    ASSERT_EQ(SecCompManager::GetInstance().DeleteSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_2),
        SC_SERVICE_ERROR_COMPONENT_NOT_EXIST);

    ASSERT_EQ(SecCompManager::GetInstance().DeleteSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_1), SC_OK);
}

/**
 * @tc.name: GetSecurityComponentFromList001
 * @tc.desc: Test get security component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, GetSecurityComponentFromList001, TestSize.Level1)
{
    ASSERT_EQ(SecCompManager::GetInstance().GetSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_1), nullptr);

    std::shared_ptr<SecCompBase> compPtr = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtr, nullptr);
    compPtr->rect_.x_ = TEST_COORDINATE;
    compPtr->rect_.y_ = TEST_COORDINATE;
    compPtr->rect_.width_ = TEST_COORDINATE;
    compPtr->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entity(compPtr, TEST_TOKEN_ID_1, TEST_SC_ID_1);
    ASSERT_EQ(SecCompManager::GetInstance().AddSecurityComponentToList(TEST_UID_1, entity), SC_OK);
    ASSERT_EQ(SecCompManager::GetInstance().GetSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_2), nullptr);
    ASSERT_NE(SecCompManager::GetInstance().GetSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_1), nullptr);
}

/**
 * @tc.name: NotifyProcessBackground001
 * @tc.desc: Test notify process background
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, NotifyProcessBackground001, TestSize.Level1)
{
    SecCompManager::GetInstance().NotifyProcessBackground(TEST_UID_1);

    std::shared_ptr<SecCompBase> compPtr = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtr, nullptr);
    compPtr->rect_.x_ = TEST_COORDINATE;
    compPtr->rect_.y_ = TEST_COORDINATE;
    compPtr->rect_.width_ = TEST_COORDINATE;
    compPtr->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entity(compPtr, TEST_TOKEN_ID_1, TEST_SC_ID_1);
    ASSERT_EQ(SecCompManager::GetInstance().AddSecurityComponentToList(TEST_UID_1, entity), SC_OK);
    ASSERT_NE(SecCompManager::GetInstance().GetSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_1), nullptr);
    SecCompManager::GetInstance().NotifyProcessBackground(TEST_UID_1);
    ASSERT_EQ(SecCompManager::GetInstance().GetSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_1), nullptr);
}

/**
 * @tc.name: NotifyProcessDied001
 * @tc.desc: Test notify process died
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, NotifyProcessDied001, TestSize.Level1)
{
    std::shared_ptr<SecCompBase> compPtr = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtr, nullptr);
    compPtr->rect_.x_ = TEST_COORDINATE;
    compPtr->rect_.y_ = TEST_COORDINATE;
    compPtr->rect_.width_ = TEST_COORDINATE;
    compPtr->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entity(compPtr, TEST_TOKEN_ID_1, TEST_SC_ID_1);
    ASSERT_EQ(SecCompManager::GetInstance().AddSecurityComponentToList(TEST_UID_1, entity), SC_OK);

    std::shared_ptr<SecCompBase> compPtr2 = std::make_shared<SecCompBase>();
    ASSERT_NE(compPtr2, nullptr);
    compPtr2->rect_.x_ = TEST_COORDINATE * 2; // not overlap
    compPtr2->rect_.y_ = TEST_COORDINATE * 2; // not overlap
    compPtr2->rect_.width_ = TEST_COORDINATE;
    compPtr2->rect_.height_ = TEST_COORDINATE;
    SecCompEntity entity2(compPtr2, TEST_TOKEN_ID_1, TEST_SC_ID_2);
    ASSERT_EQ(SecCompManager::GetInstance().AddSecurityComponentToList(TEST_UID_2, entity2), SC_OK);

    SecCompManager::GetInstance().NotifyProcessDied(TEST_UID_3);
    ASSERT_NE(SecCompManager::GetInstance().GetSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_1), nullptr);

    SecCompManager::GetInstance().NotifyProcessDied(TEST_UID_1);
    ASSERT_EQ(SecCompManager::GetInstance().GetSecurityComponentFromList(TEST_UID_1, TEST_SC_ID_1), nullptr);
}

/**
 * @tc.name: RegisterSecurityComponent001
 * @tc.desc: Test register security component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, RegisterSecurityComponent001, TestSize.Level1)
{
    nlohmann::json jsonInvalid;
    LocationButton buttonInvalid = BuildInvalidLocationComponent();
    buttonInvalid.ToJson(jsonInvalid);
    int32_t scId;
    SecCompCallerInfo caller = {
        .tokenId = TEST_TOKEN_ID_1,
        .uid = TEST_UID_1
    };
    ASSERT_EQ(SecCompManager::GetInstance().RegisterSecurityComponent(LOCATION_COMPONENT, jsonInvalid, caller, scId),
        SC_SERVICE_ERROR_COMPONENT_INFO_INVALID);

    nlohmann::json jsonValid;
    LocationButton buttonValid = BuildValidLocationComponent();
    buttonValid.ToJson(jsonValid);

    ASSERT_EQ(SecCompManager::GetInstance().RegisterSecurityComponent(LOCATION_COMPONENT, jsonValid, caller, scId),
        SC_OK);
    ASSERT_EQ(SecCompManager::GetInstance().RegisterSecurityComponent(LOCATION_COMPONENT, jsonValid, caller, scId),
        SC_SERVICE_ERROR_COMPONENT_RECT_OVERLAP);
    ASSERT_EQ(scId, INVALID_SC_ID);
}

/**
 * @tc.name: UpdateSecurityComponent001
 * @tc.desc: Test update security component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, UpdateSecurityComponent001, TestSize.Level1)
{
    nlohmann::json jsonValid;
    LocationButton buttonValid = BuildValidLocationComponent();
    buttonValid.ToJson(jsonValid);
    SecCompCallerInfo caller = {
        .tokenId = TEST_TOKEN_ID_1,
        .uid = TEST_UID_1
    };
    ASSERT_EQ(SecCompManager::GetInstance().UpdateSecurityComponent(TEST_SC_ID_1, jsonValid, caller),
        SC_SERVICE_ERROR_COMPONENT_NOT_EXIST);

    int32_t scId;
    ASSERT_EQ(SecCompManager::GetInstance().RegisterSecurityComponent(LOCATION_COMPONENT, jsonValid, caller, scId),
        SC_OK);
    nlohmann::json jsonInvalid;
    LocationButton buttonInvalid = BuildInvalidLocationComponent();
    buttonInvalid.ToJson(jsonInvalid);
    ASSERT_EQ(SecCompManager::GetInstance().UpdateSecurityComponent(scId, jsonInvalid, caller),
        SC_SERVICE_ERROR_COMPONENT_INFO_INVALID);
}

/**
 * @tc.name: UnregisterSecurityComponent001
 * @tc.desc: Test unregister security component
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, UnregisterSecurityComponent001, TestSize.Level1)
{
    SecCompCallerInfo caller = {
        .tokenId = TEST_TOKEN_ID_1,
        .uid = TEST_UID_1
    };

    ASSERT_EQ(SecCompManager::GetInstance().UnregisterSecurityComponent(TEST_INVALID_SC_ID, caller),
        SC_SERVICE_ERROR_VALUE_INVALID);
}

/**
 * @tc.name: ReportSecurityComponentClickEvent001
 * @tc.desc: Test report security component click
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompManagerTest, ReportSecurityComponentClickEvent001, TestSize.Level1)
{
    SecCompCallerInfo caller = {
        .tokenId = TEST_TOKEN_ID_1,
        .uid = TEST_UID_1
    };
    SecCompClickEvent touchInfo;
    nlohmann::json jsonVaild;
    LocationButton buttonValid = BuildValidLocationComponent();
    buttonValid.ToJson(jsonVaild);
    ASSERT_EQ(SecCompManager::GetInstance().ReportSecurityComponentClickEvent(1, jsonVaild, caller, touchInfo),
        SC_SERVICE_ERROR_COMPONENT_NOT_EXIST);

    int32_t scId;
    ASSERT_EQ(SecCompManager::GetInstance().RegisterSecurityComponent(LOCATION_COMPONENT, jsonVaild, caller, scId),
        SC_OK);

    nlohmann::json jsonInvalid;
    LocationButton buttonInvalid = BuildInvalidLocationComponent();
    buttonInvalid.ToJson(jsonInvalid);

    ASSERT_EQ(SecCompManager::GetInstance().ReportSecurityComponentClickEvent(scId, jsonInvalid, caller, touchInfo),
        SC_SERVICE_ERROR_COMPONENT_INFO_INVALID);

    ASSERT_EQ(SecCompManager::GetInstance().ReportSecurityComponentClickEvent(scId, jsonInvalid, caller, touchInfo),
        SC_SERVICE_ERROR_COMPONENT_INFO_INVALID);

    nlohmann::json jsonValid1;
    LocationButton buttonValid1 = BuildValidLocationComponent();
    buttonValid1.fontSize_ = TEST_DIFF_FONT_SIZE;
    buttonValid1.ToJson(jsonValid1);
    ASSERT_EQ(SecCompManager::GetInstance().ReportSecurityComponentClickEvent(scId, jsonValid1, caller, touchInfo),
        SC_SERVICE_ERROR_COMPONENT_INFO_NOT_EQUAL);

    touchInfo.touchX = 500.0; // not hit component
    touchInfo.touchY = 500.0; // not hit component
    ASSERT_EQ(SecCompManager::GetInstance().ReportSecurityComponentClickEvent(scId, jsonVaild, caller, touchInfo),
        SC_SERVICE_ERROR_CLICK_EVENT_INVALID);
}
