#include <iostream>
using std::string;

#include <ppl.h>
#include <pplx/pplxtasks.h>
#include <cpprest/http_client.h>
#include <cpprest/asyncrt_utils.h>
#include <boost/lexical_cast.hpp>

using web::http::uri;
using web::http::methods;
using web::http::client::http_client;
using web::http::http_request;
using web::http::http_response;
using web::http::http_pipeline_stage;
using web::http::http_exception;
using concurrency::parallel_for_each;

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

void initialize_database(driver& arango_client);
void populate_database(driver& arango_client, http_client& ncdc_client);
void die(const string& error_message);

/**
 * The main entry-point.
 *
 * The application creates a new ArangoDB database and populates it by loading some
 * weather observation data from the NCDC web service. To access the service you
 * need to request for an access token from their website and pass it as an argument
 * for this application:
 *
 * http://www.ncdc.noaa.gov/cdo-web/webservices/v2
 *
 */
int main(int argc, char* argv[]) {

  if (argc != 2) {
    die("Usage: " + string(argv[0]) + " <ncdc_access_token>");
  }

  // (1) Create a new ArangoDB driver instance and programmatically
  // initialize our database.

  configuration config =
    configuration()
    .set_host("localhost")
    .set_port("8529")
    .set_default_database("weather");

  driver arango_client(config);

  initialize_database(arango_client);

  // (2) Create a HTTP client for accessing the NCDC web service and
  // populate our database with weather data.

  http_client ncdc_client(to_string_t("http://www.ncdc.noaa.gov"));
  ncdc_client.add_handler([&argv](http_request request, std::shared_ptr<http_pipeline_stage> next_stage) -> pplx::task < http_response > {
    request.headers().add(to_string_t("token"), to_string_t(argv[1]));
    return next_stage->propagate(request);
  });

  populate_database(arango_client, ncdc_client);

  return 0;
}

/**
 * Initializes our weather database.
 *
 * The function first creates a new database called 'weather'. If the database already
 * exists, ArandoDB will send a response with error code 1207 (dublicate name).
 *
 * Once the weather database is up and running, the function then creates a new collection
 * for our weather data called 'snowfall'. Again, if the collection already exists,
 * ArangoDB will send a response with error code 1207 (dublicate name). In this case, we
 * will truncate the existing collection.
 */
void initialize_database(driver& arango_client) {

  arango_client.create_database("weather").then([](const entity& result) {
    if (result.is_error() && result.get_error_num().get() != 1207) {
      die("Failed to create 'weather' database: " + result.get_error_message().get());
    }
    else {
      std::cout << "Created 'weather' database." << std::endl;
    }
  }).wait();

 arango_client.create_collection("snowfall").then([&arango_client](const collection& result) {
    if (result.is_error()) {
      if (result.get_error_num().get() != 1207) {
        die("Failed to create 'snowfall' collection: " + result.get_error_message().get());
      }
      else {
        arango_client.truncate_collection("snowfall").then([](const collection& result) {
          if (result.is_error()) {
            die("Failed to truncate 'snowfall' collection: " + result.get_error_message().get());
          }
          else {
            std::cout << "Truncated existing 'snowfall' collection." << std::endl;
          }
        }).wait();
      }
    }
    else {
      std::cout << "Created 'snowfall' collection." << std::endl;
    }
  }).wait();
}

/**
 * Populates the database by extracting weather data from the NCDC web service.
 *
 * In this case we want to know how much it snows in Alaska and extract daily snowfall
 * data for the Anchorage international airport between for the year 2014. 
 */
void populate_database(driver& arango_client, http_client& ncdc_client) {

  std::string query_string =
    query_string_builder()
    .append("datasetid", "GHCND")
    .append("datatypeid", "SNOW")
    .append("stationid", "GHCND:USW00026451")
    .append("startdate", "2014-01-01")
    .append("enddate", "2014-12-31")
    .append("limit", "1000")
    .build();

  http_get<entity>(ncdc_client, "cdo-web/api/v2/data", query_string).then([&](const entity& data) {

    // Transform NCDC data array in to distinct measurement documents
    std::vector<document> measurements;
    web::json::array ncdc_fragments = data.get_value("results").as_array();
    std::for_each(ncdc_fragments.begin(), ncdc_fragments.end(), [&](const web::json::value& fragment) {
      entity ncdc_fragment(fragment);
      document measurement;
      measurement.put("location", ncdc_fragment.get_string("station"));
      measurement.put("date", ncdc_fragment.get_string("date"));
      measurement.put("snowfall", ncdc_fragment.get_int32("value"));      
      measurements.push_back(measurement);
    });

    // Populate the measurement collection with the snowfall data
    parallel_for_each(measurements.begin(), measurements.end(), [&](const document& m) {
      arango_client.create_document("snowfall", m).then([](const document& inserted) {
        if (inserted.is_error()) {
          die("Failed to populate the 'snowfall' collection: " + inserted.get_error_message().get());
        }
      }).wait();
    });

    std::cout << 
      "Populated the 'snowfall' collection with " + 
        boost::lexical_cast<string>(measurements.size()) + " measurements." << std::endl;

  }).wait();
}

/**
 * Prints out an error message and terminates the process.
 */
void die(const string& error_message) {
  std::cerr << error_message << std::endl;
  std::exit(-1);
}