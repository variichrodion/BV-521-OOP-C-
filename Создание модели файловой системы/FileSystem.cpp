#include <iostream>
#include <memory>
#include <vector>
#include <memory>

class FileSystemElement {
protected:
    std::string name;
    FileSystemElement(const std::string& name)
        : name(name) {
    }
    FileSystemElement* parent = nullptr;
public:
    virtual ~FileSystemElement() = default;
    virtual void displayInfo(int) const = 0;
    virtual size_t getSize() const = 0;
    virtual std::string getName() const = 0;
    virtual void setName(std::string) = 0;
    virtual void setParent(FileSystemElement*) = 0;
    virtual FileSystemElement* getParent() const = 0;
};

class File : public FileSystemElement {
private:
    size_t file_size;
public:
    File(const std::string& name, size_t size)
        : FileSystemElement(name), file_size(size) {
    }
    void displayInfo(int indent) const override {
        std::cout << std::string(indent, ' ') << "[File] " << name << " size: " << file_size << std::endl;
    }
    size_t getSize() const override { return file_size; }
    std::string getName() const override { return name; }
    void setName(std::string new_name) override { name = new_name; }
    FileSystemElement* getParent() const override { return parent; }
    void setParent(FileSystemElement* parent) override { parent = parent; }
};

class Folder : public FileSystemElement {
private:
    std::vector<std::unique_ptr<FileSystemElement>> elements;
    std::vector<std::unique_ptr<FileSystemElement>>::iterator
        searchLocalIter(std::string name) {
        auto it = find_if(elements.begin(), elements.end(),
            [&name](const auto& elem) { return elem->getName() == name; });
        return it;
    }

public:
    Folder(const std::string& name) : FileSystemElement(name) {}
    std::string getName() const override { return name; }
    void setName(std::string new_name) override { name = new_name; }
    FileSystemElement* getParent() const override { return parent; }
    void setParent(FileSystemElement* parent) override { parent = parent; }
    size_t getSize() const override {
        size_t total_size = 0;
        for (const auto& elem : elements)
            total_size += elem->getSize();
        return total_size;
    }

    /*
        // старый метод
        void addElement(std::unique_ptr<FileSystemElement> elem) {
            elements.push_back(std::move(elem));
        }
    */

    Folder* createFolder(std::string name) {
        auto new_folder = std::make_unique<Folder>(name);
        Folder* raw_ptr = new_folder.get();
        new_folder->setParent(this);
        elements.push_back(std::move(new_folder));
        return raw_ptr;
    }

    File* createFile(std::string name, size_t size) {
        auto new_file = std::make_unique<File>(name, size);
        File* raw_ptr = new_file.get();
        new_file->setParent(this);
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
        // return searchLocalIter(name)->get();
    }

    bool removeElement(std::string name) {
        auto it = searchLocalIter(name);
        if (it != elements.end()) {
            elements.erase(it);
            return true;
        }
        return false;
    }

};

std::unique_ptr<Folder> createTestFileSystem();

int main() {
    auto root = createTestFileSystem();
    root->displayInfo();
    if (root->searchLocal("documents")) std::cout << "Found!\n"; // находим элемент
    root->removeElement("documents");   // удаляем этот элемент
    root->displayInfo();
    root->removeElement("not_exist"); // при попытке удалить/найти
    // несуществующий элемент программа
    // работает стабильно
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