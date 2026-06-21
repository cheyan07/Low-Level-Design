#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

class FileSystemItem {
public:
    virtual int getSize() = 0;
    virtual void printStructure(string indent) = 0;
    virtual void deleteItem() = 0;
    virtual ~FileSystemItem() {}
};

class File : public FileSystemItem {
private:
    string name;
    int size;

public:
    File(string name, int size) : name(name), size(size) {}

    int getSize() override {
        return size;
    }

    void printStructure(string indent) override {
        cout << indent << "- " << name << " (" << size << " KB)" << endl;
    }

    void deleteItem() override {
        cout << "Deleting file: " << name << endl;
    }
};

class Folder : public FileSystemItem {
private:
    string name;
    vector<FileSystemItem*> children;

public:
    Folder(string name) : name(name) {}

    void addItem(FileSystemItem* item) {
        children.push_back(item);
    }

    void removeItem(FileSystemItem* item) {
        children.erase(
            remove(children.begin(), children.end(), item),
            children.end()
        );
    }

    int getSize() override {
        int total = 0;
        for (FileSystemItem* item : children) {
            total += item->getSize();
        }
        return total;
    }

    void printStructure(string indent) override {
        cout << indent << "+ " << name << "/" << endl;
        for (FileSystemItem* item : children) {
            item->printStructure(indent + "  ");
        }
    }

    void deleteItem() override {
        for (FileSystemItem* item : children) {
            item->deleteItem();
        }
        cout << "Deleting folder: " << name << endl;
    }
};

int main() {
    FileSystemItem* file1 = new File("readme.txt", 5);
    FileSystemItem* file2 = new File("photo.jpg", 1500);
    FileSystemItem* file3 = new File("data.csv", 300);

    Folder* documents = new Folder("Documents");
    documents->addItem(file1);
    documents->addItem(file3);

    Folder* pictures = new Folder("Pictures");
    pictures->addItem(file2);

    Folder* home = new Folder("Home");
    home->addItem(documents);
    home->addItem(pictures);

    cout << "---- File Structure ----" << endl;
    home->printStructure("");

    cout << "\nTotal Size: " << home->getSize() << " KB" << endl;

    cout << "\n---- Deleting All ----" << endl;
    home->deleteItem();

    delete file1;
    delete file2;
    delete file3;
    delete documents;
    delete pictures;
    delete home;

    return 0;
}