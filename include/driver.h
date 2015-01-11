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

#include "entity/version.h"
#include "entity/collection.h"
#include "entity/document.h"
using arango::entity::entity;
using arango::entity::version;
using arango::entity::collection;
using arango::entity::document;

namespace arango {

	/**
	 * Arango DB driver.
	 */
	class driver {
	public:

		explicit driver(const configuration& config) :
			config_(config), client_(to_string_t(config.get_base_url())) {}

		/* Document API : See ArangoDB reference, section 25.2, for documentation. */

		pplx::task<document> get_document(const std::string& collection, std::string key) {
			return http_get<document>(client_, "_api/document/" + collection + " / " + key);
		}

		pplx::task<document> create_document(const std::string& collection, const document& d) {
			return http_post<document>(client_, "_api/document?collection=" + collection, d.get_value());
		}

		/* Collection API : See ArangoDB reference, section 25.8, for documentation. */

		pplx::task<collection> get_collection(const std::string& name) {
			return http_get<collection>(client_, "_api/collection/" + name);
		}

		pplx::task<collection> get_collection_with_properties(const std::string& name) {
			return http_get<collection>(client_, "_api/collection/" + name + "/properties");
		}

		pplx::task<collection> get_collection_with_count(const std::string& name) {
			return http_get<collection>(client_, "_api/collection/" + name + "/count");
		}

		pplx::task<collection> create_collection(const collection& c) {
			return http_post<collection>(client_, "_api/collection", c.get_value());
		}

		pplx::task<collection> delete_collection(const std::string& name) {
			return http_delete<collection>(client_, "_api/collection/" + name);
		}

		pplx::task<collection> truncate_collection(const std::string& name) {
			return http_put<collection>(client_, "_api/collection/" + name + "/truncate");
		}

		pplx::task<collection> load_collection(const std::string& name) {
			return http_put<collection>(client_, "_api/collection/" + name + "/load");
		}

		pplx::task<collection> unload_collection(const std::string& name) {
			return http_put<collection>(client_, "_api/collection/" + name + "/unload");
		}

		/* Admin API */

		pplx::task<version> driver::get_version() {
			return http_get<version>(client_, "_api/version");
		}

		pplx::task<entity::entity> driver::get_statistics() {
			return http_get<entity::entity>(client_, "_admin/statistics");
		}

		pplx::task<entity::entity> driver::get_statistics_description() {
			return http_get<entity::entity>(client_, "_admin/statistics-description");
		}

	protected:

		configuration config_;

		std::string base_url_;
		
		http_client client_;

	};


}

#endif