// views_zip.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// https://www.cppstories.com/2023/view-zip/
//
// INCOMPLETE

// INCOMPLETE

// #define _HAS_CXX20 1
#define _HAS_CXX23 1    // required because visual studio 2022 ranges error (see: https://developercommunity.visualstudio.com/t/1544711)

// #include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <vector>

void combine_container_basic(void)
{
    // loop
    std::vector a{ 10, 20, 30, 40, 50 };
    std::vector<std::string> b{ "ten", "twenty", "thirty", "fourty" };

    for (size_t i = 0; i < std::min(a.size(), b.size()); ++i)
        std::cout << std::format("{} -> {}\n", a[i], b[i]);

    std::cout << "\n";

    // use iota to generate indices
    // range version
    for (auto i : std::ranges::iota_view{ 0ULL, std::min(a.size(), b.size()) })
        std::cout << std::format("{} -> {}\n", a[i], b[i]);

    std::cout << "\n";

    // extract some names rather than using indices:
    for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) {
        const auto& num = a[i];
        const auto& name = b[i];
        std::cout << std::format("{} -> {}\n", num, name);
    }

    std::cout << "\n";
}

// requires C++23
void combine_container_views()
{
    std::vector a{ 10, 20, 30, 40, 50 };
    std::vector<std::string> b{ "ten", "twenty", "thirty", "fourty" };

    //for (const auto& [num, name] : std::views::zip(std::move(a), std::move(b)))
    for (const auto& [num, name] : std::views::zip(a, b))
        std::cout << std::format("{} -> {}\n", num, name);

    std::cout << "\n";
}

void combine_multiple_containers()
{
    std::vector<std::string> dates{ "2023-03-01", "2023-03-01", "2023-03-02", "2023-03-03", "2023-03-03" };
    std::vector<std::string> products{ "Shoes", "T-Shirts", "Pants", "Shoes", "T-Shirts" };
    std::vector sales{ 50.0, 20.0, 30.0, 75.0, 40.0 };  // uses Class template argument deduction (CTAD)
    
    std::map<std::string, double> salesInDay;
    std::map<std::string, double> salesPerProduct;
    for (const auto& [d, p, s] : std::views::zip(dates, products, sales)) {
        salesInDay[d] += s;
        salesPerProduct[p] += s;
    }

    for (const auto& [day, sale] : salesInDay)
        std::cout << std::format("in {} sold {}\n", day, sale);

    for (const auto& [prod, sale] : salesPerProduct)
        std::cout << std::format("sold {} in {} category\n", sale, prod);

    std::cout << "\n";
}

void zip(void)
{
    std::vector prices { 100, 200, 150, 180, 130 };  // uses Class template argument deduction (CTAD)
    std::vector costs  { 10, 20, 50, 40, 100 };      // uses Class template argument deduction (CTAD)

    // std::vector income; // we cannot use Class template argument deduction (CTAD) here because there is no data
    std::vector<int> income;
    income.reserve(prices.size());
    for (const auto& [p, c] : std::views::zip(prices, costs))
        income.emplace_back(p - c);

    std::cout << std::accumulate(income.begin(), income.end(), 0);
    std::cout << "\n";
}

void zip_transform(void)
{
    std::vector prices{ 100, 200, 150, 180, 130 };  // uses Class template argument deduction (CTAD)
    std::vector costs{ 10, 20, 50, 40, 100 };       // uses Class template argument deduction (CTAD)

    // std::vector income; // we cannot use Class template argument deduction (CTAD) here because there is no data
    std::vector<int> income;
    income.reserve(prices.size());
    auto compute = [](const auto& p, const auto& c) { return p - c; };

    // does not compile
    auto v = std::views::zip_transform(compute, prices, costs);
    for (const auto& in : v)
        income.emplace_back(in);

    std::cout << std::accumulate(income.begin(), income.end(), 0);
    std::cout << "\n";
}

void ranges_to(void)
{
    std::vector prices{ 100, 200, 150, 180, 130 };  // uses Class template argument deduction (CTAD)
    std::vector costs{ 10, 20, 50, 40, 100 };       // uses Class template argument deduction (CTAD)
   
    auto compute = [](const auto& p, const auto& c) { return p - c; };
    auto v = std::views::zip_transform(compute, prices, costs);
    auto income = v | std::ranges::to<std::vector>();
}

int main()
{
    combine_container_basic();
    combine_container_views();
    combine_multiple_containers();
    zip();
    zip_transform();
    ranges_to();

    std::cin.get();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
