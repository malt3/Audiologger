#Audiologger ToDo List

##Recording engine
* Support for different levels of recording (framerates other than 44100Hz)
* Support for different amounts of channels like mono or more than two (now it is always 2)
* Add ability to choose input device
* Support for lossy encodings like mp3 or aac
 * Problems: Codecs have non free licenses and real time encoding could be tricky

##Playback engine
* Support for different encodings (see above)
* Add ability to choose output device
* Make volume adjustable
* Add relative playback progress (%)
* Make it stop automatically when playback is over (see Program strucure)

##File interaction
* better buffering to have fewer reads and writes
* file path sanity checks!! (eg don't overwrite existing logs. Check if a filepath makes sense)
* correctly interpret file system vars like `~`
* encryption 
    * probably AES

##Metadata
* Own file with metadata?
* encode recording date (and maybe exact time)
* support for tagging
* transskript support
* rating???
* make this data accesible and changeable for the user (searching, sorting, filtering, editing)
* maybe make use of a settings file (for preferences like paths, fav encoding, naming schemes, framerate..)

##User interaction
* Increase the level of input validation
* Solve problems with getchar (ENTER of last input gets read)
* Streamline output (more friendly and clear sentences)
* Make scriptable
* Add version number to make changes easier visible

##Program structure
* Make the Recorder and Player class inherit from the same baseclass
* Get rid of or restructure logging code
* Create callback for Player and Recorder when event state changes (playback is over, file problems...)
* Write tests
* Improve comments (describe behavior of classes and functions)
* Scan for memory leaks
* Port to windows/linux (linux might work out of the box)

##*"Marketing"*
* Spread the word
* Publish to GitHub
* Get testers
* Get commits
* Add license