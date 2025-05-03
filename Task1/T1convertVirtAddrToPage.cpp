#include <iostream>
#include <string>

const int KIBIBYTE = 1024;

int* convertVirtualAddress(int pageSize, int address);

int main() {
    
    std::string pageSizeStr;
    std::cout << "Enter a page size in kilobytes: ";
    std::cin >> pageSizeStr;
    std::cout << std::endl;

    int pageSize;
    pageSize = std::stoi(pageSizeStr);

    std::string virtAddr;
    std::cout << "Enter a virtual address: ";
    std::cin >> virtAddr;
    std::cout << std::endl;

    int address;
    address = std::stoi(virtAddr);

    int* pageNOffset;

    pageNOffset = convertVirtualAddress(pageSize, address);

    std::cout << "The adress " << virtAddr << " contains: page number= " << *pageNOffset << " offset= " << *(pageNOffset + 1) << std::endl; 

    return 0;
}

int* convertVirtualAddress(int pageSize, int address) {
    int* convertedAddress = new int[2];

    int pageSizeKB = (pageSize * KIBIBYTE);

    *(convertedAddress + 0) = address/pageSizeKB;

    *(convertedAddress + 1) = address % pageSizeKB;

    return convertedAddress;
}