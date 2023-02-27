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
#include "sec_comp_entity_test.h"
#include "sec_comp_log.h"
#include "sec_comp_err.h"
#include "sec_comp_tool.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;
using namespace OHOS::Security::AccessToken;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompEntityTest"};
static constexpr int32_t TEST_SC_ID = 1;
static constexpr AccessTokenID TEST_TOKEN_ID = 1;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr double TEST_DIFF_COORDINATE = 200.0;
}

void SecCompEntityTest::SetUpTestCase()
{}

void SecCompEntityTest::TearDownTestCase()
{}

void SecCompEntityTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
    if (entity_ != nullptr) {
        return;
    }

    std::shared_ptr<SecCompBase> component = std::make_shared<SecCompBase>();
    ASSERT_NE(nullptr, component);

    entity_ = std::make_shared<SecCompEntity>(component, 1, 1);
    ASSERT_NE(nullptr, entity_);
}

void SecCompEntityTest::TearDown()
{
    entity_ = nullptr;
}

/**
 * @tc.name: RevokeTempPermission001
 * @tc.desc: Test revoke temp permission
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompEntityTest, RevokeTempPermission001, TestSize.Level1)
{
    entity_->isGrant_ = false;
    ASSERT_EQ(entity_->RevokeTempPermission(), SC_OK);

    entity_->isGrant_ = true;
    entity_->componentInfo_->type_ = UNKNOWN_SC_TYPE;
    ASSERT_EQ(entity_->RevokeTempPermission(), SC_SERVICE_ERROR_PERMISSION_OPER_FAIL);
    ASSERT_FALSE(entity_->isGrant_);

    entity_->isGrant_ = true;
    entity_->componentInfo_->type_ = LOCATION_COMPONENT;
    ASSERT_EQ(entity_->RevokeTempPermission(), SC_OK);
    ASSERT_FALSE(entity_->isGrant_);
}

/**
 * @tc.name: GrantTempPermission001
 * @tc.desc: Test grant permission
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompEntityTest, GrantTempPermission001, TestSize.Level1)
{
    entity_->isGrant_ = true;
    ASSERT_EQ(entity_->GrantTempPermission(), SC_OK);

    entity_->isGrant_ = false;
    entity_->componentInfo_->type_ = UNKNOWN_SC_TYPE;
    ASSERT_EQ(entity_->GrantTempPermission(), SC_SERVICE_ERROR_PERMISSION_OPER_FAIL);
    ASSERT_TRUE(entity_->isGrant_);

    entity_->isGrant_ = false;
    entity_->componentInfo_->type_ = LOCATION_COMPONENT;
    ASSERT_EQ(entity_->GrantTempPermission(), SC_OK);
    ASSERT_TRUE(entity_->isGrant_);
}

/**
 * @tc.name: CheckTouchInfo001
 * @tc.desc: Test touch info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompEntityTest, CheckTouchInfo001, TestSize.Level1)
{
    SecCompClickEvent touch = {
        .touchX = TEST_COORDINATE,
        .touchY = TEST_COORDINATE,
        .timestamp = 0,
    };
    ASSERT_FALSE(entity_->CheckTouchInfo(touch));

    uint64_t current = GetCurrentTime();
    touch.timestamp = current + 10000L; // 10s
    ASSERT_FALSE(entity_->CheckTouchInfo(touch));

    entity_->componentInfo_->rect_.x_ = TEST_DIFF_COORDINATE; // click event will not hit this rect
    entity_->componentInfo_->rect_.y_ = TEST_DIFF_COORDINATE;
    entity_->componentInfo_->rect_.width_ = TEST_DIFF_COORDINATE;
    entity_->componentInfo_->rect_.height_ = TEST_DIFF_COORDINATE;
    touch.timestamp = GetCurrentTime();
    ASSERT_FALSE(entity_->CheckTouchInfo(touch));

    entity_->componentInfo_->rect_.x_ = TEST_COORDINATE;
    entity_->componentInfo_->rect_.y_ = TEST_COORDINATE;
    touch.timestamp = GetCurrentTime();
    ASSERT_TRUE(entity_->CheckTouchInfo(touch));
}

/**
 * @tc.name: IsRectOverlaped001
 * @tc.desc: Test check rect overlap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompEntityTest, IsRectOverlaped001, TestSize.Level1)
{
    SecCompEntity other(nullptr, TEST_TOKEN_ID, TEST_SC_ID);
    ASSERT_TRUE(entity_->IsRectOverlaped(other));

    entity_->componentInfo_->rect_.x_ = TEST_COORDINATE;
    entity_->componentInfo_->rect_.y_ = TEST_COORDINATE;
    entity_->componentInfo_->rect_.width_ = TEST_COORDINATE;
    entity_->componentInfo_->rect_.height_ = TEST_COORDINATE;

    other.componentInfo_ = std::make_shared<SecCompBase>();
    ASSERT_NE(other.componentInfo_, nullptr);
    other.componentInfo_->rect_.x_ = TEST_COORDINATE;
    other.componentInfo_->rect_.y_ = TEST_COORDINATE;
    other.componentInfo_->rect_.width_ = TEST_COORDINATE;
    other.componentInfo_->rect_.height_ = TEST_COORDINATE;
    ASSERT_TRUE(entity_->IsRectOverlaped(other));
}
