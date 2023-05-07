#include "../include/Manage.hpp"

extern std::vector<std::string> split(const std::string& sourceString,
                                      const std::string& delimiter);

Manage::Manage(int argc, char* argv[])
    : commandReceiver(
          argc > 1 ? argv[1]
                   : throw std::invalid_argument(
                         "The option behind toy is necessary, you can use the "
                         "option \"--help\" to know the optional options!")),
      CommandVector(split(CommandSet, "\\")) {
  env = "null";
  env_hide = "null";
  deployCode = 0;

  ManageLog("START");
}

void Manage::ManageInit() {
  std::ifstream fin("profile.json");

  if (fin.is_open()) {
    fin >> this->ManageJson;
    this->deployCode = this->ManageJson["deployCode"];
    this->env = this->ManageJson["env"];
    this->env_hide = this->ManageJson["env_hide"];
    fin.close();

    this->FolderPathCheck();
  } else {
    std::ofstream fout("profile.json");

    std::cout << "please enter the env path"
              << " " << std::endl;
    std::cin >> this->env;
    std::cout << "please enter the hided env path"
              << " " << std::endl;
    std::cin >> this->env_hide;

    this->deployCode = 1;
    this->ManageJson["deployCode"] = this->deployCode;
    this->ManageJson["env"] = this->env;
    this->ManageJson["env_hide"] = this->env_hide;

    this->FolderPathCheck();

    fout << this->ManageJson << std::endl;
    fout.close();

    ManageLog("INIT");
  }
}

void Manage::ManageHandle() {
  if (this->CommandSet.find(this->commandReceiver) == std::string::npos) {
    throw std::runtime_error("You have entered the nonexistent command!!!");
  }
  
  if ((this->commandReceiver) == "--store") {
    this->ManageStore();
  } else if ((this->commandReceiver) == "--delete") {
    this->ManageDelete();
  } else if ((this->commandReceiver) == "--delete--deep") {
    this->ManageDeepDelete();
  } else if ((this->commandReceiver) == "--list") {
    this->ManageList();
  } else if ((this->commandReceiver) == "--list--hide") {
    this->ManageListHide();
  } else if ((this->commandReceiver) == "--recover") {
    this->ManageRecoverFileResource();
  } else if ((this->commandReceiver) == "--help") {
    this->ManageHelp();
  } else {
    throw std::invalid_argument(
        "You have entered the nonexustent command!!!\nplease make sure your "
        "option parameters start with \"--\"!!!");
  }
}

void Manage::ManageStore() {
  std::string name;
  std::string fileName;
  std::string filePath;
  std::string Destination;
  std::string _Command;
  std::string _CommandHide;

  std::cout << "please enter the information of the book " << std::endl;
  std::cout << "Book Name: ";
  std::cin >> name;
  std::cout << "Book File Name: ";
  std::cin >> fileName;
  std::cout << "Book File Path: ";
  std::cin >> filePath;

  try {
    FilePathCheck(filePath, fileName);
    Book BookGoal = Book(name, fileName, filePath);
    BookGoal.jsonConvert();
    std::fstream fs;
    std::fstream fs_hide;

    ManageJsonStore(fs, "storage.json", BookGoal);
    ManageJsonStore(fs_hide, "storage_hide.json", BookGoal);

    _Command = std::string("mv ") + filePath + std::string("\\") + fileName +
               std::string(" ") + this->env;
    _CommandHide = std::string("copy ") + this->env + std::string("\\") +
                   fileName + std::string(" ") + this->env_hide;

    system(_Command.c_str());
    system(_CommandHide.c_str());

    ManageLog("STORE");
  } catch (std::exception& ErrorMsg) {
    std::cerr << ErrorMsg.what() << std::endl;
    ManageLog("STORE_FAILURE");
  }
}

void Manage::ManageDelete() {
  std::string name;
  std::string _Command;

  std::cout << "please enter the name of file deleted: ";
  std::cin >> name;

  std::ifstream fin("storage.json");
  fin >> ManageBookJsonArray;

  for (auto _iterator = ManageBookJsonArray.begin();
       _iterator != ManageBookJsonArray.end(); ++_iterator) {
    // 查找当前元素中是否有"name":"object"的键值对
    auto found = _iterator->find("name");
    auto found_fileName = _iterator->find("fileName");
    if (found != _iterator->end() && *found == name) {
      try {
        FilePathCheck(this->env, found_fileName.value().get<std::string>());
        _Command = std::string("del ") + this->env + std::string("\\") +
                   found_fileName.value().get<std::string>();
        system(_Command.c_str());
        std::cout << "file has been deleted!" << std::endl;

        // 如果有，就删除该元素
        ManageBookJsonArray.erase(_iterator);
        // 因为删除了一个元素，所以迭代器需要回退一步
        --_iterator;
      } catch (std::exception& ErrorMsg) {
        std::cerr << ErrorMsg.what() << std::endl;
        ManageLog("DELETE_FAILURE");
      }
    }
  }

  fin.close();

  std::ofstream fout("storage.json");
  fout << ManageBookJsonArray;

  fout.close();

  ManageLog("DELETE");
}

void Manage::ManageDeepDelete() {
  std::string name;
  std::string _Command;

  std::cout << "please enter the name of file deleted: ";
  std::cin >> name;

  std::ifstream fin("storage_hide.json");
  fin >> ManageBookJsonArray;

  for (auto _iterator = ManageBookJsonArray.begin();
       _iterator != ManageBookJsonArray.end(); ++_iterator) {
    // 查找当前元素中是否有"name":"object"的键值对
    auto found = _iterator->find("name");
    auto found_fileName = _iterator->find("fileName");
    if (found != _iterator->end() && *found == name) {
      try {
        FilePathCheck(this->env_hide,
                      found_fileName.value().get<std::string>());
        _Command = std::string("del ") + this->env_hide + std::string("\\") +
                   found_fileName.value().get<std::string>();
        system(_Command.c_str());
        std::cout << "file has been deleted!" << std::endl;

        // 如果有，就删除该元素
        ManageBookJsonArray.erase(_iterator);
        // 因为删除了一个元素，所以迭代器需要回退一步
        --_iterator;
      } catch (std::exception& ErrorMsg) {
        std::cerr << ErrorMsg.what() << std::endl;
        ManageLog("DEEP_DELETE_FAILURE");
      }
    }

    fin.close();

    std::ofstream fout("storage_hide.json");
    fout << ManageBookJsonArray;

    fout.close();

    ManageLog("DEEP_DELETE");
  }
}

void Manage::ManageList() {
  std::string name;
  std::ifstream fin("storage.json");

  fin >> ManageBookJsonArray;

  for (auto _iterator = ManageBookJsonArray.begin();
       _iterator != ManageBookJsonArray.end(); ++_iterator) {
    name = (_iterator->find("name")).value().get<std::string>();
    std::cout << name << std::endl;
  }

  fin.close();

  ManageLog("LIST");
}

void Manage::ManageListHide() {
  std::string name;
  std::ifstream fin("storage_hide.json");

  fin >> ManageBookJsonArray;

  for (auto _iterator = ManageBookJsonArray.begin();
       _iterator != ManageBookJsonArray.end(); ++_iterator) {
    name = (_iterator->find("name")).value().get<std::string>();
    std::cout << name << std::endl;
  }

  fin.close();

  ManageLog("LIST_HIDE");
}

void Manage::ManageRecoverFileResource() {
  std::string name;
  std::string _Command;
  std::ifstream fin("storage_hide.json");

  std::cout << "please enter name of the book to be recovered: ";
  std::cin >> name;
  fin >> ManageBookJsonArray;

  for (auto _iterator = ManageBookJsonArray.begin();
       _iterator != ManageBookJsonArray.end(); ++_iterator) {
    auto found_name = _iterator->find("name");
    auto found_fileName = _iterator->find("fileName");
    auto found_filePath = _iterator->find("filePath");

    if (found_name != _iterator->end() && *found_name == name) {
      try {
        FilePathCheck(this->env_hide,
                      found_fileName.value().get<std::string>());

        _Command = std::string("copy /-Y ") + this->env_hide +
                   std::string("\\") +
                   found_fileName.value().get<std::string>() +
                   std::string(" ") + this->env;
        system(_Command.c_str());

        std::fstream fs;
        ManageJsonStore(fs, "storage.json",
                        Book(found_name.value().get<std::string>(),
                             found_fileName.value().get<std::string>(),
                             found_filePath.value().get<std::string>()));

        std::cout << "file has been recovered!" << std::endl;

        ManageLog("RECOVER");
      } catch (std::exception& ErrorMsg) {
        std::cerr << ErrorMsg.what() << std::endl;
        ManageLog("RECOVER_FAILURE");
      }
    }
  }

  fin.close();
}

void Manage::ManageHelp() {
  for (auto Command : CommandVector) {
    std::cout << Command << std::endl;
  }

  ManageLog("HELP");
}

void Manage::ManageLog(const std::string& CommandName) {
  std::ofstream fout("log", std::ios::out | std::ios::app);

  time(&CurrentTime);
  LogTime = ctime(&CurrentTime);

  fout << "Execute " << CommandName << "   Time is : " << LogTime << std::endl;

  fout.close();
}

void Manage::ManageJsonStore(std::fstream& fs, const std::string& JsonFileName,
                             const Book& BookGoal) {
  fs.open(JsonFileName,
          std::ios::in | std::ios::out);  // 以输入输出模式打开文件
  if (!fs.is_open()) {
    fs.open(JsonFileName,
            std::ios::out);  // 如果文件不存在，以输出模式创建文件
    fs << "[]" << std::endl;  // 写入一个空的JSON数组
    fs.close();               // 关闭文件
    fs.open(JsonFileName,
            std::ios::in | std::ios::out);  // 重新以输入输出模式打开文件
  }
  fs >> ManageBookJsonArray;                         // 读取JSON数组
  ManageBookJsonArray.push_back(BookGoal.bookJson);  // 添加新的元素
  fs.seekp(0);                                       // 清空文件内容
  fs << ManageBookJsonArray << std::endl;  // 写入修改后的JSON数组
  fs.close();                              // 关闭文件

  ManageLog(JsonFileName + " STORED");
}

void Manage::FolderPathCheck() {
  std::string _Command;
  std::string _CommandHide;

  std::filesystem::path BookStoredFolder = this->env;
  std::filesystem::path BookStoredFolderHided = this->env_hide;

  if (!std::filesystem::exists(BookStoredFolder)) {
    _Command = std::string("mkdir ") + this->env;
    system(_Command.c_str());
  }

  if (!std::filesystem::exists(BookStoredFolderHided)) {
    _CommandHide = std::string("mkdir ") + this->env_hide;
    system(_CommandHide.c_str());
  }

  ManageLog("FolderPathCheck");
}

void Manage::FilePathCheck(const std::string& filePath,
                           const std::string& fileName) {
  std::filesystem::path BookFilePath = filePath + std::string("\\") + fileName;

  if (!std::filesystem::exists(BookFilePath)) {
    ManageLog("FilePathCheck");
    throw std::invalid_argument(
        "The infomation entered of the book is invalid! please check again!");
  }
  ManageLog("FilePathCheck");
}

std::vector<std::string> split(const std::string& sourceString,
                               const std::string& delimiter) {
  std::vector<std::string> tokens;
  size_t pos = 0;
  size_t last = 0;
  while ((pos = sourceString.find(delimiter, last)) != std::string::npos) {
    tokens.push_back(sourceString.substr(last, pos - last));
    last = pos + delimiter.length();
  }
  tokens.push_back(sourceString.substr(last));
  return tokens;
}