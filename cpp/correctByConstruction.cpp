#include "pch.h"

enum class BuyOrSell { BUY, SELL };

struct Quantity {
    // explicit keyword prevents raw int's and float from being used to construct
    explicit Quantity(int quantity_) : quantity(quantity_) {}
    int value() const { return quantity; }
private:
    int quantity;
};

struct Price {
    explicit Price(float price_) : price(price_) {}
    float value() const { return price; }
private:
    float price;
};

void tradeStockUnsafe(const std::string&name, bool isBuying, int quantity, float price) {
    std::cout << (isBuying ? "Bought " : "Sold ") << quantity << " " << name << " shares for $" << price << " each" << std::endl;
}
void tradeStockSafe(const std::string& name, BuyOrSell buyOrSell, Quantity quantity, Price price) {
    tradeStockUnsafe(name, buyOrSell == BuyOrSell::BUY, quantity.value(), price.value());
}

int main() {
    // Trade 100 Google shares for $1000.00 each.
    tradeStockUnsafe("GOOG", false, 100, 1000.00);
    tradeStockSafe("GOOG", BuyOrSell::SELL, Quantity(100), Price(1000.00));

    tradeStockUnsafe("GOOG", false, 1000.00, 100);
    // sellStockSafe("GOOG", BuyOrSell::SELL, 1000.00, 100); Compiler Error: explicit constructor prevents this
}