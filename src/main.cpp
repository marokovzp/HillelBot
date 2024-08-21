#include <iostream>
#include <fmt/core.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

int main() {
   std::ifstream secret_file("secret.json");
   //std::cout << "Good? => " << (secret_file.good() && secret_file.is_open()) << std::endl;
   bool good = (secret_file.good() && secret_file.is_open());

   if (!good) {
      std::cout << "Enable to read secret file." << std::endl;
      return -1;
   }

   nlohmann::json secret_json;
   secret_file >> secret_json;
   auto secret = secret_json["key"].get<std::string>();
   //std::cout << "secret key: " << secret << std::endl;
   secret_file.close();

	//std::cout << "Hello Bot" << std::endl;
	//fmt::print("Hello fmt!\n");

    // Виконати GET-запит до публічного API
    std::string base_url = (std::stringstream() << "http://api.telegram.org/" << secret).str();
    cpr::Response r = cpr::Get(cpr::Url{ base_url, "/getUpdates?offset=217951020" });

    // Вивести статус код відповіді
    //std::cout << "Status Code: " << r.status_code << std::endl;
    //std::cout << "Full responce: " << r.text << std::endl;

    // Парсити JSON відповідь
    nlohmann::json jsonResponse = nlohmann::json::parse(r.text);
    std::cout << "Result" << std::setw(2)<< jsonResponse["result"] << std::endl;

    if (!jsonResponse["result"].size()) {
       std::cout << "There has been no updates on the bot!" << std::endl;
       return 0;
    }

    // Вивести певне поле з JSON відповіді
    //std::cout << "===JSON===\n " << std::setw(2) << jsonResponse << std::endl;

    for (auto& user_message : jsonResponse["result"]) {
       //std::cout << "Count: " << jsonResponse.size() << std::endl;
       std::cout << "!!MESSAGE!!" << std::endl;

       //int chat_id = jsonResponse["result"][0]["message"]["chat"]["id"].get<int>();
       int64_t chat_id = user_message["message"]["chat"]["id"].get<int>();
       std::string user_text = user_message["message"]["text"].get<std::string>();

       cpr::Payload p{
          {"chat_id", std::to_string(chat_id)},
          {"text", user_text}
       };
       //std::cout << "Chat ID = " << chat_id << std::endl;

       cpr::Response postR = cpr::Post(cpr::Url{ base_url, "/sendMessage" }, p);
       std::cout << "Post status code? => " << postR.status_code << std::endl;

       //nlohmann::json postResponse = nlohmann::json::parse(postR.text);
       //std::cout << "Post text => " << std::setw(2) << postResponse << std::endl;
    }

    return 0;
}