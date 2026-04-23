

#include "SkipList.hpp"
#include <iostream>
#include <string>

int main() {
    SkipList<int> sl;
    std::string op;
    int val;
    while (std::cin >> op >> val) {
        if (op == "insert") {
            sl.insert(val);
        } else if (op == "delete") {
            sl.deleteItem(val);
        } else if (op == "search") {
            if (sl.search(val)) {
                std::cout << "true" << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }
        }
    }
    return 0;
}

