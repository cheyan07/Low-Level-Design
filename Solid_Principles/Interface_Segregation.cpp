#include <iostream>
#include <vector>
#include <typeinfo>
#include <stdexcept>

using namespace std;


// Audio-only capabilities
class AudioPlayerControls {
public:
    virtual void playAudio(const string& audioFile) = 0;
    virtual void stopAudio() = 0;
    virtual void adjustAudioVolume(int volume) = 0;
    virtual ~AudioPlayerControls() = default;
};

// Video-only capabilities
class VideoPlayerControls {
public:
    virtual void playVideo(const string& videoFile) = 0;
    virtual void stopVideo() = 0;
    virtual void adjustVideoBrightness(int brightness) = 0;
    virtual void displaySubtitles(const string& subtitleFile) = 0;
    virtual ~VideoPlayerControls() = default;
};

class ModernAudioPlayer : public AudioPlayerControls {
public:
    void playAudio(const string& audioFile) override {
        cout << "ModernAudioPlayer: Playing audio - " << audioFile << endl;
    }

    void stopAudio() override {
        cout << "ModernAudioPlayer: Audio stopped." << endl;
    }

    void adjustAudioVolume(int volume) override {
        cout << "ModernAudioPlayer: Volume set to " << volume << endl;
    }
};

class SilentVideoPlayer : public VideoPlayerControls {
public:
    void playVideo(const string& videoFile) override {
        cout << "SilentVideoPlayer: Playing video - " << videoFile << endl;
    }

    void stopVideo() override {
        cout << "SilentVideoPlayer: Video stopped." << endl;
    }

    void adjustVideoBrightness(int brightness) override {
        cout << "SilentVideoPlayer: Brightness set to " << brightness << endl;
    }

    void displaySubtitles(const string& subtitleFile) override {
        cout << "SilentVideoPlayer: Subtitles from " << subtitleFile << endl;
    }
};
class ComprehensiveMediaPlayer : public AudioPlayerControls, public VideoPlayerControls {
public:
    void playAudio(const string& audioFile) override {
        cout << "ComprehensiveMediaPlayer: Playing audio - " << audioFile << endl;
    }

    void stopAudio() override {
        cout << "ComprehensiveMediaPlayer: Audio stopped." << endl;
    }

    void adjustAudioVolume(int volume) override {
        cout << "ComprehensiveMediaPlayer: Audio volume set to " << volume << endl;
    }

    void playVideo(const string& videoFile) override {
        cout << "ComprehensiveMediaPlayer: Playing video - " << videoFile << endl;
    }

    void stopVideo() override {
        cout << "ComprehensiveMediaPlayer: Video stopped." << endl;
    }

    void adjustVideoBrightness(int brightness) override {
        cout << "ComprehensiveMediaPlayer: Brightness set to " << brightness << endl;
    }

    void displaySubtitles(const string& subtitleFile) override {
        cout << "ComprehensiveMediaPlayer: Subtitles from " << subtitleFile << endl;
    }
};

int main() {

    // Audio-only player
    AudioPlayerControls* audioPlayer = new ModernAudioPlayer();

    audioPlayer->playAudio("song.mp3");
    audioPlayer->adjustAudioVolume(70);
    audioPlayer->stopAudio();

    cout << endl;

    // Video-only player
    VideoPlayerControls* videoPlayer = new SilentVideoPlayer();

    videoPlayer->playVideo("movie.mp4");
    videoPlayer->adjustVideoBrightness(80);
    videoPlayer->displaySubtitles("movie.srt");
    videoPlayer->stopVideo();

    cout << endl;

    // Comprehensive media player (supports both audio and video)
    ComprehensiveMediaPlayer* mediaPlayer = new ComprehensiveMediaPlayer();

    // Use as Audio Player
    AudioPlayerControls* audioInterface = mediaPlayer;
    audioInterface->playAudio("podcast.mp3");
    audioInterface->adjustAudioVolume(50);

    cout << endl;

    // Use as Video Player
    VideoPlayerControls* videoInterface = mediaPlayer;
    videoInterface->playVideo("documentary.mp4");
    videoInterface->adjustVideoBrightness(60);
    videoInterface->displaySubtitles("documentary.srt");

    cout << endl;

    // Cleanup
    delete audioPlayer;
    delete videoPlayer;
    delete mediaPlayer;

    return 0;
}

    
    
    