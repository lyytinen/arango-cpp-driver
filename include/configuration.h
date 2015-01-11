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

#ifndef ARANGO_CPP_CONFIGURATION_H_
#define ARANGO_CPP_CONFIGURATION_H_

#include "common.h"

namespace arango {

  class configuration {
  public:

    std::string get_host() const {
      return host_;
    }

    configuration& set_host(const std::string& host) {
      host_ = host;
      return *this;
    }

    std::string get_port() const {
      return port_;
    }

    configuration& set_port(const std::string& port) {
      port_ = port;
      return *this;
    }

    configuration& set_default_database(const std::string& database) {
      default_database_ = database;
      return *this;
    }

    std::string get_default_database() const {
      return default_database_;
    }

    std::string get_base_url() const {
      return "http://" + get_host() + ":" + get_port() + "/";
    }

  private:

    std::string host_;
    std::string port_;
    std::string default_database_;

  };

}

#endif