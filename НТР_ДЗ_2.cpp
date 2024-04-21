#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>

// (1 + 2) * 3 / 4 + 5 * (6 - 7)

// 1. Токенизация
// 2. Парсер (построение дерева разбора выражения)

// +, -, *, /, %

struct OpeningBracket {};

struct ClosingBracket {};

struct Number {
    int value;
};

struct UnknownToken {
    std::string value;
};

struct MinToken {};

struct AbsToken {};

struct Plus {};

struct Minus {};

struct Multiply {};

struct Modulo {};

struct Divide {};

struct MaxToken {};

struct SqrToken {};

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, AbsToken, Plus, Minus,
    Multiply, Modulo, Divide, MaxToken, SqrToken>;

// 1234

const std::unordered_map<char, Token> kSymbol2Token{
    {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'/', Divide{}}, {'%', Modulo{}} };

const std::unordered_map<std::string, Token> kName2Token{
    {"max", MaxToken{}}, {"sqr", SqrToken{}}
};

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string name;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isalpha(symbol)) {
        name.push_back(symbol);
        if (pos == input.size() - 1) {
            throw std::runtime_error("Ошибка при разборе имени: выход за границу строки");
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    if (auto it = kName2Token.find(name); it != kName2Token.end()) {
        return it->second;
    }
    else {
        return UnknownToken{ name };
    }
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size() - 1) {
            throw std::runtime_error("Ошибка при разборе числа: выход за границу строки");
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    return Number{ value };
}

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;
    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol)) {
            ++pos;
        }
        else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        }
        else if (std::isalpha(symbol)) {
            tokens.emplace_back(ParseName(input, pos));
        }
        else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
            ++pos;
        }
        else {
            tokens.emplace_back(UnknownToken{ std::string(1,symbol) });
            ++pos;
        }
    }
    return tokens;
}

int main() {
    std::string expression = "(1 + 2) * 3 / 4 + 5 * (6 - 7)";

    // Токенизация выражения
    std::vector<Token> tokens = Tokenize(expression);

    // Вывод токенов
    std::cout << "Tokens: ";
    for (const auto& token : tokens) {
        if (std::holds_alternative<Number>(token)) {
            std::cout << std::get<Number>(token).value << " ";
        }
        else if (std::holds_alternative<Plus>(token)) {
            std::cout << "+ ";
        }
        else if (std::holds_alternative<Minus>(token)) {
            std::cout << "- ";
        }
        else if (std::holds_alternative<Multiply>(token)) {
            std::cout << "* ";
        }
        else if (std::holds_alternative<Divide>(token)) {
            std::cout << "/ ";
        }
        else if (std::holds_alternative<Modulo>(token)) {
            std::cout << "% ";
        }
        else if (std::holds_alternative<OpeningBracket>(token)) {
            std::cout << "( ";
        }
        else if (std::holds_alternative<ClosingBracket>(token)) {
            std::cout << ") ";
        }
        else if (std::holds_alternative<MaxToken>(token)) {
            std::cout << "max ";
        }
        else if (std::holds_alternative<SqrToken>(token)) {
            std::cout << "sqr ";
        }
        else if (std::holds_alternative<UnknownToken>(token)) {
            std::cout << std::get<UnknownToken>(token).value << " ";
        }
        else {
            std::cout << "operation ";
        }
    }
    std::cout << std::endl;

    return 0;
}