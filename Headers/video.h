#ifndef VIDEO_H
#define VIDEO_H

#include <filesystem>
#include <string>
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>

/*
EXPLINATION ON HOW TO LOAD VIDEOS:
Create a folder in the 'videos' directory that has a number as its name (the video index)
Fill that video with pngs for each frame, where each png is XXX-###.png, where Xs are any characters, and ### is a number (0 to num frames - 1)
Make the object that should display the video use an image called 'video_#_' as it's diffuse texture (the contents of the texture can be arbitrary)
The # is the video index (video folder name)
Make sure that the video starts playing
*/

struct Frame {
	Texture2D texture;
	int index;
};

class Video {
private:
	std::vector<Frame> mFrames;
	double mStartTime{ 0 };
	bool mIsPlaying{ false };
	int mFPS;

	const Frame& getFrameWithIndex(int index) const {
		const Frame* firstFrame = &(mFrames[0]); // could be wrong, but will probably get corrected in the loop
		for (const Frame& frame : mFrames) {
			if (frame.index == index) {
				return frame;
			}
			if (frame.index == 0) {
				firstFrame = &frame;
			}
		}
		return *firstFrame;
	}

	void play() {
		mIsPlaying = true;
		mStartTime = glfwGetTime();
	}

public:
	int mVideoIndex;
	Video(int videoIndex, int frameRate, bool shouldPlay = false) : mFPS{ frameRate }, mVideoIndex{ videoIndex } {
		std::string dir = "videos/" + std::to_string(videoIndex);
		for (const auto& dirEntry : std::filesystem::directory_iterator(dir)) {
			std::string filePath{ dirEntry.path().string() };
			size_t numIndex = filePath.find_last_of('-') + 1;
			size_t len = filePath.find_last_of('.') - numIndex;
			int frameIndex = std::stoi(filePath.substr(numIndex, len)) - 1;

			Frame frame{ Texture2D{ filePath }, frameIndex };
			mFrames.emplace_back(std::move(frame));
		};
		if (shouldPlay) {
			play();
		}
	}

	const TEX& getFrame() const {
		if (!mIsPlaying) {
			return getFrameWithIndex(0).texture.mTex;
		}
		double currentTime = glfwGetTime();
		double duration = currentTime - mStartTime;
		int currentFrameIndex = static_cast<int>(duration * mFPS) % mFrames.size();

		return getFrameWithIndex(currentFrameIndex).texture.mTex;
	}
};

#endif