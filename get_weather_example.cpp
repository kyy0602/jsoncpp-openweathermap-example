#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
using namespace std;

size_t function_ptr(void *ptr, size_t size, size_t nmemb, string *s) {
  s->append(static_cast<char *>(ptr), size*nmemb);
  return size*nmemb;
}

int main() {
  string weather;
  CURL *curl;
  CURLcode res;

  //request openweathermap data
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/weather?q={place}&appid={api_key}&units=metric");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, function_ptr);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &weather);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  }

  //read returned json results
  Json::Value root;
  Json::Reader reader;
  bool parseSuccess = reader.parse(weather, root, false);
  cout << weather << endl;
  if(parseSuccess) {
    const Json::Value tempInfo = root["main"];
    const Json::Value temp = tempInfo["temp"].asFloat();
    cout << "the temperature in {place} today is " << temp.asInt() << "°C" << endl;
  }

  return 0;
}
