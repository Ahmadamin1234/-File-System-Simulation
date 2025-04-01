#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class Date {
public:
    int day, month, year;
    Date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        day = ltm->tm_mday;
        month = 1 + ltm->tm_mon;
        year = 1900 + ltm->tm_year;
    }
    void displayDate() const { cout << day << "-" << month << "-" << year; }
};

class File {
public:
    string name, extension, content;
    double size; // Size in KB
    Date lastModificationDate;
    File* next;
    File* prev;

    File(string n, string ext) : name(n), extension(ext), content(""), size(0), next(nullptr), prev(nullptr) {}
};

class Directory {
public:
    string name;
    double size;
    Date lastModificationDate;
    Directory* left;
    Directory* right;
    File* files;

    Directory(string n) : name(n), size(0), left(nullptr), right(nullptr), files(nullptr) {}

    void updateSize() {
        size = 0;
        File* curr = files;
        while (curr) {
            size += curr->size;
            curr = curr->next;
        }
    }

    void addFile(string fileName, string ext) {
        File* newFile = new File(fileName, ext);
        if (!files) {
            files = newFile;
        }
        else {
            File* temp = files;
            while (temp->next) temp = temp->next;
            temp->next = newFile;
            newFile->prev = temp;
        }
        updateSize();
        cout << "File '" << fileName << "." << ext << "' added to directory '" << name << "'\n";
    }

    File* searchFile(const string& fileName) {
        File* temp = files;
        while (temp) {
            if (temp->name == fileName) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void removeFile(const string& fileName) {
        File* file = searchFile(fileName);
        if (!file) {
            cout << "File '" << fileName << "' not found!\n";
            return;
        }
        if (file->prev) file->prev->next = file->next;
        if (file->next) file->next->prev = file->prev;
        if (files == file) files = file->next;
        delete file;
        updateSize();
        cout << "File '" << fileName << "' removed successfully!\n";
    }
};

class DirectoryBST {
private:
    Directory* root;

    void insert(Directory*& node, Directory* dir) {
        if (!node) node = dir;
        else if (dir->name < node->name) insert(node->left, dir);
        else if (dir->name > node->name) insert(node->right, dir);
    }

    Directory* search(Directory* node, const string& name) {
        if (!node || node->name == name) return node;
        return name < node->name ? search(node->left, name) : search(node->right, name);
    }

    void inorderTraversal(Directory* node) {
        if (!node) return;
        inorderTraversal(node->left);
        cout << node->name << " ";
        inorderTraversal(node->right);
    }

public:
    DirectoryBST() { root = nullptr; }

    void displayMenu() {
        cout << "\nCommands:\n";
        cout << "cr <dirName> - Create Directory\n";
        cout << "cd <dirName> - Check Directory\n";
        cout << "addf <dirName> <fileName> <extension> - Add File to Directory\n";
        cout << "editf <dirName> <fileName> - Edit File Content\n";
        cout << "op <dirName> <fileName> - Open File\n";
        cout << "free <dirName> <fileName> - Delete File\n";
        cout << "dir - List All Directories\n";
        cout << "exit - Exit Program\n";
    }

    void createDirectory(const string& dirName) {
        Directory* newDir = new Directory(dirName);
        insert(root, newDir);
        cout << "Directory '" << dirName << "' created successfully!" << endl;
    }

    void checkDirectory(const string& dirName) {
        if (search(root, dirName)) cout << "Directory '" << dirName << "' exists!" << endl;
        else cout << "Directory not found!" << endl;
    }

    void listAllDirectories() {
        if (!root) {
            cout << "No directories available!" << endl;
            return;
        }
        cout << "Directories: ";
        inorderTraversal(root);
        cout << endl;
    }

    void addFileToDirectory(const string& dirName, const string& fileName, const string& ext) {
        Directory* dir = search(root, dirName);
        if (!dir) {
            cout << "Directory '" << dirName << "' not found!\n";
            return;
        }
        dir->addFile(fileName, ext);
    }

    void editFile(const string& dirName, const string& fileName) {
        Directory* dir = search(root, dirName);
        if (!dir) {
            cout << "Directory '" << dirName << "' not found!\n";
            return;
        }
        File* file = dir->searchFile(fileName);
        if (!file) {
            cout << "File '" << fileName << "' not found in directory '" << dirName << "'!\n";
            return;
        }
        cin.ignore();
        cout << "Enter new content for '" << fileName << "': ";
        string newContent;
        getline(cin, newContent);
        file->content = newContent;
        cout << "File content updated successfully!\n";
    }

    void openFile(const string& dirName, const string& fileName) {
        Directory* dir = search(root, dirName);
        if (!dir) {
            cout << "Directory '" << dirName << "' not found!\n";
            return;
        }
        File* file = dir->searchFile(fileName);
        if (!file) {
            cout << "File '" << fileName << "' not found in directory '" << dirName << "'!\n";
            return;
        }
        cout << "\nFile Content:\n" << file->content << "\n";
    }

    void freeFile(const string& dirName, const string& fileName) {
        Directory* dir = search(root, dirName);
        if (!dir) {
            cout << "Directory '" << dirName << "' not found!\n";
            return;
        }
        dir->removeFile(fileName);
    }
};

int main() {
    DirectoryBST bst;
    string command, dirName, fileName, ext;
    while (true) {
        bst.displayMenu();
        cout << "\nEnter command: ";
        cin >> command;
        if (command == "cr") {
            cin >> dirName;
            bst.createDirectory(dirName);
        }
        else if (command == "cd") {
            cin >> dirName;
            bst.checkDirectory(dirName);
        }
        else if (command == "addf") {
            cin >> dirName >> fileName >> ext;
            bst.addFileToDirectory(dirName, fileName, ext);
        }
        else if (command == "editf") {
            cin >> dirName >> fileName;
            bst.editFile(dirName, fileName);
        }
        else if (command == "op") {
            cin >> dirName >> fileName;
            bst.openFile(dirName, fileName);
        }
        else if (command == "free") {
            cin >> dirName >> fileName;
            bst.freeFile(dirName, fileName);
        }
        else if (command == "dir") {
            bst.listAllDirectories();
        }
        else if (command == "exit") break;
        else cout << "Invalid command!" << endl;
    }
    return 0;
}