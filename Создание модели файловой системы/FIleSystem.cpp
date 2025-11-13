#include <iostream>
#include <vector>
#include <string>
#include <memory>

// Абстрактный базовый класс для всех элементов файловой системы
class FileSystemElement {
public:
    FileSystemElement(const std::string& name) : name_(name) {}
    virtual ~FileSystemElement() = default;

    // Виртуальная функция для отображения информации (полиморфизм)
    virtual void display(int indent = 0) const = 0;
    virtual size_t getSize() const = 0;

protected:
    std::string name_;
};

// Класс "Файл"
class File : public FileSystemElement {
public:
    File(const std::string& name, size_t size)
        : FileSystemElement(name), size_(size) {
    }

    void display(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "[File] " << name_
            << " (" << size_ << " bytes)" << std::endl;
    }

    size_t getSize() const override { return size_; }

private:
    size_t size_;
};

// Класс "Папка" (композиция)
class Folder : public FileSystemElement {
public:
    Folder(const std::string& name) : FileSystemElement(name) {}

    void addElement(std::unique_ptr<FileSystemElement> element) {
        elements_.push_back(std::move(element));
    }

    void display(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "[Folder] " << name_ << std::endl;
        // Рекурсивный вызов display для всех вложенных элементов
        for (const auto& elem : elements_) {
            elem->display(indent + 2); // Увеличиваем отступ для вложенных элементов
        }
    }

    size_t getSize() const override {
        size_t totalSize = 0;
        for (const auto& elem : elements_) {
            totalSize += elem->getSize();
        }
        return totalSize;
    }

private:
    std::vector<std::unique_ptr<FileSystemElement>> elements_; // Композиция: папка содержит элементы
};

// Пример использования
int main() {
    auto root = std::make_unique<Folder>("Root");
    auto file1 = std::make_unique<File>("document.txt", 1024);
    auto file2 = std::make_unique<File>("image.jpg", 2048);

    auto subFolder = std::make_unique<Folder>("My Documents");
    auto file3 = std::make_unique<File>("notes.txt", 512);

    subFolder->addElement(std::move(file3));
    root->addElement(std::move(file1));
    root->addElement(std::move(file2));
    root->addElement(std::move(subFolder));

    root->display();
    std::cout << "Total size: " << root->getSize() << " bytes" << std::endl;

    return 0;
}
