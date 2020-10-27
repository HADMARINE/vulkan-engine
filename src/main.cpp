//
// Created by HADMARINE on 2020/09/20.
//

#include "SharkEngine.hpp"

int main() {
    SharkEngine engine;
  try{
    engine.run();
  } catch (const std::exception& e) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}