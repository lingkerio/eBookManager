#include "../include/Book.hpp"

void Book::from_json(const json& bookJson, Book::bookInfo& bookInfoInstance) {
  bookJson.at("name").get_to(bookInfoInstance.name);
  bookJson.at("fileName").get_to(bookInfoInstance.fileName);
  bookJson.at("filePath").get_to(bookInfoInstance.filePath);
}

void Book::jsonConvert() {
  this->bookJson = json{{"name", (this->bookInfoInstance).name},
                        {"fileName", (this->bookInfoInstance).fileName},
                        {"filePath", (this->bookInfoInstance).filePath}};
}

const json& Book::getJson() const { return this->bookJson; }

Book::~Book() {}