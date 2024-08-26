#include <iostream>
#include <fmt/core.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

#include <config.hxx>
#include <Secret.hxx>

int main() {
   Secret s{ std::string(SecretFilename) };

   if (!s.is_good()) {
      std::cout << "Enable to read secret file." << std::endl;
      return -1;
   }

   //read settings file number update
   std::ifstream read_settings_file("settings.json");
   nlohmann::json read_settings;
   int64_t last_offset_num = -1;
   if (read_settings_file.is_open() && read_settings_file.good()) {
      read_settings_file >> read_settings;
      if (read_settings["last_offset"].is_number()) {
         last_offset_num = (read_settings["last_offset"].get<int64_t>() + 1);
         std::cout << "Last offset from settings file: " << last_offset_num << std::endl;
      }
   }

   std::stringstream base_url;
   base_url << "http://api.telegram.org/" << s.get_secret();

   // Выполнить GET-запрос до публичного API
   std::stringstream url;
   url << base_url.str() << "/getUpdates";
   if (last_offset_num != -1) {
      url << "?offset=" << last_offset_num;
   }

   cpr::Response r = cpr::Get(cpr::Url{ url.str() });

    nlohmann::json jsonResponse = nlohmann::json::parse(r.text);
    std::cout << "Result" << std::setw(2)<< jsonResponse["result"] << std::endl;

    auto result_size = jsonResponse["result"].size();
    if (!result_size) {
       std::cout << "There has been no updates on the bot!" << std::endl;
       return 0;
    }

    for (auto& user_message : jsonResponse["result"]) {
       std::cout << "!!MESSAGE!!" << std::endl;
;
       int64_t chat_id = user_message["message"]["chat"]["id"].get<int>();
       std::string user_text = user_message["message"]["text"].get<std::string>();

       if (!user_text.starts_with("/echo ")) {        //если начинается на echo
          continue;
       }
       user_text.erase(0, strlen("/echo "));          //стереть строку

       cpr::Payload p{
          {"chat_id", std::to_string(chat_id)},
          {"text", user_text}
       };

       std::cout << "Chat ID? => " << chat_id << std::endl;

       cpr::Response postR = cpr::Post(cpr::Url{ base_url.str(), "/sendMessage" }, p);
       std::cout << "Post status code? => " << postR.status_code << std::endl;

    }

    //check the last update number and write it in settings file
    auto last_offset = jsonResponse["result"][result_size - 1]["update_id"].get<int64_t>();
    nlohmann::json settings;
    settings["last_offset"] = last_offset;
    std::cout << "last ID: " << last_offset << std::endl;
    std::ofstream settings_file("settings.json", std::ios::trunc);
    settings_file << settings;
    settings_file.close();

    return 0;
}