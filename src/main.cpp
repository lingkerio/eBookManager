#include "../include/Manage.hpp"

int main(int argc, char* argv[]) {
  try {
    auto Manager = Manage(argc, argv);
    Manager.ManageInit();

    try {
      Manager.ManageHandle();
    } catch (std::exception& ErrorMsg) {
      std::cerr << ErrorMsg.what() << std::endl;
    }
    
  } catch (std::exception& ErrorMsg) {
    std::cerr << ErrorMsg.what() << std::endl;
  }
  return 0;
}