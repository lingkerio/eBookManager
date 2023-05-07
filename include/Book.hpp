#ifndef BOOK_HPP
#define BOOK_HPP

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Manage;

class Book {
 private:
  friend class Manage;
  struct bookInfo {
    std::string name;
    std::string fileName;
    std::string filePath;
    bookInfo(const std::string& name, const std::string& fileName,
             const std::string& filePath) {
      this->name = name;
      this->fileName = fileName;
      this->filePath = filePath;
    }
  };
  const bookInfo bookInfoInstance;
  json bookJson;

 public:
  Book(const std::string& name, const std::string& fileName,
       const std::string& filePath)
      : bookInfoInstance{name, fileName, filePath} {}
  void from_json(const json& bookJson, Book::bookInfo& bookInfoInstance);
  void jsonConvert();
  const json& getJson() const;
  ~Book();
};
#endif