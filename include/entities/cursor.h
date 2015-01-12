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

#ifndef ARANGO_CPP_CURSOR_H
#define ARANGO_CPP_CURSOR_H

#include "common.h"
#include "entity.h"
#include "document.h"

namespace arango {

  namespace entities {

    static const std::string CURSOR_ID_FIELD = "id";
    static const std::string CURSOR_HAS_MORE_FIELD = "id";
    static const std::string CURSOR_COUNT_FIELD = "id";
    static const std::string CURSOR_RESULT_FIELD = "result";

    /**
     * An ArangoDB cursor.
     */
    class cursor : public entity {
    public:

      cursor() {}
      explicit cursor(const web::json::value& value) : entity(value) {}

      std::string get_id() const {
        return get_string(CURSOR_ID_FIELD);
      }

      bool has_more() const {
        return get_boolean(CURSOR_HAS_MORE_FIELD);
      }

      uint32_t get_count() const {
        return get_int32(CURSOR_COUNT_FIELD);
      }

      std::vector<document> get_result() const {
        utility::string_t key = to_string_t(CURSOR_RESULT_FIELD);
        std::vector<document> documents;
        documents.resize(value_.at(key).size());
        std::transform(
          value_.at(key).as_array().begin(),
          value_.at(key).as_array().end(),
          documents.begin(),
          [](const web::json::value& val) { return document(val); });
        return documents;
      }

    };

  }

}

#endif