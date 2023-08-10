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
#include "save_button_test.h"
#include <string>
#include "sec_comp_log.h"
#include "sec_comp_err.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SaveButtonTest"};

static constexpr float TEST_SIZE = 100.0;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr double TEST_DIMENSION = 100.0;
static constexpr uint32_t TEST_COLOR_YELLOW = 0x7ffff00;
static constexpr uint32_t TEST_COLOR_RED = 0xff0000;
static constexpr uint32_t TEST_COLOR_BLUE = 0x0000ff;
static constexpr uint32_t TEST_DIFF_COLOR = 0;

static void BuildSaveComponentInfo(nlohmann::json& jsonComponent)
{
    jsonComponent[JsonTagConstants::JSON_SC_TYPE] = SAVE_COMPONENT;
    jsonComponent[JsonTagConstants::JSON_NODE_ID] = 0;
    jsonComponent[JsonTagConstants::JSON_RECT] = nlohmann::json {
        {JsonTagConstants::JSON_RECT_X, TEST_COORDINATE },
        {JsonTagConstants::JSON_RECT_Y, TEST_COORDINATE },
        {JsonTagConstants::JSON_RECT_WIDTH, TEST_COORDINATE },
        {JsonTagConstants::JSON_RECT_HEIGHT, TEST_COORDINATE }
    };
    jsonComponent[JsonTagConstants::JSON_WINDOW_RECT] = nlohmann::json {
        {JsonTagConstants::JSON_RECT_X, TEST_COORDINATE },
        {JsonTagConstants::JSON_RECT_Y, TEST_COORDINATE },
        {JsonTagConstants::JSON_RECT_WIDTH, TEST_COORDINATE },
        {JsonTagConstants::JSON_RECT_HEIGHT, TEST_COORDINATE }
    };
    nlohmann::json jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };

    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };

    jsonComponent[JsonTagConstants::JSON_BORDER_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_BORDER_WIDTH_TAG, TEST_SIZE },
    };
    jsonComponent[JsonTagConstants::JSON_PARENT_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_PARENT_EFFECT_TAG, false },
    };
    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, SaveDesc::DOWNLOAD },
        { JsonTagConstants::JSON_ICON_TAG, SaveIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
}
}

void SaveButtonTest::SetUpTestCase()
{}

void SaveButtonTest::TearDownTestCase()
{}

void SaveButtonTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
}

void SaveButtonTest::TearDown()
{}

/**
 * @tc.name: IsParamValid001
 * @tc.desc: Test save button from wrong value params json
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SaveButtonTest, FromJson010, TestSize.Level1)
{
nlohmann::json jsonComponent;
    BuildSaveComponentInfo(jsonComponent);
    SaveButton button;

    ASSERT_TRUE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, UNKNOWN_TEXT },
        { JsonTagConstants::JSON_ICON_TAG, SaveIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, SaveDesc::DOWNLOAD},
        { JsonTagConstants::JSON_ICON_TAG, UNKNOWN_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, SaveDesc::DOWNLOAD },
        { JsonTagConstants::JSON_ICON_TAG, SaveIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::UNKNOWN_BG },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, SaveDesc::MAX_LABEL_TYPE },
        { JsonTagConstants::JSON_ICON_TAG, SaveIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, SaveDesc::DOWNLOAD },
        { JsonTagConstants::JSON_ICON_TAG, SaveIcon::MAX_ICON_TYPE },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, SaveDesc::DOWNLOAD },
        { JsonTagConstants::JSON_ICON_TAG, SaveIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::MAX_BG_TYPE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));
}

/**
 * @tc.name: CompareSaveButton001
 * @tc.desc: Test compare save button
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SaveButtonTest, CompareSaveButton001, TestSize.Level1)
{
    SaveButton button1;
    SaveButton button2;

    nlohmann::json jsonComponent;
    BuildSaveComponentInfo(jsonComponent);

    ASSERT_TRUE(button1.FromJson(jsonComponent));
    ASSERT_TRUE(button2.FromJson(jsonComponent));
    ASSERT_TRUE(button1.CompareComponentBasicInfo(&button2, true));

    button1.text_ = UNKNOWN_TEXT;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2, true));
    button1.text_ = static_cast<int32_t>(SaveDesc::DOWNLOAD);

    button1.icon_ = UNKNOWN_ICON;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2, true));
    button1.icon_ = static_cast<int32_t>(SaveIcon::LINE_ICON);

    button1.bg_ = SecCompBackground::UNKNOWN_BG;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2, true));
    button1.bg_ = SecCompBackground::CIRCLE;

    ASSERT_TRUE(button1.CompareComponentBasicInfo(&button2, true));
}

/**
 * @tc.name: CompareSaveButton002
 * @tc.desc: Test SaveButton compare
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SaveButtonTest, CompareSaveButton002, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    SaveButton comp1;
    BuildSaveComponentInfo(jsonComponent);
    ASSERT_TRUE(comp1.FromJson(jsonComponent));
    SaveButton comp2 = comp1;

    comp1.type_ = PASTE_COMPONENT;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.type_ = SAVE_COMPONENT;

    comp1.fontSize_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.fontSize_ = TEST_SIZE;

    comp1.iconSize_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.iconSize_ = TEST_SIZE;

    comp1.padding_.top = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.padding_.top = TEST_DIMENSION;

    comp1.padding_.right = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.padding_.right = TEST_DIMENSION;

    comp1.padding_.bottom = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.padding_.bottom = TEST_DIMENSION;

    comp1.padding_.left = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.padding_.left = TEST_DIMENSION;

    comp1.textIconSpace_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.textIconSpace_ = TEST_SIZE;

    comp1.borderWidth_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.borderWidth_ = TEST_SIZE;

    comp1.fontColor_.value = TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.fontColor_.value = TEST_COLOR_RED;

    comp1.bgColor_.value = TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.bgColor_.value = TEST_COLOR_YELLOW;

    comp1.iconColor_.value = TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.iconColor_.value = TEST_COLOR_BLUE;

    ASSERT_TRUE(comp1.CompareComponentBasicInfo(&comp2, true));
}