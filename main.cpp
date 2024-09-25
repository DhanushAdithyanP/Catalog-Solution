#include <iostream>
#include <fstream>
#include <vector>
#include "picojson.h"

long long decode_value(const std::string& value, int base) {
    return std::stoll(value, nullptr, base);
}

double lagrange_interpolation(const std::vector<long long>& x_values, const std::vector<long long>& y_values, int k) {
    double c = 0;

    for (int i = 0; i < k; i++) {
        double term = y_values[i];
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0 - x_values[j]) / (double)(x_values[i] - x_values[j]);
            }
        }
        c += term;
    }

    return c;
}

int main() {
    std::ifstream input_file("testcase.json");
    std::string json_str((std::istreambuf_iterator<char>(input_file)),
                         std::istreambuf_iterator<char>());

    picojson::value data;
    std::string err = picojson::parse(data, json_str);
    if (!err.empty()) {
        std::cerr << "JSON parsing error: " << err << std::endl;
        return 1;
    }

    int n = static_cast<int>(data.get("keys").get<picojson::object>()["n"].get<double>());
    int k = static_cast<int>(data.get("keys").get<picojson::object>()["k"].get<double>());

    std::vector<long long> x_values;
    std::vector<long long> y_values;

    const picojson::object& obj = data.get<picojson::object>();
    for (auto& el : obj) {
        if (el.first == "keys") continue;

        int x = std::stoi(el.first); 

        picojson::object el_obj = el.second.get<picojson::object>();
        std::string base_str = el_obj["base"].get<std::string>();
        std::string value_str = el_obj["value"].get<std::string>();

        int base = std::stoi(base_str); 
        long long y = decode_value(value_str, base); 

        x_values.push_back(x);
        y_values.push_back(y);
    }

    std::cout << "x_values: ";
    for (const auto& x : x_values) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::cout << "y_values: ";
    for (const auto& y : y_values) {
        std::cout << y << " ";
    }
    std::cout << std::endl;

    double c = lagrange_interpolation(x_values, y_values, k);

    std::cout << "The constant term 'c' is: " << c << std::endl;

    return 0;
}