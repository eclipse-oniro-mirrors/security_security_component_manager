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

#include "sec_comp_info_helper_test.h"
#include "location_button.h"
#include "paste_button.h"
#include "save_button.h"
#include "sec_comp_log.h"
#include "sec_comp_err.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompInfoHelperTest"};

static constexpr uint32_t TEST_FONT_SIZE = 100;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr uint32_t TEST_COLOR = 0xffffffff;

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

void SecCompInfoHelperTest::SetUpTestCase()
{}

void SecCompInfoHelperTest::TearDownTestCase()
{}

void SecCompInfoHelperTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
}

void SecCompInfoHelperTest::TearDown()
{}

/**
 * @tc.name: ParseComponent001
 * @tc.desc: Test parse component info
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent001, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    ASSERT_EQ(SecCompInfoHelper::ParseComponent(UNKNOWN_SC_TYPE, jsonComponent), nullptr);

    LocationButton button = BuildValidLocationComponent();
    button.fontSize_ = 0;
    button.ToJson(jsonComponent);
    ASSERT_EQ(SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent), nullptr);
    ASSERT_NE(SecCompInfoHelper::ParseComponent(PASTE_COMPONENT, jsonComponent), nullptr);
    ASSERT_NE(SecCompInfoHelper::ParseComponent(SAVE_COMPONENT, jsonComponent), nullptr);
}

/**
 * @tc.name: CompareSecCompInfo001
 * @tc.desc: Test compare sec comp info
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(SecCompInfoHelperTest, CompareSecCompInfo001, TestSize.Level1)
{
    SecCompBase comp1;
    ASSERT_FALSE(SecCompInfoHelper::CompareSecCompInfo(&comp1, nullptr));
    ASSERT_FALSE(SecCompInfoHelper::CompareSecCompInfo(nullptr, &comp1));

    SecCompBase comp2;
    comp2.type_ = UNKNOWN_SC_TYPE;
    comp1.type_ = LOCATION_COMPONENT;
    ASSERT_FALSE(SecCompInfoHelper::CompareSecCompInfo(&comp1, &comp2));

    comp1.type_ = UNKNOWN_SC_TYPE;
    ASSERT_FALSE(SecCompInfoHelper::CompareSecCompInfo(&comp1, &comp2));

    LocationButton locationButton1 = BuildValidLocationComponent();
    LocationButton locationButton2 = BuildValidLocationComponent();
    ASSERT_TRUE(SecCompInfoHelper::CompareSecCompInfo(&locationButton1, &locationButton2));

    PasteButton pasteButton;
    pasteButton.type_ = PASTE_COMPONENT;
    pasteButton.rect_.x_ = TEST_COORDINATE;
    pasteButton.rect_.y_ = TEST_COORDINATE;
    ASSERT_TRUE(SecCompInfoHelper::CompareSecCompInfo(&pasteButton, &pasteButton));

    SaveButton saveButton;
    saveButton.type_ = SAVE_COMPONENT;
    saveButton.rect_.x_ = TEST_COORDINATE;
    saveButton.rect_.y_ = TEST_COORDINATE;
    ASSERT_TRUE(SecCompInfoHelper::CompareSecCompInfo(&saveButton, &saveButton));
}
