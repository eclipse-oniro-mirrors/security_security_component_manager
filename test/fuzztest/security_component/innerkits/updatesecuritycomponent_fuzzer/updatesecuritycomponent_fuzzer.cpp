/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "accesstoken_kit.h"
#include "fuzz_common.h"
#include "securec.h"
#include "token_setproc.h"
#include "updatesecuritycomponent_fuzzer.h"
#include "nlohmann/json.hpp"
#include "sec_comp_base.h"

using namespace OHOS::Security::SecurityComponent;
using namespace OHOS::Security::AccessToken;
namespace OHOS {
static void UpdateSecurityComponentFuzzTest(const uint8_t *data, size_t size)
{
    CompoRandomGenerator generator(data, size);
    int32_t scId = generator.GetData<int32_t>();
    std::string componentInfo = generator.GenerateRandomCompoStr(generator.GetScType());
    SecCompKit::UpdateSecurityComponent(scId, componentInfo);
}
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::UpdateSecurityComponentFuzzTest(data, size);
    return 0;
}
