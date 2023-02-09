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
#ifndef SECURITY_COMPONENT_PATTERN_H
#define SECURITY_COMPONENT_PATTERN_H

static constexpr int32_t MIN_FONT_SIZE = 10;
enum SecCompType {
    UNKNOWN_SC_TYPE = 0,
    LOCATION_COMPONENT,
    PASTE_COMPONENT,
    SAVE_COMPONENT,
    MAX_SC_TYPE
};

enum SecCompFontFamily {
    UNKNOWN_FONT_FAMILY_TYPE = 0,
    FONT_HO_SANS, // default font family
    MAX_FONT_FAMILY_TYPE
};

enum SecCompLabel {
    UNKNOWN_LABEL = 0,
    DEFAULT_LABEL,
    MAX_LABEL_TYPE
};

enum SecCompIcon {
    UNKNOWN_ICON = 0,
    DEFAULT_ICON,
    MAX_ICON_TYPE
};

union SecCompColor {
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    } argb;
    uint32_t value;
};

enum SecCompButtonType {
    UNKNOWN_BUTTON_TYPE = 0,
    CAPSULE,
    CIRCLE,
    NORMAL,
    MAX_BUTTON_TYPE
};

static inline bool IsComponentTypeValid(int32_t type)
{
    return type > UNKNOWN_SC_TYPE && type < MAX_SC_TYPE;
}

static inline bool IsComponentFontFamilyValid(int32_t font)
{
    return font > UNKNOWN_FONT_FAMILY_TYPE && font < MAX_FONT_FAMILY_TYPE;
}

static inline bool IsComponentLabelValid(int32_t label)
{
    return label > UNKNOWN_LABEL && label < MAX_LABEL_TYPE;
}

static inline bool IsComponentIconValid(int32_t icon)
{
    return icon > UNKNOWN_ICON && icon < MAX_ICON_TYPE;
}

static inline bool IsComponentButtonTypeValid(int32_t buttonType)
{
    return buttonType > UNKNOWN_BUTTON_TYPE && buttonType < MAX_BUTTON_TYPE;
}

static inline bool IsComponentFontSizeValid(int32_t fontSize)
{
    return fontSize > MIN_FONT_SIZE;
}
#endif // SECURITY_COMPONENT_PATTERN_H
