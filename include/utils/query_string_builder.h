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

#ifndef ARANGO_CPP_QUERY_STRING_BUILDER_H_
#define ARANGO_CPP_QUERY_STRING_BUILDER_H_

#include "common.h"
#include "http_utils.h"

namespace arango {

	/**
	 * A helper class for constructing query strings.
	 */
	class query_string_builder {
	public:

		query_string_builder& append(const std::string& key, const std::string& value) {
			if (is_set(key)) {
				parameters_[key].push_back(value);
			}
			else {
				parameters_[key] = std::vector<std::string>(1, value);
			}
			return *this;
		}

		std::string build() const {
			std::string query_string;
			for (auto i = parameters_.begin(); i != parameters_.end(); i++) {
				std::string key = url_encode(i->first);
				for (auto j = i->second.begin(); j != i->second.end(); j++) {
					if (!query_string.empty()) {
						query_string += "&";
					}
					query_string += key + "=" + url_encode(*j);
				}
			}
			return query_string;
		}

	private:

		std::string url_encode(const std::string& value) const {
			return to_utf8string(uri::encode_uri(to_string_t(value)));
		}

		bool is_set(const std::string& key) {
			return parameters_.find(key) != parameters_.end();
		}

		std::map<std::string, std::vector<std::string>> parameters_;

	};
}

#endif