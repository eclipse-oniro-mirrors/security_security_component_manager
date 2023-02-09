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
#ifndef SECURITY_COMPONENT_RECT_H
#define SECURITY_COMPONENT_RECT_H
namespace OHOS {
namespace Security {
namespace SecurityComponent {
inline bool GreatOrEqual(double left, double right)
{
    constexpr double epsilon = -0.001f;
    return (left - right) > epsilon;
}

inline bool GreatNotEqual(double left, double right)
{
    constexpr double epsilon = 0.001f;
    return (left - right) > epsilon;
}

class SecCompRect {
public:
    SecCompRect(double x, double y, double width, double height) : x_(x), y_(y), width_(width), height_(height) {};
    SecCompRect() = default;
    ~SecCompRect() = default;

    bool IsValid() const;
    bool IsRectOverlaped(const SecCompRect& other) const;
    bool IsInRect(double x, double y) const;

    double x_ = 0.0;
    double y_ = 0.0;
    double width_ = 0.0;
    double height_ = 0.0;
};
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
#endif  // SECURITY_COMPONENT_INFO_H
