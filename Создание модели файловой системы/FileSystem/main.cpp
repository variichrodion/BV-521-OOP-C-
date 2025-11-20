#include "FileSystem.h"

int main() {
    FileSystem fs;
    // fs.getCurrentFolder()->displayInfo();
    fs.navigateToPath("/root/documents/projects");
    std::cout << fs.getCurrentFolder()->getPath() << '\n';
    return 0;
}
