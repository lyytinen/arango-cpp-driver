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

#ifndef ARANGO_CPP_ENTITY_H_
#define ARANGO_CPP_ENTITY_H_

#include <boost/optional.hpp>
#include <cpprest/json.h>
#include <cpprest/asyncrt_utils.h>
using utility::conversions::to_string_t;
using utility::conversions::to_utf8string;

#include "common.h"

namespace arango {
	namespace entities {

		static std::string ERROR_FIELD = "error";
		static std::string ERROR_NUM_FIELD = "errorNum";
		static std::string ERROR_MESSAGE_FIELD = "errorMessage";

		/**
		 * Base class for all entity classes.
		 */
		class entity {
		public:

			entity() : value_() {}
			explicit entity(const web::json::value& value) : value_(value) {};

			web::json::value get_value() const {
				return value_;
			}

			web::json::value get_value(const std::string& key) const {
				return value_.at(to_string_t(key));
			}

			std::string get_string(const std::string& key) const {
				return to_utf8string(value_.at(to_string_t(key)).as_string());
			}

			bool get_boolean(const std::string& key) const {
				return value_.at(to_string_t(key)).as_bool();
			}

			int32_t get_int32(const std::string& key) const {
				return value_.at(to_string_t(key)).as_number().to_int32();
			}

			int64_t get_int64(const std::string& key) const {
				return value_.at(to_string_t(key)).as_number().to_int64();
			}

			boost::optional<std::string> get_optional_string(const std::string& key) const {
				return value_.has_field(to_string_t(key)) ?
					boost::optional<std::string>(get_string(key)) :
					boost::none;
			}

			boost::optional<bool> get_optional_boolean(const std::string& key) const {
				return value_.has_field(to_string_t(key)) ?
					boost::optional<bool>(get_boolean(key)) :
					boost::none;
			}

			boost::optional<int32_t> get_optional_int32(const std::string& key) const {
				return value_.has_field(to_string_t(key)) ?
					boost::optional<int32_t>(get_int32(key)) :
					boost::none;
			}

			boost::optional<int64_t> get_optional_int64(const std::string& key) const {
				return value_.has_field(to_string_t(key)) ?
					boost::optional<int64_t>(get_int64(key)) :
					boost::none;
			}

			void put(const std::string& key, const std::string& value) {
				value_[to_string_t(key)] = web::json::value::string(to_string_t(value));
			}

			void put(const std::string& key, int32_t value) {
				value_[to_string_t(key)] = web::json::value::number(value);
			}

			void put(const std::string& key, double value) {
				value_[to_string_t(key)] = web::json::value::number(value);
			}

			void put(const std::string& key, bool value) {
				value_[to_string_t(key)] = web::json::value::boolean(value);
			}

			bool is_error() const {
				if (value_.has_field(to_string_t(ERROR_FIELD))) {
					return get_boolean(ERROR_FIELD);
				}
				else {
					return false;
				}
			}

			void set_error(bool error) {
				put(ERROR_FIELD, error);
			}

			boost::optional<std::string> get_error_num() const {
				return get_optional_string(ERROR_NUM_FIELD);
			}

			void set_error_num(std::string error_num) {
				put(ERROR_NUM_FIELD, error_num);
			}

			boost::optional<std::string> get_error_message() const {
				return get_optional_string(ERROR_MESSAGE_FIELD);
			}

			void set_error_message(std::string error_message) {
				put(ERROR_MESSAGE_FIELD, error_message);
			}

		protected:

			web::json::value value_;

		};

	}
}

#endif