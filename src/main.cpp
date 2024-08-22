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

    std::string base_url = (std::stringstream() << "http://api.telegram.org/" << s.get_secret()).str();
    cpr::Response r = cpr::Get(cpr::Url{ base_url, "/getUpdates?offset=217951020" });

    nlohmann::json jsonResponse = nlohmann::json::parse(r.text);
    //std::cout << "Result" << std::setw(2)<< jsonResponse["result"] << std::endl;

    if (!jsonResponse["result"].size()) {
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
       user_text.erase(0, strlen("\echo "));          //стереть строку

       cpr::Payload p{
          {"chat_id", std::to_string(chat_id)},
          {"text", user_text}
       };

       cpr::Response postR = cpr::Post(cpr::Url{ base_url, "/sendMessage" }, p);
       std::cout << "Post status code? => " << postR.status_code << std::endl;

    }

    return 0;
}