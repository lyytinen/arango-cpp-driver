#include <iostream>

#include <ppl.h>
#include <pplx/pplxtasks.h>
#include <cpprest/http_client.h>
#include <cpprest/asyncrt_utils.h>

using web::http::uri;
using web::http::methods;
using web::http::client::http_client;
using web::http::http_request;
using web::http::http_response;
using web::http::http_pipeline_stage;
using web::http::http_exception;
using concurrency::parallel_for_each;

#include <boost/lexical_cast.hpp>

#include "configuration.h"
#include "driver.h"
#include "utils/http_utils.h"
#include "utils/query_string_builder.h"

using arango::configuration;
using arango::driver;
using arango::query_string_builder;
using arango::http_get;
using arango::entities::document;
using arango::entities::version;
using arango::entities::entity;

/**
 * Transforms the NCDC fragment into a measurement document.
 *
 * @param fragment the NCDC fragment
 * @return a measurement document
 */
document transform(const web::json::value& fragment) {
	entity ncdc_fragment(fragment);
	document measurement;
	measurement.put("location", ncdc_fragment.get_string("station"));
	measurement.put("date", ncdc_fragment.get_string("date"));
	measurement.put("snowfall", ncdc_fragment.get_int32("value"));
	return measurement;
}

/**
 * A main entry-point for testing purposes.
 */
int main(int argc, char* argv[]) {

	http_client weather_client(U("http://www.ncdc.noaa.gov"));
	weather_client.add_handler([](http_request request, std::shared_ptr<http_pipeline_stage> next_stage) -> pplx::task<http_response> {
		request.headers().add(U("token"), U("RdWQWpxaJborDiZxvWtdiziCHskDiKyP"));
		return next_stage->propagate(request);
	});

	configuration config =
		configuration()
		.set_host("localhost")
		.set_port("8529")
		.set_default_database("_system");

	driver arango_client(config);

	std::string query_string =
		query_string_builder()
			.append("datasetid", "GHCND")
			.append("datatypeid", "SNOW")
			.append("stationid", "GHCND:USW00026451")
			.append("startdate", "2014-01-01")
			.append("enddate", "2014-01-02")
			.append("limit", "1000")
			.build();

	http_get<entity>(weather_client, "cdo-web/api/v2/data", query_string).then([&] (const entity& data) {
		
		// Transform NCDC data array in to distinct measurement documents
		std::vector<document> measurements;
		web::json::array ncdc_fragments = data.get_value("results").as_array();
		std::for_each(ncdc_fragments.begin(), ncdc_fragments.end(), [&](const web::json::value& fragment) {
			measurements.push_back(transform(fragment));
		});

		// Populate the measurement collection with the snowfall data
		parallel_for_each(measurements.begin(), measurements.end(), [&](const document& m) {
			arango_client.create_document("snowfall", m).then([](const document& inserted) {
				if (inserted.is_error()) {
					std::cerr << "Failed to populate database: " + inserted.get_error_message().get() << std::endl;
					std::exit(-1);
				}
			}).wait();
		});

	}).wait();

	return 0;
}
