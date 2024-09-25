#include <iostream>  
#include <curl/curl.h>  
#include "json.hpp"
#include <nlohmann/json.hpp>  

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    buffer->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getWeatherData(const std::string& apiKey, const std::string& city) {
    CURL* curl;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string apiUrl = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

void displayWeatherData(const std::string& weatherData) {
    nlohmann::json jsonData = nlohmann::json::parse(weatherData);

    std::string cityName = jsonData["name"];
    double temperature = jsonData["main"]["temp"];
    double humidity = jsonData["main"]["humidity"];
    std::string weatherDescription = jsonData["weather"][0]["description"];

    std::cout << "Weather in " << cityName << ":\n";
    std::cout << "Temperature: " << temperature << "°C\n";
    std::cout << "Humidity: " << humidity << "%\n";
    std::cout << "Condition: " << weatherDescription << "\n";
}

int main() {
    std::string apiKey = "YOUR_API_KEY";   //Replace with API key from OpenWeatherMap
    std::string city;

    std::cout << "Enter the name of the city: ";
    std::getline(std::cin, city);

    std::string weatherData = getWeatherData(apiKey, city);

    if (!weatherData.empty()) {
        displayWeatherData(weatherData);
    } else {
        std::cout << "Error fetching weather data.\n";
    }

    return 0;
}