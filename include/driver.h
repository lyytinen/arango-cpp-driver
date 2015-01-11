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

#ifndef ARANGO_CPP_ARANGO_DRIVER_H_
#define ARANGO_CPP_ARANGO_DRIVER_H_

#include "common.h"
#include "configuration.h"
#include "utils/http_utils.h"

#include "entities/database.h"
#include "entities/collection.h"
#include "entities/document.h"
#include "entities/version.h"
using arango::entities::entity;
using arango::entities::database;
using arango::entities::collection;
using arango::entities::document;
using arango::entities::version;

namespace arango {

	/**
	 * Arango DB driver.
	 */
	class driver {
	public:

		explicit driver(const configuration& config) :
			config_(config), client_(to_string_t(config.get_base_url())) {}

		/* Database API : See ArangoDB reference, section 25.2, for documentation. */

		pplx::task<entity> create_database(const std::string& name) {
			return create_database(database(name));
		}

		pplx::task<entity> create_database(const database& db) {
			return http_post<entity>(client_, "_api/database", db.get_value());
		}

		/* Document API : See ArangoDB reference, section 25.2, for documentation. */

		pplx::task<document> get_document(const std::string& collection, std::string key) {
			return http_get<document>(client_, create_location("_api/document/" + collection + " / " + key));
		}

		pplx::task<document> create_document(const std::string& collection, const document& d) {
			return http_post<document>(client_, create_location("_api/document?collection=" + collection), d.get_value());
		}

		/* Collection API : See ArangoDB reference, section 25.8, for documentation. */

		pplx::task<collection> get_collection(const std::string& name) {
			return http_get<collection>(client_, create_location("_api/collection/" + name));
		}

		pplx::task<collection> get_collection_with_properties(const std::string& name) {
			return http_get<collection>(client_, create_location("_api/collection/" + name + "/properties"));
		}

		pplx::task<collection> get_collection_with_count(const std::string& name) {
			return http_get<collection>(client_, create_location("_api/collection/" + name + "/count"));
		}

		pplx::task<collection> create_collection(const std::string& name) {
			return create_collection(collection(name));
		}

		pplx::task<collection> create_collection(const collection& c) {
			return http_post<collection>(client_, create_location("_api/collection"), c.get_value());
		}

		pplx::task<collection> delete_collection(const std::string& name) {
			return http_delete<collection>(client_, create_location("_api/collection/" + name));
		}

		pplx::task<collection> truncate_collection(const std::string& name) {
			return http_put<collection>(client_, create_location("_api/collection/" + name + "/truncate"));
		}

		pplx::task<collection> load_collection(const std::string& name) {
			return http_put<collection>(client_, create_location("_api/collection/" + name + "/load"));
		}

		pplx::task<collection> unload_collection(const std::string& name) {
			return http_put<collection>(client_, create_location("_api/collection/" + name + "/unload"));
		}

		/* Admin API */

		pplx::task<version> driver::get_version() {
			return http_get<version>(client_, "_api/version");
		}

		pplx::task<entity> driver::get_statistics() {
			return http_get<entity>(client_, "_admin/statistics");
		}

		pplx::task<entity> driver::get_statistics_description() {
			return http_get<entity>(client_, "_admin/statistics-description");
		}

	protected:

		std::string create_location(const std::string& location) {
			return "_db/" + config_.get_default_database() + "/" + location;
		}

		configuration config_;

		std::string base_url_;
		
		http_client client_;

	};


}

#endif