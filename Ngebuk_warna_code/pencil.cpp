#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream svg("pencil.svg");

    svg << R"(<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg"
     width="240"
     height="240"
     viewBox="0 0 24 24">

    <!-- Pencil Body -->
    <polygon points="7,17 5,21 9,19 18,10 16,8"
             fill="#F9CA24"
             stroke="#555"
             stroke-width="0.8"/>

    <!-- Wooden Tip -->
    <polygon points="18,10 20,8 18,6 16,8"
             fill="#D2A679"
             stroke="#555"
             stroke-width="0.8"/>

    <!-- Graphite -->
    <polygon points="20,8 21.5,6.5 18,6"
             fill="#333"/>

    <!-- Eraser -->
    <polygon points="5,21 3,19 4.5,17.5 7,17"
             fill="#FFB3C6"
             stroke="#555"
             stroke-width="0.8"/>

    <!-- Metal Band -->
    <polygon points="7,17 4.5,17.5 6,16 8,15"
             fill="#C0C0C0"
             stroke="#555"
             stroke-width="0.6"/>

</svg>)";

    svg.close();

    cout << "File pencil.svg berhasil dibuat!" << endl;

    return 0;
}