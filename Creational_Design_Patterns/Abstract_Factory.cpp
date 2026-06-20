#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <algorithm> // for transform
#include <cctype>    // for tolower

using namespace std;


class Button {
public:
    virtual void paint() = 0;
    virtual void onClick() = 0;
    virtual ~Button() = default;
};

class Checkbox {
public:
    virtual void paint() = 0;
    virtual void onSelect() = 0;
    virtual ~Checkbox() {}
};

class WindowsButton : public Button {
public:
    void paint() override {
        cout << "Painting a Windows-style button." << endl;
    }

    void onClick() override {
        cout << "Windows button clicked." << endl;
    }
};

class WindowsCheckbox : public Checkbox {
public:
    void paint() override {
        cout << "Painting a Windows-style checkbox." << endl;
    }

    void onSelect() override {
        cout << "Windows checkbox selected." << endl;
    }
};

class MacOSButton : public Button {
public:
    void paint() override {
        cout << "Painting a macOS-style button." << endl;
    }

    void onClick() override {
        cout << "macOS button clicked." << endl;
    }
};

class MacOSCheckbox : public Checkbox {
public:
    void paint() override {
        cout << "Painting a macOS-style checkbox." << endl;
    }

    void onSelect() override {
        cout << "macOS checkbox selected." << endl;
    }
};

class GUIFactory {
public:
    virtual Button* createButton() = 0;
    virtual Checkbox* createCheckbox() = 0;
    virtual ~GUIFactory() {}
};

class WindowsFactory : public GUIFactory {
public:
    Button* createButton() override {
        return new WindowsButton();
    }
    Checkbox* createCheckbox() override {
        return new WindowsCheckbox();
    }
};

class MacOSFactory : public GUIFactory {
public:
    Button* createButton() override {
        return new MacOSButton();
    }
    Checkbox* createCheckbox() override {
        return new MacOSCheckbox();
    }
};

class Application {
private:
    Button* button;
    Checkbox* checkbox;

public:
    Application(GUIFactory* factory) {
        button = factory->createButton();
        checkbox = factory->createCheckbox();
    }

    ~Application() {
        delete button;
        delete checkbox;
    }

    void renderUI() {
        button->paint();
        checkbox->paint();
    }
};

int main() {
    string os;
    cout << "Enter OS (Windows/Mac): ";
    getline(cin, os);

    
    GUIFactory* factory = nullptr;

    // Simulated platform detection
    transform(os.begin(), os.end(), os.begin(), ::tolower);
    if (os.find("windows") != string::npos) {
        factory = new WindowsFactory();
    } else {
        factory = new MacOSFactory();
    }

    Application app(factory);
    app.renderUI();

    delete factory;

    return 0;
}

// # Abstract Factory Pattern Flow (Windows Example)

// ```
// main()
// │
// ├── User enters "Windows"
// │
// ├── factory = new WindowsFactory()
// │
// └──────────────┐
//                │
//                ▼
//       Application(factory)
//                │
//                │ Constructor
//                │
//       ┌────────┴────────┐
//       │                 │
//       ▼                 ▼

// factory->createButton()    factory->createCheckbox()
//       │                             │
//       ▼                             ▼
// WindowsFactory::createButton()   WindowsFactory::createCheckbox()
//       │                             │
//       ▼                             ▼
// new WindowsButton()             new WindowsCheckbox()
//       │                             │
//       ▼                             ▼
// button points to              checkbox points to
// WindowsButton object          WindowsCheckbox object


//                Memory after construction
//                -------------------------

//                  Application
//                  ┌──────────┐
// button ----------► WindowsButton
// checkbox --------► WindowsCheckbox
//                  └──────────┘


// main()
// │
// └── app.renderUI()
//         │
//         ├──────────────┐
//         │              │
//         ▼              ▼

// button->paint()      checkbox->paint()
//         │              │
//         ▼              ▼

// (dynamic dispatch)  (dynamic dispatch)
//         │              │
//         ▼              ▼

// WindowsButton::paint()    WindowsCheckbox::paint()
//         │                         │
//         ▼                         ▼

// "Painting a Windows-      "Painting a Windows-
//  style button."            style checkbox."


// ---------------------------------------------------------
// Complete Sequence
// ---------------------------------------------------------

// main()
// │
// ├── new WindowsFactory()
// │
// ├── Application(factory)
// │       │
// │       ├── createButton()
// │       │       │
// │       │       ▼
// │       │  WindowsFactory
// │       │       │
// │       │       ▼
// │       │  WindowsButton
// │       │
// │       └── createCheckbox()
// │               │
// │               ▼
// │          WindowsFactory
// │               │
// │               ▼
// │          WindowsCheckbox
// │
// └── renderUI()
//         │
//         ├── button->paint()
//         │       │
//         │       ▼
//         │  WindowsButton::paint()
//         │
//         └── checkbox->paint()
//                 │
//                 ▼
//            WindowsCheckbox::paint()
// ```

// ## Relationship Diagram

// ```
//                      GUIFactory
//                    (Abstract Factory)
//                     /            \
//                    /              \
//                   ▼                ▼
//           WindowsFactory      MacOSFactory
//                  │                  │
//          ┌───────┴───────┐  ┌───────┴───────┐
//          ▼               ▼  ▼               ▼
//    WindowsButton  WindowsCheckbox  MacOSButton  MacOSCheckbox
//          ▲               ▲             ▲             ▲
//          └───────┬───────┘             └───────┬─────┘
//                  │                             │
//                  └──────── Application ────────┘

// Application knows only:
//     Button*
//     Checkbox*

// It never knows whether they are Windows or Mac objects.
// ```
