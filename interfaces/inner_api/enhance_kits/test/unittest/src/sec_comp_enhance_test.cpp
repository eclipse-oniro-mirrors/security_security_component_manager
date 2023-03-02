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
#include "sec_comp_enhance_test.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"
#include <unistd.h>

using namespace testing::ext;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompEnhanceTest"};
static bool g_inputEnhanceExist = false;
static bool g_srvEnhanceExist = false;
#ifdef _ARM64_
static const std::string LIB_PATH = "/system/lib64/";
#else
static const std::string LIB_PATH = "/system/lib/";
#endif
static const std::string ENHANCE_INPUT_INTERFACE_LIB = LIB_PATH + "libsec_comp_input_enhance.z.so";
static const std::string ENHANCE_SRV_INTERFACE_LIB = LIB_PATH + "libsec_comp_service_enhance.z.so";
}  // namespace

void SecCompEnhanceTest::SetUpTestCase()
{
    if (access(ENHANCE_INPUT_INTERFACE_LIB.c_str(), F_OK) == 0) {
        g_inputEnhanceExist = true;
    }

    if (access(ENHANCE_SRV_INTERFACE_LIB.c_str(), F_OK) == 0) {
        g_srvEnhanceExist = true;
    }

    SC_LOG_INFO(LABEL, "SetUpTestCase.");
}

void SecCompEnhanceTest::TearDownTestCase()
{
    SC_LOG_INFO(LABEL, "TearDownTestCase.");
}

void SecCompEnhanceTest::SetUp()
{
    SC_LOG_INFO(LABEL, "SetUp ok.");
}

void SecCompEnhanceTest::TearDown()
{
    SC_LOG_INFO(LABEL, "TearDown.");
}

/**
 * @tc.name: SetEnhanceCfg001
 * @tc.desc: test SetEnhanceCfg
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompEnhanceTest, SetEnhanceCfg001, TestSize.Level1)
{
    uint8_t cfgData[16] = { 0 };
    int32_t result = SecCompEnhanceKit::SetEnhanceCfg(reinterpret_cast<SecCompEnhanceCfgBase *>(&cfgData));
    if (g_inputEnhanceExist) {
        EXPECT_EQ(result, SC_OK);
    } else {
        EXPECT_EQ(result, SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE);
    }
}

/**
 * @tc.name: GetPoniterEventEnhanceData001
 * @tc.desc: test GetPoniterEventEnhanceData
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompEnhanceTest, GetPoniterEventEnhanceData001, TestSize.Level1)
{
    uint8_t originData[16] = { 0 };
    uint32_t dataLen = 16;
    uint8_t* enhanceData = nullptr;
    uint32_t enHancedataLen = 0;

    int32_t result = SecCompEnhanceKit::GetPointerEventEnhanceData(originData, dataLen, &enhanceData, enHancedataLen);
    if (g_inputEnhanceExist) {
        EXPECT_EQ(result, SC_OK);
        EXPECT_EQ(enHancedataLen, static_cast<uint32_t>(0));
    } else {
        EXPECT_EQ(result, SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE);
    }
}

/**
 * @tc.name: MarshallEnhanceCfg001
 * @tc.desc: test MarshallEnhanceCfg
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompEnhanceTest, MarshallEnhanceCfg001, TestSize.Level1)
{
    OHOS::Parcel out;
    int32_t result = SecCompEnhanceAdapter::MarshallEnhanceCfg(nullptr, out);
    if (g_inputEnhanceExist) {
        EXPECT_EQ(result, SC_ENHANCE_ERROR_OPER_FAIL);
    } else {
        EXPECT_EQ(result, SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE);
    }
}

/**
 * @tc.name: UnmarshallEnhanceCfg001
 * @tc.desc: test UnmarshallEnhanceCfg
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompEnhanceTest, UnmarshallEnhanceCfg001, TestSize.Level1)
{
    OHOS::Parcel in;
    SecCompEnhanceCfgBase* cfg = nullptr;
    int32_t result = SecCompEnhanceAdapter::UnmarshallEnhanceCfg(in, cfg);
    if (g_inputEnhanceExist) {
        EXPECT_EQ(result, SC_OK);
        EXPECT_EQ(cfg, nullptr);
    } else {
        EXPECT_EQ(result, SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE);
    }
}

/**
 * @tc.name: EnableInputEnhance001
 * @tc.desc: test UnmarshallEnhanceCfg
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompEnhanceTest, EnableInputEnhance001, TestSize.Level1)
{
    int32_t result = SecCompEnhanceAdapter::EnableInputEnhance();
    if (g_srvEnhanceExist) {
        EXPECT_EQ(result, SC_OK);
    } else {
        EXPECT_EQ(result, SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE);
    }
}

/**
 * @tc.name: DisableInputEnhance001
 * @tc.desc: test DisableInputEnhance
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompEnhanceTest, DisableInputEnhance001, TestSize.Level1)
{
    int32_t result = SecCompEnhanceAdapter::DisableInputEnhance();
    if (g_srvEnhanceExist) {
        EXPECT_EQ(result, SC_OK);
    } else {
        EXPECT_EQ(result, SC_ENHANCE_ERROR_NOT_EXIST_ENHANCE);
    }
}
