#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

class CharacterFlyweight {
public:
    virtual ~CharacterFlyweight() {}
    virtual void draw(int x, int y) = 0;
};

class CharacterGlyph : public CharacterFlyweight {
private:
    char symbol;
    string fontFamily;
    int fontSize;
    string color;

public:
    CharacterGlyph(char symbol, const string& fontFamily, int fontSize, const string& color)
        : symbol(symbol), fontFamily(fontFamily), fontSize(fontSize), color(color) {}

    void draw(int x, int y) override {
        cout << "Drawing '" << symbol << "' [Font: " << fontFamily 
             << ", Size: " << fontSize << ", Color: " << color 
             << "] at (" << x << "," << y << ")" << endl;
    }
};

class CharacterFlyweightFactory {
private:
    map<string, CharacterFlyweight*> flyweightMap;

public:
    CharacterFlyweight* getFlyweight(char symbol, const string& fontFamily, int fontSize, const string& color) {
        string key = string(1, symbol) + fontFamily + to_string(fontSize) + color;
        
        if (flyweightMap.find(key) == flyweightMap.end()) {
            flyweightMap[key] = new CharacterGlyph(symbol, fontFamily, fontSize, color);
        }
        
        return flyweightMap[key];
    }

    int getFlyweightCount() {
        return flyweightMap.size();
    }

    ~CharacterFlyweightFactory() {
        for (auto& pair : flyweightMap) {
            delete pair.second;
        }
    }
};

class RenderedCharacter {
private:
    CharacterFlyweight* glyph;
    int x, y;

public:
    RenderedCharacter(CharacterFlyweight* glyph, int x, int y) : glyph(glyph), x(x), y(y) {}

    void render() {
        glyph->draw(x, y);
    }
};

class TextEditorClient {
private:
    CharacterFlyweightFactory factory;
    vector<RenderedCharacter> document;

public:
    void addCharacter(char c, int x, int y, const string& font, int size, const string& color) {
        CharacterFlyweight* glyph = factory.getFlyweight(c, font, size, color);
        document.push_back(RenderedCharacter(glyph, x, y));
    }

    void renderDocument() {
        for (RenderedCharacter& rc : document) {
            rc.render();
        }
        cout << "Total flyweight objects used: " << factory.getFlyweightCount() << endl;
    }
};

void flyweightDemo() {
    TextEditorClient editor;

    // Render "Hello" with same style
    string word = "Hello";
    for (int i = 0; i < word.length(); i++) {
        editor.addCharacter(word[i], 10 + i * 15, 50, "Arial", 14, "#000000");
    }

    // Render "World" with different font and color
    string word2 = "World";
    for (int i = 0; i < word2.length(); i++) {
        editor.addCharacter(word2[i], 10 + i * 15, 100, "Times New Roman", 14, "#3333FF");
    }

    editor.renderDocument();
}

int main() {
    flyweightDemo();
    return 0;
}
