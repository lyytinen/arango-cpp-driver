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

#ifndef ARANGO_CPP_DOCUMENT_H_
#define ARANGO_CPP_DOCUMENT_H_

#include "entity.h"

namespace arango {

	namespace entities {

		static const std::string DOCUMENT_HANDLE_FIELD = "_id";
		static const std::string DOCUMENT_KEY_FIELD = "_key";
		static const std::string DOCUMENT_REVISION_FIELD = "_rev";

		/**
		 * An ArangoDB document.
		 */
		class document : public entity {
		public:

			document() {}
			explicit document(const web::json::value& value) : entity(value) {}

			std::string get_id() const {
				return get_string(DOCUMENT_HANDLE_FIELD);
			}

			std::string get_key() const {
				return get_string(DOCUMENT_KEY_FIELD);
			}

			std::string get_revision() const {
				return get_string(DOCUMENT_REVISION_FIELD);
			}

		};

	}

}

#endif