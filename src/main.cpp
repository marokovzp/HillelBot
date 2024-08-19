#include <iostream>
#include <fmt/core.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

int main() {
	std::cout << "Hello Bot" << std::endl;
	fmt::print("Hello fmt!\n");

    // Виконати GET-запит до публічного API
    cpr::Response r = cpr::Get(cpr::Url{"http://echo.jsontest.com/key/value/12one/t2wo"});

    // Вивести статус код відповіді
    std::cout << "Status Code: " << r.status_code << std::endl;
    std::cout << "Full responce: " << r.text << std::endl;

    // Парсити JSON відповідь
    nlohmann::json jsonResponse = nlohmann::json::parse(r.text);

    // Вивести певне поле з JSON відповіді
    std::cout << "Title: " << jsonResponse["12one"] << std::endl;

    return 0;
}