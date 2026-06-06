#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream svg("ellipse.svg");

    svg << R"(<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg"
     viewBox="0 0 24 24"
     width="240"
     height="240">

  <!-- Shadow -->
  <ellipse cx="12.5"
           cy="12.5"
           rx="9"
           ry="6"
           fill="#00000018"/>

  <!-- Ellipse outline -->
  <ellipse cx="11.5"
           cy="11.5"
           rx="9"
           ry="6"
           fill="white"
           stroke="#E06090"
           stroke-width="2"/>

  <!-- Corner handles -->
  <circle cx="2.5"
          cy="11.5"
          r="1.5"
          fill="#E06090"/>

  <circle cx="20.5"
          cy="11.5"
          r="1.5"
          fill="#E06090"/>

  <circle cx="11.5"
          cy="5.5"
          r="1.5"
          fill="#E06090"/>

  <circle cx="11.5"
          cy="17.5"
          r="1.5"
          fill="#E06090"/>

</svg>)";

    svg.close();

    cout << "File SVG berhasil dibuat: ellipse.svg" << endl;

    return 0;
}