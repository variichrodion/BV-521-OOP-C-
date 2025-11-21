#include "FileSystem.h"

/* == FileSystemElement START == */

// конструктор элемента файловой системы
FileSystemElement::FileSystemElement(const std::string& name) : name(name) {}

/* == FileSystemElement END == */

/* == File START == */

// конструктор файла
File::File(const std::string& name, size_t size) : FileSystemElement(name), file_size(size) {}

// отобразить информацию файла
void File::displayInfo(int indent) const {
    std::cout << std::string(indent, ' ') << "[File] " << name << " size: " << file_size << std::endl;
}

// получить размер файла
size_t File::getSize() const { return file_size; }

// получить название файла
std::string File::getName() const { return name; }

// установить название файла
void File::setName(std::string new_name) { name = new_name; }

// получить родительский файла
FileSystemElement* File::getParent() const { return parent; }

// установить родительский элемент файла
void File::setParent(FileSystemElement* parent) { this->parent = parent; }

// получить тип "файл"
FileSystemElement::Type File::getType() const { return Type::FILE; };

// получить полный путь файла
std::string File::getPath() const { return parent->getPath() + "/" + getName(); }

/* == File END == */

/* == Folder START == */

// Поиск внутри директории (возвращается итератор на элемент)
std::vector<std::unique_ptr<FileSystemElement>>::iterator
Folder::searchLocalIter(std::string name) {
    auto it = find_if(elements.begin(), elements.end(),
        [&name](const auto& elem) { return elem->getName() == name; });
    return it;
}

// конструктор директории
Folder::Folder(const std::string& name) : FileSystemElement(name) {}

// отобразить информацию директории
void Folder::displayInfo(int indent) const {
    std::cout << std::string(indent, ' ') << "[Folder] " << name << std::endl;
    for (const auto& elem : elements)
        elem->displayInfo(indent + 3);
}

// получить размер директории
size_t Folder::getSize() const {
    size_t total_size = 0;
    for (const auto& elem : elements)
        total_size += elem->getSize();
    return total_size;
}

// получить название директории
std::string Folder::getName() const { return name; }

// установить название директории
void Folder::setName(std::string new_name) { name = new_name; }

// получить указатель на родительскую директорию
FileSystemElement* Folder::getParent() const { return parent; }

// установить родительский элемент директории
void Folder::setParent(FileSystemElement* parent) { this->parent = parent; }

// получить тип "директория"
FileSystemElement::Type Folder::getType() const { return Type::FOLDER; };

// получить полный путь директории
std::string Folder::getPath() const {
    if (parent) return parent->getPath() + "/" + getName();
    return "/" + getName();
}

// создать директорию (mkdir)
Folder* Folder::createFolder(std::string name) {
    auto new_folder = std::make_unique<Folder>(name);
    Folder* raw_ptr = new_folder.get();
    raw_ptr->setParent(this);
    elements.push_back(std::move(new_folder));
    return raw_ptr;
}

// создать файл (touch)
File* Folder::createFile(std::string name, size_t size) {
    auto new_file = std::make_unique<File>(name, size);
    File* raw_ptr = new_file.get();
    raw_ptr->setParent(this);
    elements.push_back(std::move(new_file));
    return raw_ptr;
}

// найти элемент внутри директории (findlocal)
FileSystemElement* Folder::searchLocal(const std::string& name) {
    auto it = searchLocalIter(name); // elements.end();
    if (it != elements.end()) return it->get();
    else return nullptr;
}

// найти элемент во всех поддиректориях (finddeep)
FileSystemElement* Folder::deepSearch(const std::string& name) {
    for (auto& element : elements) { // element - unique_ptr<FileSystemElement>
        switch (element->getType()) {
        case Type::FILE:
            if (name == element->getName()) { return element.get(); }
            break;
        case Type::FOLDER:
            if (name == element->getName()) { return element.get(); }
            auto ptr = static_cast<Folder*>(element.get())->deepSearch(name);
            if (ptr) return ptr;
            break;
        }
    }
    return nullptr;
}

// удалить элемент в текущей директории (rm)
bool Folder::removeElement(const std::string& name) {
    auto it = searchLocalIter(name);
    if (it != elements.end()) {
        elements.erase(it);
        return true;
    }
    return false;
}

// подсчитать количество элементов (calcelem)
void Folder::calculateElements(int& fileCount, int& folderCount) {
    for (auto& element : elements) {
        switch (element->getType()) {
        case Type::FILE:
            fileCount++; break;
        case Type::FOLDER:
            folderCount++;
            static_cast<Folder*>(element.get())->calculateElements(fileCount, folderCount);
            break;
        }
    }
}


/* == Folder END == */

/* == FileSystem START == */

// конструктор FileSystem
FileSystem::FileSystem() {
    root = std::make_unique<Folder>("root");
    current_folder = root.get();
    initStruct();
}

// получить указатель на текущую директорию
Folder* FileSystem::getCurrentFolder() { return current_folder; }

// обработчик команд
void FileSystem::CommandProcessor(const std::string& user_input) {
    // user_input = "cd documents/projects"
    std::vector<std::string> cmd_input = splitStr(user_input, ' ');
    // cmd_input = ["cd", "documents/projects"]
    if (cmd_input.empty()) return;
    std::string command = cmd_input[0];     // command = "cd"
    Commands cmd = parseCommand(command);   // cmd = CD
    cmd_input.erase(cmd_input.begin());     // cmd_input = ["documents/projects"]
    // cmd_input = []
    switch (cmd) {
    case Commands::CD:
        if(!cmd_input.empty()) navigateToPath(cmd_input[0]);
        break;
    case Commands::TREE:
        current_folder->displayInfo();
        break;
    default:
        std::cout << "Неизвестная команда!\n";
        break;
    }
}

// преобразование string -> enum Commands
FileSystem::Commands FileSystem::parseCommand(const std::string& command) {
    if (command == "cd") return Commands::CD;
    if (command == "ls") return Commands::LS;
    if (command == "tree") return Commands::TREE;
}

// определяем метод перехода (полный или относительный путь)
void FileSystem::navigateToPath(const std::string& path) {
    if (!path.empty()) {
        if (path[0] == '/') current_folder = navigateFromRoot(path);
        else current_folder = navigateFromCurrent(path);
    }
}

// для полного пути (начинается с '/')
Folder* FileSystem::navigateFromRoot(const std::string& path) {
    Folder* end_folder = root.get();                    // конечная директория
    std::vector<std::string> parts = splitStr(path, '/');   // разделяем путь на подстроки
    // "/root/documents/..." = "/documents"
    // parts = ["root", "documents", "..."]
    for (const std::string& part : parts) {
        // Что вернул поиск подпапки?
        if (part == "root") continue;
        Folder* temp = dynamic_cast<Folder*>(end_folder->searchLocal(part));
        if (!temp) {
            std::cout << "Ошибка: путь не существует! - " << part << '\n';
            return current_folder;
        }
        end_folder = temp;
    }
    return end_folder;
}

// для относительного пути   (не имеет '/')
Folder* FileSystem::navigateFromCurrent(const std::string& path) {
    // мы в root/documents: ".././images"
    Folder* end_folder = current_folder;
    std::vector<std::string> parts = splitStr(path, '/');
    // parts[0] = ".." parts[1] = "images"
    for (const std::string& part : parts) {
        if (part == "..") {     // если ".."
            if (end_folder->getParent())
                end_folder = dynamic_cast<Folder*>(end_folder->getParent());
        }
        else if (part != ".") { // если "."
            Folder* temp = dynamic_cast<Folder*>(end_folder->searchLocal(part));
            if (!temp) {
                std::cout << "Ошибка: путь не существует! - " << part << '\n';
                return current_folder;
            }
            end_folder = temp;
        }
    }
    return end_folder;
}

// разделение строки по символу
std::vector<std::string> FileSystem::splitStr(const std::string& str, char sym) {
    // "root/documents/projects"
    std::vector<std::string> parts; // parts - части
    std::stringstream ss(str);
    std::string part;               // часть строки
    while (std::getline(ss, part, sym)) {
        if (!part.empty()) parts.push_back(part);
    }
    return parts;
}

// создание тестовой файловой системы
void FileSystem::initStruct() {
    // папки первого уровня
    Folder* documents = root->createFolder("documents");
    Folder* images = root->createFolder("images");

    // содержимое documents
    documents->createFile("resume.pdf", 2048);
    documents->createFile("contract.docx", 4096);

    Folder* projects = documents->createFolder("projects");
    projects->createFile("main.cpp", 1536);
    projects->createFile("utils.h", 512);
    projects->createFile("README.md", 1024);

    Folder* work = documents->createFolder("work");
    work->createFile("report.xlsx", 3072);
    work->createFile("presentation.pptx", 6144);

    // сожжержимое images
    images->createFile("photo.jpg", 10240);
    images->createFile("screenshot.png", 8192);

    Folder* vacations = images->createFolder("vacations");
    vacations->createFile("beach.jpg", 15360);
    vacations->createFile("mountain.png", 12288);

    Folder* screenshots = images->createFolder("screenshots");
    screenshots->createFile("game.png", 7168);
    screenshots->createFile("app.png", 6144);
}

/* == FileSystem END == */