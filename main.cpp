#include <iostream>
#include <string>
#include <map>
#include <curl/curl.h>

std::map<std::string, double> x;

size_t w(void* a, size_t b, size_t c, std::string* d) {
    d->append((char*)a, b * c);
    return b * c;
}

void g() {
    CURL* e = curl_easy_init();
    if (e) {
        std::string f;
        curl_easy_setopt(e, CURLOPT_URL, "https://v6.exchangerate-api.com/v6/a569ce69d7a11609634e07bd/latest/USD");
        curl_easy_setopt(e, CURLOPT_WRITEFUNCTION, w);
        curl_easy_setopt(e, CURLOPT_WRITEDATA, &f);
        curl_easy_perform(e);

        size_t p = f.find("USD\":");
        if (p != std::string::npos) {
            x["USD"] = 1.0;
            p = f.find("EUR\":");
            if (p != std::string::npos) {
                size_t q = f.find(",", p);
                x["EUR"] = stod(f.substr(p + 5, q - p - 5));
            }
            p = f.find("RUB\":");
            if (p != std::string::npos) {
                size_t q = f.find(",", p);
                x["RUB"] = stod(f.substr(p + 5, q - p - 5));
            }
        }
        curl_easy_cleanup(e);
    }
}

int main() {
    g();
    if (x.empty()) {
        std::cout << "Error!" << std::endl;
        return 1;
    }

    while (true) {
        std::cout << "1. USD = " << x["USD"] << std::endl;
        std::cout << "2. EUR = " << x["EUR"] << std::endl;
        std::cout << "3. RUB = " << x["RUB"] << std::endl;
        std::cout << "4. exit" << std::endl;

        int c;
        std::cin >> c;

        if (c == 1) {
            double a;
            std::cout << "USD to EUR: ";
            std::cin >> a;
            std::cout << a << " USD = " << a * x["EUR"] << " EUR" << std::endl;
        }
        else if (c == 2) {
            double a;
            std::cout << "EUR to USD: ";
            std::cin >> a;
            std::cout << a << " EUR = " << a / x["EUR"] << " USD" << std::endl;
        }
        else if (c == 3) {
            double a;
            std::cout << "RUB to USD: ";
            std::cin >> a;
            std::cout << a << " RUB = " << a / x["RUB"] << " USD" << std::endl;
        }
        else if (c == 4) break;
        else std::cout << " " << std::endl;
    }
    return 0;
}
