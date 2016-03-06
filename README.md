#Audiologger

[![Build Status](https://travis-ci.org/malt3/Audiologger.svg?branch=master)](https://travis-ci.org/malt3/Audiologger)

Simple and elegant recording and playback program focused on a journal kind of voice recording.

Has a small codebase and simple file structure. However, this program is still in early alpha and should only used experimentally.
Recorded logs should be backuped regularly. If you want to help out, have a look at the todo list in [TODO.md](TODO.md) or just read some of the code (I am certain it can be drastically improved).

##Features (as of today)
* Can record 44100Hz Stereo Audio from your standard audio input (aka microphone) and save it as an uncompressed *.wav* file.
* Records as long as you wish (probably) or at least until the file gets too large.
* Can list all of your recorded audiologs with title and length ordered by the time of recording
* Can play any of the recorded audiologs in a separate thread
* Can stop playback before the end of the recording is reached.
* Can delete audiologs
* Gives info on what is playing at the moment

##Usage
Simply run `Audiologger` in your shell. If you want to record to a folder different to the default (`~/audiologs/`), give a file path as the first argument. The program itself is interactive. It has the following commands:
* `?` prints help
* `n` creates a new audiolog
* `l` lists all audilogs
* `p` plays an audiolog
* `i` displays info on what is currently playing
* `s` stops a running playback
* `e` edits metadata of an audiolog (not yet implemented)
* `q` quits the program

##Dependencies

##Building
Audiologger was successfully build on:
* Xcode 6.4 to 7.3
* Ubuntu 14.04 trusty
* Debian 8.3 jessy

### Mac OS X
Audiologger uses clang for compilation. On osx, the easiest way to get clang is via XCode. This can be installed from the [App Store](https://itunes.apple.com/en/app/xcode/id497799835) or betas from their [developer portal](https://developer.apple.com/xcode/download/). Just entering `xcode-select --install` will also allow you to install clang.

The dependencies can be installed via [homebrew](http://brew.sh/):
`/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"` 
`brew update` 
`brew install portaudio taglib libsndfile cmake git`

Then you can clone the git repo:
`git clone https://github.com/malt3/Audiologger.git && cd Audiologger`

Now you can build the project by entering 
`cmake .` 
`make`

The executable can be found at `bin/Audiologger
