#include "FileSystem.h"

int main() {
    std::unique_ptr<Folder> root;
    root = root->createTestFileSystem();
    root->displayInfo();
    return 0;
}
