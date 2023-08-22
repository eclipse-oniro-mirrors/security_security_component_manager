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
#include "paste_button_test.h"

#include <string>
#include "sec_comp_err.h"
#include "sec_comp_log.h"
#include "test_common.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "PasteButtonTest"};
}

void PasteButtonTest::SetUpTestCase()
{}

void PasteButtonTest::TearDownTestCase()
{}

void PasteButtonTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
}

void PasteButtonTest::TearDown()
{}

/**
 * @tc.name: IsParamValid001
 * @tc.desc: Test paste button from wrong value params json
 * @tc.type: FUNC
 * @tc.require: AR000HO9JB
 */
HWTEST_F(PasteButtonTest, FromJson010, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    TestCommon::BuildPasteComponentInfo(jsonComponent);
    PasteButton button;

    ASSERT_TRUE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, UNKNOWN_TEXT },
        { JsonTagConstants::JSON_ICON_TAG, PasteIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, PasteDesc::PASTE},
        { JsonTagConstants::JSON_ICON_TAG, UNKNOWN_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, PasteDesc::PASTE },
        { JsonTagConstants::JSON_ICON_TAG, PasteIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::UNKNOWN_BG },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, PasteDesc::MAX_LABEL_TYPE },
        { JsonTagConstants::JSON_ICON_TAG, PasteIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, PasteDesc::PASTE },
        { JsonTagConstants::JSON_ICON_TAG, PasteIcon::MAX_ICON_TYPE },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, PasteDesc::PASTE },
        { JsonTagConstants::JSON_ICON_TAG, PasteIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::MAX_BG_TYPE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));
}

/**
 * @tc.name: ComparePasteButton001
 * @tc.desc: Test compare paste button
 * @tc.type: FUNC
 * @tc.require: AR000HO9JB
 */
HWTEST_F(PasteButtonTest, ComparePasteButton001, TestSize.Level1)
{
    PasteButton button1;
    PasteButton button2;

    nlohmann::json jsonComponent;
    TestCommon::BuildPasteComponentInfo(jsonComponent);

    ASSERT_TRUE(button1.FromJson(jsonComponent));
    ASSERT_TRUE(button2.FromJson(jsonComponent));
    ASSERT_TRUE(button1.CompareComponentBasicInfo(&button2, true));

    button1.text_ = UNKNOWN_TEXT;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2, true));
    button1.text_ = static_cast<int32_t>(PasteDesc::PASTE);

    button1.icon_ = UNKNOWN_ICON;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2, true));
    button1.icon_ = static_cast<int32_t>(PasteIcon::LINE_ICON);

    button1.bg_ = SecCompBackground::UNKNOWN_BG;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2, true));
    button1.bg_ = SecCompBackground::CIRCLE;

    ASSERT_TRUE(button1.CompareComponentBasicInfo(&button2, true));
}

/**
 * @tc.name: ComparePasteButton002
 * @tc.desc: Test PasteButton compare
 * @tc.type: FUNC
 * @tc.require: AR000HO9JB
 */
HWTEST_F(PasteButtonTest, ComparePasteButton002, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    PasteButton comp1;
    TestCommon::BuildPasteComponentInfo(jsonComponent);
    ASSERT_TRUE(comp1.FromJson(jsonComponent));
    PasteButton comp2 = comp1;

    comp1.type_ = LOCATION_COMPONENT;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.type_ = PASTE_COMPONENT;

    comp1.fontSize_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.fontSize_ = TestCommon::TEST_SIZE;

    comp1.iconSize_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.iconSize_ = TestCommon::TEST_SIZE;

    comp1.padding_.top = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.padding_.top = TestCommon::TEST_DIMENSION;

    comp1.padding_.right = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.padding_.right = TestCommon::TEST_DIMENSION;

    comp1.padding_.bottom = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.padding_.bottom = TestCommon::TEST_DIMENSION;

    comp1.padding_.left = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.padding_.left = TestCommon::TEST_DIMENSION;

    comp1.textIconSpace_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.textIconSpace_ = TestCommon::TEST_SIZE;

    comp1.borderWidth_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.borderWidth_ = TestCommon::TEST_SIZE;

    comp1.fontColor_.value = TestCommon::TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.fontColor_.value = TestCommon::TEST_COLOR_RED;

    comp1.bgColor_.value = TestCommon::TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.bgColor_.value = TestCommon::TEST_COLOR_YELLOW;

    comp1.iconColor_.value = TestCommon::TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2, true));
    comp1.iconColor_.value = TestCommon::TEST_COLOR_BLUE;

    ASSERT_TRUE(comp1.CompareComponentBasicInfo(&comp2, true));
}
