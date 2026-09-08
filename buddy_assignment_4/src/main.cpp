#include <iostream>
#include <string>

#include "fastmap.hpp"
#include "kmeans.hpp"

static void print_menu() {
    std::cout << "\n===== CS509 Buddy Task (K-Means + FastMap) =====\n";
    std::cout << " 1) K-Means Clustering\n";
    std::cout << " 2) FastMap\n";
    std::cout << " 0) Exit\n";
    std::cout << "Choice: ";
}

static void interactive_mode() {
    while (true) {
        print_menu();
        int choice = 0;
        if (!(std::cin >> choice)) break;

        switch (choice) {
            case 1: {
                std::cout << "Enter input file path: ";
                std::string path;
                std::cin >> path;
                run_kmeans(path);
                break;
            }
            case 2: {
                std::cout << "Enter input file path: ";
                std::string path;
                std::cin >> path;
                run_fastmap(path);
                break;
            }
            case 0:
                return;
            default:
                std::cout << "Unknown option.\n";
        }
    }
}

int main(int argc, char **argv) {
    if (argc == 3) {
        const std::string mode = argv[1];
        const std::string path = argv[2];

        if (mode == "km") return run_kmeans(path);
        if (mode == "fm") return run_fastmap(path);

        std::cerr << "Usage: " << argv[0] << " <km|fm> <input_file>\n";
        return 1;
    }

    if (argc != 1) {
        std::cerr << "Usage: " << argv[0] << " <km|fm> <input_file>\n";
        return 1;
    }

    interactive_mode();
    return 0;
}
