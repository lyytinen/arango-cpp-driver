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

#ifndef ARANGO_CPP_HTTP_UTILS_H_
#define ARANGO_CPP_HTTP_UTILS_H_

#include <pplx/pplxtasks.h>
#include <cpprest/http_client.h>
#include <cpprest/asyncrt_utils.h>

using web::http::uri;
using web::http::methods;
using web::http::http_response;
using web::http::http_exception;
using web::http::client::http_client;
using utility::conversions::to_string_t;
using utility::conversions::to_utf8string;

namespace arango {

	template<typename T>
	pplx::task<T> process_response(pplx::task<http_response> response_task) {
		try {
			http_response response = response_task.get();
			pplx::task<web::json::value> extract_json_task = response.extract_json();
			return extract_json_task.then([](web::json::value payload) {
				return T(payload);
			});
		}
		catch (http_exception& e) {
			T error;
			error.set_error(true);
			error.set_error_num(0);
			error.set_error_message(std::string(e.what()));
			return pplx::task_from_result(error);
		}		
	}

	template<typename T>
	pplx::task<T> http_get(http_client& client, const std::string& location) {
		return client.request(methods::GET, to_string_t(location))
			.then([](pplx::task<http_response> response_task) { return process_response<T>(response_task); });
	}

	template<typename T>
	pplx::task<T> http_get(http_client& client, const std::string& location, const std::string& query_string) {
		return client.request(methods::GET, to_string_t(location + "?" + query_string))
			.then([](pplx::task<http_response> response_task) { return process_response<T>(response_task); });
	}

	template<typename T>
	pplx::task<T> http_put(http_client& client, const std::string& location) {
		return client.request(methods::PUT, to_string_t(location))
			.then([](pplx::task<http_response> response_task) { return process_response<T>(response_task); });
	}

	template<typename T>
	pplx::task<T> http_post(http_client& client, const std::string& location, const web::json::value& payload) {
		return client.request(methods::POST, to_string_t(location), payload)
			.then([](pplx::task<http_response> response_task) { return process_response<T>(response_task); });
	}

	template<typename T>
	pplx::task<T> http_delete(http_client& client, const std::string& location) {
		return client.request(methods::DEL, to_string_t(location))
			.then([](pplx::task<http_response> response_task) { return process_response<T>(response_task); });
	}

}

#endif
