#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <cstring>

/* == FileSystemElement START == */

class FileSystemElement {
protected:
    std::string name;
    FileSystemElement* parent = nullptr;
    FileSystemElement(const std::string&);              // конструктор элемента файловой системы
public:
    enum class Type { FILE, FOLDER };                   // класс перечисления (возможные типы элементов)
    virtual ~FileSystemElement() = default;             // деструктор
    virtual void displayInfo(int) const = 0;            // отобразить информацию
    virtual size_t getSize() const = 0;                 // получить размер
    virtual std::string getName() const = 0;            // получить название
    virtual void setName(std::string) = 0;              // установить название
    virtual FileSystemElement* getParent() const = 0;   // получить родительский
    virtual void setParent(FileSystemElement*) = 0;     // установить родительский
    virtual Type getType() const = 0;                   // получить тип
    virtual std::string getPath() const = 0;            // получить полный путь
};

/* == FileSystemElement END == */


/* == File START == */

class File : public FileSystemElement {
private:
    size_t file_size;
public:
    File(const std::string&, size_t size = 0);          // конструктор файла
    void displayInfo(int indent = 0) const override;    // отобразить информацию файла
    size_t getSize() const override;                    // получить размер файла
    std::string getName() const override;               // получить название файла
    void setName(std::string) override;                 // установить название файла
    FileSystemElement* getParent() const override;      // получить родительский файла
    void setParent(FileSystemElement*) override;        // установить родительский элемент файла
    FileSystemElement::Type getType() const override;   // получить тип "файл"
    std::string getPath() const override;               // получить полный путь файла
};

/* == File END == */

/* == Folder START == */

class Folder : public FileSystemElement {
private:
    std::vector<std::unique_ptr<FileSystemElement>> elements;
    // Поиск внутри директории (возвращается итератор на элемент)
    std::vector<std::unique_ptr<FileSystemElement>>::iterator searchLocalIter(std::string);

public:
    Folder(const std::string&);                         // конструктор директории
    void displayInfo(int indent = 0) const override;    // отобразить информацию директории
    size_t getSize() const override;                    // получить размер директории
    std::string getName() const override;               // получить название директории
    void setName(std::string) override;                 // установить название директории
    FileSystemElement* getParent() const override;      // получить родительский директории
    void setParent(FileSystemElement* parent) override; // установить родительский элемент директории
    FileSystemElement::Type getType() const override;   // получить тип "директория"
    std::string getPath() const override;               // получить полный путь директории

    /* операции над элементами системы */

    Folder* createFolder(std::string);                  // создать директорию (mkdir)
    File* createFile(std::string, size_t);              // создать файл (touch)
    FileSystemElement* searchLocal(const std::string&); // найти элемент внутри директории (findlocal)
    FileSystemElement* deepSearch(const std::string&);  // найти элемент во всех поддиректориях (finddeep)
    bool removeElement(const std::string&);             // удалить элемент в текущей директории (rm)
    void calculateElements(int&, int&);                 // подсчитать количество элементов (calcelem)
    std::unique_ptr<Folder> createTestFileSystem();     // Создать тестовую файловую систему

};

/* == Folder END == */