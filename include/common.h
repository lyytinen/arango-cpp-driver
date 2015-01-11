/*
 * Copyright (C) 2015 Jussi Lyytinen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ARANGO_CPP_COMMON_H_
#define ARANGO_CPP_COMMON_H_

#include <string>
#include <memory>
#include <map>
#include <vector>

#ifdef _WIN32
#define _UTF16_STRINGS
#endif

#ifdef _UTF16_STRINGS
#define TO_CBSTR(value) utility::conversions::to_string_t(value)
#else
#define TO_CBSTR(value) value;
#endif

#ifdef _UTF16_STRINGS
#define FROM_CBSTR(value) utility::conversions::to_utf8string(value);
#else
#define FROM_CBSTR(value) value
#endif

#endif