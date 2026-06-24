#include <bits/stdc++.h>
using namespace std;

template<typename T>
class Iterator {
public:
    virtual ~Iterator() {}
    virtual bool hasNext() = 0;
    virtual T next() = 0;
};

template<typename T>
class IterableCollection {
public:
    virtual ~IterableCollection() {}
    virtual Iterator<T>* createIterator() = 0;
};

// Forward declaration
class PlaylistIterator;

class Playlist : public IterableCollection<string> {
private:
    vector<string> songs;

public:
    void addSong(const string& song) {
        songs.push_back(song);
    }

    string getSongAt(int index) const {
        return songs[index];
    }

    int getSize() const {
        return songs.size();
    }

    Iterator<string>* createIterator() override;
};

class PlaylistIterator : public Iterator<string> {
private:
    Playlist* playlist;
    int index;

public:
    PlaylistIterator(Playlist* pl)
        : playlist(pl), index(0) {}

    bool hasNext() override {
        return index < playlist->getSize();
    }

    string next() override {
        string song = playlist->getSongAt(index);
        index++;
        return song;
    }
};

Iterator<string>* Playlist::createIterator() {
    return new PlaylistIterator(this);
}

void musicPlayerDemo() {
    Playlist playlist;

    playlist.addSong("Shape of You");
    playlist.addSong("Bohemian Rhapsody");
    playlist.addSong("Blinding Lights");

    Iterator<string>* iterator = playlist.createIterator();

    cout << "Now Playing:\n";
    while (iterator->hasNext()) {
        cout << "🎵 " << iterator->next() << endl;
    }

    delete iterator;
}

int main() {
    musicPlayerDemo();
    return 0;
}