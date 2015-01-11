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

#ifndef ARANGO_CPP_DATABASE_H_
#define ARANGO_CPP_DATABASE_H_

#include "entity.h"

namespace arango {

	namespace entities {

		static const std::string DATABASE_NAME_FIELD = "name";

		/**
		 * An ArangoDb database.
		 */
		class database : public entity {
		public: 

			database() {}
			explicit database(const web::json::value& value) : entity(value) {}
			explicit database(const std::string& name) {
				put(DATABASE_NAME_FIELD, name);
			}

			std::string get_name() const {
				return get_string(DATABASE_NAME_FIELD);
			}

			void set_name(const std::string& name) {
				put(DATABASE_NAME_FIELD, name);
			}

		};

	}

}

#endif