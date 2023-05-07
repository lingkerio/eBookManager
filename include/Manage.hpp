#ifndef MANAGE_HPP
#define MANAGE_HPP

#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "Book.hpp"

using json = nlohmann::json;

class Manage {
 private:
  int deployCode;

  time_t CurrentTime;

  std::string env;
  std::string env_hide;
  std::string LogTime;

  const std::string commandReceiver;
  const std::string CommandSet =
      "--store\\--delete\\--delete--deep\\--list\\--list--hide\\--recover\\--"
      "help";
  std::vector<std::string> CommandVector;

  json ManageJson;
  json ManageBookJsonArray = json::array();

 public:
  Manage(int argc, char* argv[]);

  void ManageInit();
  void ManageHandle();

  void ManageStore();
  void ManageDelete();
  void ManageDeepDelete();
  void ManageList();
  void ManageListHide();
  void ManageRecoverFileResource();
  void ManageHelp();

  void ManageJsonStore(std::fstream& fs, const std::string& JsonFileName,
                       const Book& BookGoal);
  void ManageLog(const std::string& CommandName);
  void FolderPathCheck();
  void FilePathCheck(const std::string& filePath, const std::string& fileName);

  ~Manage() { ManageLog("END"); }

  friend std::vector<std::string> split(const std::string& sourceString,
                                        const std::string& delimiter);
};

#endif