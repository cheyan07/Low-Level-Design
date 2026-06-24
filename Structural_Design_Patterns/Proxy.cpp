#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

class Image {
public:
    virtual void display() = 0;
    virtual string getFileName() = 0;
    virtual ~Image() {}
};

class HighResolutionImage : public Image {
private:
    string fileName;
    char* imageData;

    void loadImageFromDisk() {
        cout << "Loading image: " << fileName << " from disk (Expensive Operation)..." << endl;
        imageData = new char[10 * 1024 * 1024];
        cout << "Image " << fileName << " loaded successfully." << endl;
    }

public:
    HighResolutionImage(string fileName) : fileName(fileName), imageData(nullptr) {
        loadImageFromDisk();
    }

    ~HighResolutionImage() {
        delete[] imageData;
    }

    void display() override {
        cout << "Displaying image: " << fileName << endl;
    }

    string getFileName() override {
        return fileName;
    }
};

class ImageProxy : public Image {
private:
    string fileName;
    HighResolutionImage* realImage;

public:
    ImageProxy(string fileName) : fileName(fileName), realImage(nullptr) {
        cout << "ImageProxy: Created for " << fileName << ". Real image not loaded yet." << endl;
    }

    ~ImageProxy() {
        delete realImage;
    }

    string getFileName() override {
        return fileName;
    }

    void display() override {
        if (realImage == nullptr) {
            cout << "ImageProxy: display() requested for " << fileName << ". Loading high-resolution image..." << endl;
            realImage = new HighResolutionImage(fileName);
        } else {
            cout << "ImageProxy: Using cached high-resolution image for " << fileName << endl;
        }
        realImage->display();
    }
};
int main() {
    cout << "Application Started. Initializing image proxies for gallery..." << endl;

    Image* image1 = new ImageProxy("photo1.jpg");
    Image* image2 = new ImageProxy("photo2.png");
    Image* image3 = new ImageProxy("photo3.gif");

    cout << "\nGallery initialized. No images actually loaded yet." << endl;
    cout << "Image 1 Filename: " << image1->getFileName() << endl;

    cout << "\nUser requests to display " << image1->getFileName() << endl;
    image1->display();

    cout << "\nUser requests to display " << image1->getFileName() << " again." << endl;
    image1->display();

    cout << "\nUser requests to display " << image3->getFileName() << endl;
    image3->display();

    cout << "\nApplication finished. Note: photo2.png was never loaded." << endl;

    delete image1;
    delete image2;
    delete image3;
    return 0;
}