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
 */

#ifndef ARANGO_CPP_VERSION_H_
#define ARANGO_CPP_VERSION_H_

#include "common.h"
#include "entity.h"

namespace arango {
  namespace entities {

    static const std::string VERSION_FIELD = "version";
    static const std::string SERVER_FIELD = "server";

    /**
     * Entity class for the API version.
     */
    class version : public entity {
    public:

      version() {};
      explicit version(const web::json::value& value) : entity(value) {}

      std::string get_version() const {
        return get_string(VERSION_FIELD);
      }

      std::string get_server() const {
        return get_string(SERVER_FIELD);
      };

    };

  }
}

#endif