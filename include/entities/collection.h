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

#ifndef ARANGO_CPP_COLLECTION_H_
#define ARANGO_CPP_COLLECTION_H_

#include "entity.h"

namespace arango {

	namespace entities {

		static const std::string COLLECTION_ID_FIELD = "id";
		static const std::string COLLECTION_NAME_FIELD = "name";
		static const std::string COLLECTION_WAIT_FOR_SYNC_FIELD = "waitForSync";
		static const std::string COLLECTION_DO_COMPACT_FIELD = "doCompact";
		static const std::string COLLECTION_JOURNAL_SIZE_FIELD = "journalSize";
		static const std::string COLLECTION_IS_SYSTEM_FIELD = "isSystem";
		static const std::string COLLECTION_IS_VOLATILE_FIELD = "isVolatile";
		static const std::string COLLECTION_KEY_OPTIONS_FIELD = "keyOptions";
		static const std::string COLLECTION_TYPE_FIELD = "type";
		static const std::string COLLECTION_NUMBER_OF_SHARDS_FIELD = "numberOfShards";
		static const std::string COLLECTION_SHARD_KEYS_FIELD = "shardKeys";
		static const std::string COLLECTION_COUNT_FIELD = "count";
		static const std::string COLLECTION_STATUS_FIELD = "status";

		/**
		 * A type of an ArangoDB collection.
		 */
		enum class collection_type {
			COLLECTION = 2,
			EDGE = 3
		};

		/**
		 * A status of an ArangoDB collection.
		 */
		enum class collection_status {
			NEW = 1,
			UNLOADED = 2,
			LOADED = 3,
			UNLOADING = 4,
			DELETED = 5
		};

		/**
		 * An ArangoDB collection. See ArangoDB reference documentation, section 25.8.1, for details.
		 */
		class collection : public entity {
		public:

			collection() {}
			explicit collection(const web::json::value& value) : entity(value) {}

			int64_t get_id() const {
				return get_int64(COLLECTION_ID_FIELD);
			}

			std::string get_name() const {
				return get_string(COLLECTION_NAME_FIELD);
			}

			collection& set_name(const std::string& name) {
				put(COLLECTION_NAME_FIELD, name);
				return *this;
			}

			boost::optional<bool> is_wait_for_sync() const {
				return get_optional_boolean(COLLECTION_WAIT_FOR_SYNC_FIELD);
			}

			collection& set_wait_for_sync(bool wait_for_sync) {
				put(COLLECTION_WAIT_FOR_SYNC_FIELD, wait_for_sync);
				return *this;
			}

			boost::optional<bool> is_do_compact() const {
				return get_optional_boolean(COLLECTION_DO_COMPACT_FIELD);
			}

			collection& set_do_compact(bool do_compact) {
				put(COLLECTION_DO_COMPACT_FIELD, do_compact);
				return *this;
			}

			boost::optional<int32_t> get_journal_size() const {
				return get_optional_int32(COLLECTION_JOURNAL_SIZE_FIELD);
			}

			collection& set_journal_size(int32_t journal_size) {
				put(COLLECTION_JOURNAL_SIZE_FIELD, journal_size);
				return *this;
			}

			boost::optional<bool> is_system() const {
				return get_optional_boolean(COLLECTION_IS_SYSTEM_FIELD);
			}

			collection& set_system(bool system) {
				put(COLLECTION_IS_SYSTEM_FIELD, system);
				return *this;
			}

			boost::optional<bool> is_volatile() const {
				return get_optional_boolean(COLLECTION_IS_VOLATILE_FIELD);
			}

			collection& set_volatile(bool is_volatile) {
				put(COLLECTION_IS_VOLATILE_FIELD, is_volatile);
				return *this;
			}

			boost::optional<collection_type> get_type() {
				return value_.has_field(to_string_t(COLLECTION_TYPE_FIELD)) ?
					boost::optional<collection_type>(static_cast<collection_type>(get_int32(COLLECTION_TYPE_FIELD))) :
					boost::none;
			}

			collection& set_type(collection_type type) {
				put(COLLECTION_TYPE_FIELD, static_cast<int32_t>(type));
				return *this;
			}

			boost::optional<int> get_number_of_shards() const {
				return get_optional_int32(COLLECTION_NUMBER_OF_SHARDS_FIELD);
			}

			collection& set_number_of_shards(int number_of_shards) {
				put(COLLECTION_NUMBER_OF_SHARDS_FIELD, number_of_shards);
				return *this;
			}

			boost::optional<std::vector<std::string>> get_shard_keys() {
				utility::string_t key = to_string_t(COLLECTION_SHARD_KEYS_FIELD);
				if (value_.has_field(key)) {
					if (!value_.at(key).is_array()) {
						throw std::runtime_error("Unexpected value for '" + COLLECTION_SHARD_KEYS_FIELD + "'.");
					}
					std::vector<std::string> shard_keys;
					shard_keys.resize(value_.at(key).size());
					std::transform(
						value_.at(key).as_array().begin(),
						value_.at(key).as_array().end(),
						shard_keys.begin(),
						[](const web::json::value& val) { return FROM_CBSTR(val.as_string()); });
				} else {
					return boost::none;
				}				
			}

			collection& set_shard_keys(const std::vector<std::string>& shard_keys) {
				web::json::value shard_key_array;
				for (unsigned int i = 0; i < shard_keys.size(); ++i) {
					shard_key_array[i] = web::json::value::string(to_string_t(shard_keys.at(i)));
				}
				value_[to_string_t(COLLECTION_SHARD_KEYS_FIELD)] = shard_key_array;
				return *this;
			}

			boost::optional<int32_t> get_count() const {
				return get_optional_int32(COLLECTION_COUNT_FIELD);
			}

			boost::optional<collection_status> get_status() {
				return value_.has_field(to_string_t(COLLECTION_STATUS_FIELD)) ?
					boost::optional<collection_status>(static_cast<collection_status>(get_int32(COLLECTION_STATUS_FIELD))) :
					boost::none;
			}
			
		};

	}

}

#endif