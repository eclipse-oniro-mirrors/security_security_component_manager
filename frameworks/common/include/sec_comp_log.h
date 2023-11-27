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
#ifndef SECURITY_COMPONENT_LOG_H
#define SECURITY_COMPONENT_LOG_H

#ifdef HILOG_ENABLE

#include "hilog/log.h"

#ifndef __cplusplus

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD005A05

#define SC_LOG_DEBUG(fmt, ...) HILOG_DEBUG(LOG_CORE, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)
#define SC_LOG_INFO(fmt, ...) HILOG_INFO(LOG_CORE, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)
#define SC_LOG_WARN(fmt, ...) HILOG_WARN(LOG_CORE, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)
#define SC_LOG_ERROR(fmt, ...) HILOG_ERROR(LOG_CORE, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)
#define SC_LOG_FATAL(fmt, ...) HILOG_FATAL(LOG_CORE, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)

#else

static constexpr unsigned int SECURITY_DOMAIN_SECURITY_COMPONENT = 0xD005A05;

#define SC_LOG_DEBUG(label, fmt, ...) \
    OHOS::HiviewDFX::HiLog::Debug(label, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)
#define SC_LOG_INFO(label, fmt, ...) OHOS::HiviewDFX::HiLog::Info(label, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)
#define SC_LOG_WARN(label, fmt, ...) OHOS::HiviewDFX::HiLog::Warn(label, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)
#define SC_LOG_ERROR(label, fmt, ...) \
    OHOS::HiviewDFX::HiLog::Error(label, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)
#define SC_LOG_FATAL(label, fmt, ...) \
    OHOS::HiviewDFX::HiLog::Fatal(label, "[%{public}s]:" fmt, __func__, ##__VA_ARGS__)

#endif  // __cplusplus

#else

#include <cstdio>

#undef LOG_TAG

#define SC_LOG_DEBUG(fmt, ...) printf("[%s] debug: %s: " fmt "\n", LOG_TAG, __func__, ##__VA_ARGS__)
#define SC_LOG_INFO(fmt, ...) printf("[%s] info: %s: " fmt "\n", LOG_TAG, __func__, ##__VA_ARGS__)
#define SC_LOG_WARN(fmt, ...) printf("[%s] warn: %s: " fmt "\n", LOG_TAG, __func__, ##__VA_ARGS__)
#define SC_LOG_ERROR(fmt, ...) printf("[%s] error: %s: " fmt "\n", LOG_TAG, __func__, ##__VA_ARGS__)
#define SC_LOG_FATAL(fmt, ...) printf("[%s] fatal: %s: " fmt "\n", LOG_TAG, __func__, ##__VA_ARGS__)

#endif  // HILOG_ENABLE

#endif  // SECURITY_COMPONENT_LOG_H
