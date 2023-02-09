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
#include "sec_comp_kit_test.h"
#include "location_button.h"
#include "sec_comp_err.h"
#include "sec_comp_info.h"
#include "sec_comp_log.h"
#include "sec_comp_tool.h"

using namespace testing::ext;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompKitTest"};
static constexpr uint32_t TEST_FONT_SIZE = 100;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr uint32_t TEST_COLOR = 0xffffffff;
}  // namespace

void SecCompKitTest::SetUpTestCase()
{
    SC_LOG_INFO(LABEL, "SetUpTestCase.");
}

void SecCompKitTest::TearDownTestCase()
{
    SC_LOG_INFO(LABEL, "TearDownTestCase.");
}

void SecCompKitTest::SetUp()
{
    SC_LOG_INFO(LABEL, "SetUp ok.");
}

void SecCompKitTest::TearDown()
{
    SC_LOG_INFO(LABEL, "TearDown.");
}

/**
 * @tc.name: ExceptCall001
 * @tc.desc: do kit except call.
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompKitTest, ExceptCall001, TestSize.Level1)
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

    nlohmann::json jsonRes;
    button.ToJson(jsonRes);
    int32_t scId = -1;
    ASSERT_NE(SC_OK, SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, jsonRes.dump(), scId));
    ASSERT_EQ(-1, scId);
    ASSERT_NE(SC_OK, SecCompKit::UpdateSecurityComponent(scId, jsonRes.dump()));

    struct SecCompClickEvent touch = {
        .touchX = TEST_COORDINATE,
        .touchY = TEST_COORDINATE,
        .timestamp = GetCurrentTime()
    };
    EXPECT_NE(SC_OK, SecCompKit::ReportSecurityComponentClickEvent(scId, jsonRes.dump(), touch));
    EXPECT_NE(SC_OK, SecCompKit::UnregisterSecurityComponent(scId));
}
