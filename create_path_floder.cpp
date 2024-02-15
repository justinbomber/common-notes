#include <iostream>
#include <string>
#include <sys/stat.h> // For stat()
#include <sstream>
#include <vector>

// 函數用來檢查並創建資料夾
void createDirectoryIfNotExist(const std::string& path) {
    struct stat info;

    // 檢查路徑是否存在
    if (stat(path.c_str(), &info) != 0) {
        // 路徑不存在，創建資料夾
        int status = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status == 0) {
            std::cout << "Created directory: " << path << std::endl;
        } else {
            std::cerr << "Unable to create directory: " << path << std::endl;
        }
    } else if (info.st_mode & S_IFDIR) {
        // 路徑存在且是一個資料夾
        std::cout << "Directory already exists: " << path << std::endl;
    } else {
        // 路徑存在但不是資料夾
        std::cerr << "Path exists but is not a directory: " << path << std::endl;
    }
}

// 主函數，接收一個路徑字符串，檢查並創建不存在的資料夾
void checkAndCreateDirectories(const std::string& inputPath) {
    std::istringstream iss(inputPath);
    std::string part;
    std::vector<std::string> parts;
    std::string currentPath;

    // 以"/"為分割點拆分路徑
    while (getline(iss, part, '/')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }

    // 逐一檢查路徑的每一部分
    for (const std::string& folder : parts) {
        currentPath += folder + "/";
        createDirectoryIfNotExist(currentPath);
    }
}

// 測試函數
int main() {
    std::string path = "your/path/here"; // 替換為你的路徑
    checkAndCreateDirectories(path);
    return 0;
}

