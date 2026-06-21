#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

class TextView {
public:
   virtual void render() = 0;
   virtual ~TextView() {}
};

class PlainTextView : public TextView {
private:
   string text;

public:
   PlainTextView(string text) : text(text) {}

   void render() override {
       cout << text;
   }
};

class TextDecorator : public TextView {
protected:
   TextView* inner;

public:
   TextDecorator(TextView* inner) : inner(inner) {}
};


class BoldDecorator : public TextDecorator {
public:
   BoldDecorator(TextView* inner) : TextDecorator(inner) {}

   void render() override {
       cout << "<b>";
       inner->render();
       cout << "</b>";
   }
};

class ItalicDecorator : public TextDecorator {
public:
   ItalicDecorator(TextView* inner) : TextDecorator(inner) {}

   void render() override {
       cout << "<i>";
       inner->render();
       cout << "</i>";
   }
};

class UnderlineDecorator : public TextDecorator {
public:
   UnderlineDecorator(TextView* inner) : TextDecorator(inner) {}

   void render() override {
       cout << "<u>";
       inner->render();
       cout << "</u>";
   }
};

int main() {
    PlainTextView text("Hello, World!");

    // Plain text
    cout << "Plain:                   ";
    text.render();
    cout << endl;

    // Single decorator: Bold
    cout << "Bold:                    ";
    BoldDecorator boldText(&text);
    boldText.render();
    cout << endl;

    // Two decorators: Italic + Underline
    cout << "Italic + Underline:      ";
    ItalicDecorator italic(&text);
    UnderlineDecorator italicUnderline(&italic);
    italicUnderline.render();
    cout << endl;

    // Three decorators: Bold + Italic + Underline
    cout << "Bold + Italic + Underline: ";
    BoldDecorator bold(&text);
    ItalicDecorator italicBold(&bold);
    UnderlineDecorator allStyles(&italicBold);
    allStyles.render();
    cout << endl;

    return 0;
}

// allStyles.render()
//         │
//         ▼
// +----------------------+
// | UnderlineDecorator   |
// | print "<u>"          |
// +----------------------+
//         │
//         ▼
// +----------------------+
// | ItalicDecorator      |
// | print "<i>"          |
// +----------------------+
//         │
//         ▼
// +----------------------+
// | BoldDecorator        |
// | print "<b>"          |
// +----------------------+
//         │
//         ▼
// +----------------------+
// | PlainTextView        |
// | print Hello,World!   |
// +----------------------+
//         │
//         ▼
// +----------------------+
// | BoldDecorator        |
// | print "</b>"         |
// +----------------------+
//         │
//         ▼
// +----------------------+
// | ItalicDecorator      |
// | print "</i>"         |
// +----------------------+
//         │
//         ▼
// +----------------------+
// | UnderlineDecorator   |
// | print "</u>"         |
// +----------------------+

// Output:

// <u><i><b>Hello, World!</b></i></u>