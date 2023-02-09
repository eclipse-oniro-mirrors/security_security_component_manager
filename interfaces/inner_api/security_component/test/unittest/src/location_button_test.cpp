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
#include "location_button_test.h"
#include <string>
#include "sec_comp_log.h"
#include "sec_comp_err.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "LocationButtonTest"};

static const std::string JSON_FONT_FAMILY = "font";
static const std::string JSON_LABEL_TYPE = "label";
static const std::string JSON_ICON = "icon";
static const std::string JSON_FONT_SIZE = "fontSize";
static const std::string JSON_FONT_COLOR = "fontColor";
static const std::string JSON_BG_COLOR = "bgColor";
static const std::string JSON_BUTTON_TYPE = "buttonType";
static const std::string JSON_RECT = "rect";
static const std::string JSON_SC_TYPE = "type";
static const std::string JSON_RECT_X = "x";
static const std::string JSON_RECT_Y = "y";
static const std::string JSON_RECT_WIDTH = "width";
static const std::string JSON_RECT_HEIGHT = "height";

static constexpr uint32_t TEST_FONT_SIZE = 100;
static constexpr uint32_t TEST_INVALID_FONT_SIZE = 0;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr uint32_t TEST_COLOR = 0xffffffff;
static constexpr uint32_t TEST_DIFF_COLOR = 0;

static void BuildLocationComponentInfo(LocationButton& button)
{
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
}
}

void LocationButtonTest::SetUpTestCase()
{}

void LocationButtonTest::TearDownTestCase()
{}

void LocationButtonTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
}

void LocationButtonTest::TearDown()
{}

/**
 * @tc.name: FromJson001
 * @tc.desc: Test location button from json success
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(LocationButtonTest, FromJson001, TestSize.Level1)
{
    nlohmann::json jsonComponent;

    jsonComponent[JSON_FONT_FAMILY] = FONT_HO_SANS;
    jsonComponent[JSON_LABEL_TYPE] = DEFAULT_LABEL;
    jsonComponent[JSON_ICON] = DEFAULT_ICON;
    jsonComponent[JSON_FONT_SIZE] = TEST_FONT_SIZE;
    jsonComponent[JSON_FONT_COLOR] = TEST_COLOR;
    jsonComponent[JSON_BG_COLOR] = TEST_COLOR;
    jsonComponent[JSON_BUTTON_TYPE] = CAPSULE;
    jsonComponent[JSON_SC_TYPE] = LOCATION_COMPONENT;
    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };

    LocationButton button;
    button.FromJson(jsonComponent);
    ASSERT_TRUE(button.IsValid());
}

/**
 * @tc.name: FromJson002
 * @tc.desc: Test empty location button from json
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(LocationButtonTest, FromJson002, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    LocationButton button;
    button.FromJson(jsonComponent);
    ASSERT_FALSE(button.IsValid());
}

/**
 * @tc.name: FromJson003
 * @tc.desc: Test location button from wrong type params json
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(LocationButtonTest, FromJson003, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    jsonComponent[JSON_FONT_FAMILY] = "wrongType";
    jsonComponent[JSON_LABEL_TYPE] = "wrongType";
    jsonComponent[JSON_ICON] = "wrongType";
    jsonComponent[JSON_FONT_SIZE] = "wrongType";
    jsonComponent[JSON_FONT_COLOR] = "wrongType";
    jsonComponent[JSON_BG_COLOR] = "wrongType";
    jsonComponent[JSON_BUTTON_TYPE] = "wrongType";
    jsonComponent[JSON_BUTTON_TYPE] = "wrongType";
    jsonComponent[JSON_SC_TYPE] = "wrongType";
    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };

    LocationButton button;
    button.FromJson(jsonComponent);
    ASSERT_FALSE(button.IsValid());
    ASSERT_EQ(button.rect_.x_, 0);
    ASSERT_EQ(button.rect_.y_, 0);
    ASSERT_EQ(button.rect_.width_, 0);
    ASSERT_EQ(button.rect_.height_, 0);
}

/**
 * @tc.name: FromJson004
 * @tc.desc: Test location button from wrong value params json
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(LocationButtonTest, FromJson004, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    jsonComponent[JSON_FONT_FAMILY] = 0;
    jsonComponent[JSON_LABEL_TYPE] = 0;
    jsonComponent[JSON_ICON] = 0;
    jsonComponent[JSON_FONT_SIZE] = 0;
    jsonComponent[JSON_FONT_COLOR] = 0;
    jsonComponent[JSON_BG_COLOR] = 0;
    jsonComponent[JSON_BUTTON_TYPE] = 0;
    jsonComponent[JSON_BUTTON_TYPE] = 0;
    jsonComponent[JSON_SC_TYPE] = 0;
    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };

    LocationButton button;
    button.FromJson(jsonComponent);
    ASSERT_FALSE(button.IsValid());
}

/**
 * @tc.name: IsValid001
 * @tc.desc: Test check location button invalid
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(LocationButtonTest, IsValid001, TestSize.Level1)
{
    LocationButton button;
    BuildLocationComponentInfo(button);

    button.font_ = UNKNOWN_FONT_FAMILY_TYPE;
    ASSERT_FALSE(button.IsValid());
    button.font_ = FONT_HO_SANS;

    button.label_ = UNKNOWN_LABEL;
    ASSERT_FALSE(button.IsValid());
    button.label_ = DEFAULT_LABEL;

    button.icon_ = UNKNOWN_ICON;
    ASSERT_FALSE(button.IsValid());
    button.icon_ = DEFAULT_ICON;

    button.fontSize_ = TEST_INVALID_FONT_SIZE;
    ASSERT_FALSE(button.IsValid());
    button.fontSize_ = TEST_FONT_SIZE;

    button.buttonType_ = UNKNOWN_BUTTON_TYPE;
    ASSERT_FALSE(button.IsValid());
    button.buttonType_ = CAPSULE;

    button.type_ = UNKNOWN_SC_TYPE;
    ASSERT_FALSE(button.IsValid());
}

/**
 * @tc.name: CompareLocationButton001
 * @tc.desc: Test compare location button
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(LocationButtonTest, CompareLocationButton001, TestSize.Level1)
{
    LocationButton button1;
    BuildLocationComponentInfo(button1);

    LocationButton button2;
    BuildLocationComponentInfo(button2);

    button1.font_ = UNKNOWN_FONT_FAMILY_TYPE;
    ASSERT_FALSE(button1 == button2);
    button1.font_ = FONT_HO_SANS;

    button1.label_ = UNKNOWN_LABEL;
    ASSERT_FALSE(button1 == button2);
    button1.label_ = DEFAULT_LABEL;

    button1.icon_ = UNKNOWN_ICON;
    ASSERT_FALSE(button1 == button2);
    button1.icon_ = DEFAULT_ICON;

    button1.fontSize_ = TEST_INVALID_FONT_SIZE;
    ASSERT_FALSE(button1 == button2);
    button1.fontSize_ = TEST_FONT_SIZE;

    button1.fontColor_.value = TEST_DIFF_COLOR;
    ASSERT_FALSE(button1 == button2);
    button1.fontColor_.value = TEST_COLOR;

    button1.bgColor_.value = TEST_DIFF_COLOR;
    ASSERT_FALSE(button1 == button2);
    button1.bgColor_.value = TEST_COLOR;

    ASSERT_TRUE(button1 == button2);
}
