#include <iostream>
#include <memory>
#include <vector>
#include <cstring>

class FileSystemElement {
protected:
    std::string name;
    FileSystemElement(const std::string& name) : name(name) {}
    FileSystemElement* parent = nullptr;
public:
    enum class Type {FILE, FOLDER};
    virtual ~FileSystemElement() = default;
    virtual void displayInfo(int) const = 0;
    virtual size_t getSize() const = 0;
    virtual std::string getName() const = 0;
    virtual void setName(std::string) = 0;
    virtual void setParent(FileSystemElement*) = 0;
    virtual FileSystemElement* getParent() const = 0;
    /* ================================= */
    virtual Type getType() const = 0;
    virtual std::string getPath() const = 0;
};

class File : public FileSystemElement {
private:
    size_t file_size;
public:
    File(const std::string& name, size_t size) : FileSystemElement(name), file_size(size) {}
    void displayInfo(int indent) const override {
        std::cout << std::string(indent, ' ') << "[File] " << name << " size: " << file_size << std::endl;
    }
    size_t getSize() const override { return file_size; }
    std::string getName() const override { return name; }
    void setName(std::string new_name) override { name = new_name; }
    FileSystemElement* getParent() const override { return parent; }
    void setParent(FileSystemElement* parent) override { this->parent = parent; }
    /* =============================================== */
    FileSystemElement::Type getType() const override { return Type::FILE; };
    std::string getPath() const override{
        return parent->getPath() + "/" + getName();
    }
};

class Folder : public FileSystemElement {
private:
   
    std::vector<std::unique_ptr<FileSystemElement>>::iterator
        searchLocalIter(std::string name) {
        auto it = find_if(elements.begin(), elements.end(),
            [&name](const auto& elem) { return elem->getName() == name; });
        return it;
    }

public:
    std::vector<std::unique_ptr<FileSystemElement>> elements;
    Folder(const std::string& name) : FileSystemElement(name) {}
    std::string getName() const override { return name; }
    void setName(std::string new_name) override { name = new_name; }
    FileSystemElement* getParent() const override { return parent; }
    void setParent(FileSystemElement* parent) override { this->parent = parent; }
    FileSystemElement::Type getType() const override { return Type::FOLDER; };
    size_t getSize() const override {
        size_t total_size = 0;
        for (const auto& elem : elements)
            total_size += elem->getSize();
        return total_size;
    }

    Folder* createFolder(std::string name) {
        auto new_folder = std::make_unique<Folder>(name);
        Folder* raw_ptr = new_folder.get();
        raw_ptr->setParent(this);
        elements.push_back(std::move(new_folder));
        return raw_ptr;
    }

    File* createFile(std::string name, size_t size) {
        auto new_file = std::make_unique<File>(name, size);
        File* raw_ptr = new_file.get();
        raw_ptr->setParent(this);
        std::cout << this << '\n';
        std::cout << raw_ptr->getParent() << '\n';
        elements.push_back(std::move(new_file));
        return raw_ptr;
    }

    void displayInfo(int indent = 0) const override { // indent - отступ
        std::cout << std::string(indent, ' ') << "[Folder] " << name << std::endl;
        for (const auto& elem : elements)
            elem->displayInfo(indent + 3);
    }

    FileSystemElement* searchLocal(const std::string& name) {
        auto it = searchLocalIter(name); // elements.end();
        if (it != elements.end()) return it->get();
        else return nullptr;
    }

    bool removeElement(const std::string& name) {
        auto it = searchLocalIter(name);
        if (it != elements.end()) {
            elements.erase(it);
            return true;
        }
        return false;
    }

    FileSystemElement* deepSearch(const std::string& name) {
        for (auto& element : elements) { // element - unique_ptr<FileSystemElement>
            switch (element->getType()) {
            case Type::FILE:
                if (name == element->getName()) { return element.get(); }
                break;
            case Type::FOLDER:
                if (name == element->getName()) { return element.get(); }
                auto ptr = static_cast<Folder*>(element.get())->deepSearch(name);
                if(ptr) return ptr;
                break;
            }
        }
        return nullptr;
    }
    
    void calculateElements(int& fileCount, int& folderCount) {
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

    std::string getPath() const override {
        if(parent) return parent->getPath() + "/" + getName();
        return "/" + getName();
    }

}; /* Конец класса Folder */

std::unique_ptr<Folder> createTestFileSystem();

int main() {
    auto root = createTestFileSystem(); // корневая папка
    root->displayInfo();                // вывод иерархии
    std::cout << root->deepSearch("README.md")->getPath() << '\n';

    return 0;
}

std::unique_ptr<Folder> createTestFileSystem() {
    auto root = std::make_unique<Folder>("root");

    // Создаем папки первого уровня
    Folder* documents = root->createFolder("documents");
    Folder* images = root->createFolder("images");

    // Заполняем папку documents
    documents->createFile("resume.pdf", 2048);
    documents->createFile("contract.docx", 4096);

    Folder* projects = documents->createFolder("projects");
    projects->createFile("main.cpp", 1536);
    projects->createFile("utils.h", 512);
    projects->createFile("README.md", 1024);

    Folder* work = documents->createFolder("work");
    work->createFile("report.xlsx", 3072);
    work->createFile("presentation.pptx", 6144);

    // Заполняем папку images
    images->createFile("photo.jpg", 10240);
    images->createFile("screenshot.png", 8192);

    Folder* vacations = images->createFolder("vacations");
    vacations->createFile("beach.jpg", 15360);
    vacations->createFile("mountain.png", 12288);

    Folder* screenshots = images->createFolder("screenshots");
    screenshots->createFile("game.png", 7168);
    screenshots->createFile("app.png", 6144);

    return root;
}