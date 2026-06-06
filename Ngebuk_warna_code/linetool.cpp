#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream svg("line.svg");

    svg << R"(<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg"
     viewBox="0 0 24 24"
     width="240"
     height="240">

  <!-- Line -->
  <line x1="3"
        y1="20"
        x2="21"
        y2="4"
        stroke="#6AB04C"
        stroke-width="2.5"
        stroke-linecap="round"/>

  <!-- Endpoint handles -->
  <circle cx="3"
          cy="20"
          r="2.5"
          fill="#6AB04C"/>

  <circle cx="21"
          cy="4"
          r="2.5"
          fill="#6AB04C"/>

</svg>)";

    svg.close();

    cout << "File SVG berhasil dibuat: line.svg" << endl;
    return 0;
}