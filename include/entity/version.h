#ifndef ARANGO_CPP_VERSION_H_
#define ARANGO_CPP_VERSION_H_

#include "common.h"
#include "entity.h"

namespace arango {
	namespace entity {

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