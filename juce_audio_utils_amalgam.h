/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/


/*** Start of inlined file: juce_audio_utils.h ***/
#ifndef __JUCE_AUDIO_UTILS_JUCEHEADER__
#define __JUCE_AUDIO_UTILS_JUCEHEADER__

#include "juce_gui_basics_amalgam.h"
#include "juce_audio_devices_amalgam.h"
#include "juce_audio_formats_amalgam.h"
#include "juce_audio_processors_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE gui, players
#ifndef __JUCE_AUDIODEVICESELECTORCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_AudioDeviceSelectorComponent.h ***/
#ifndef __JUCE_AUDIODEVICESELECTORCOMPONENT_JUCEHEADER__
#define __JUCE_AUDIODEVICESELECTORCOMPONENT_JUCEHEADER__

/**
	A component containing controls to let the user change the audio settings of
	an AudioDeviceManager object.

	Very easy to use - just create one of these and show it to the user.

	@see AudioDeviceManager
*/
class JUCE_API  AudioDeviceSelectorComponent  : public Component,
												public ComboBoxListener, // (can't use ComboBox::Listener due to idiotic VC2005 bug)
												public ChangeListener
{
public:

	/** Creates the component.

		If your app needs only output channels, you might ask for a maximum of 0 input
		channels, and the component won't display any options for choosing the input
		channels. And likewise if you're doing an input-only app.

		@param deviceManager            the device manager that this component should control
		@param minAudioInputChannels    the minimum number of audio input channels that the application needs
		@param maxAudioInputChannels    the maximum number of audio input channels that the application needs
		@param minAudioOutputChannels   the minimum number of audio output channels that the application needs
		@param maxAudioOutputChannels   the maximum number of audio output channels that the application needs
		@param showMidiInputOptions     if true, the component will allow the user to select which midi inputs are enabled
		@param showMidiOutputSelector   if true, the component will let the user choose a default midi output device
		@param showChannelsAsStereoPairs    if true, channels will be treated as pairs; if false, channels will be
										treated as a set of separate mono channels.
		@param hideAdvancedOptionsWithButton    if true, only the minimum amount of UI components
										are shown, with an "advanced" button that shows the rest of them
	*/
	AudioDeviceSelectorComponent (AudioDeviceManager& deviceManager,
								  const int minAudioInputChannels,
								  const int maxAudioInputChannels,
								  const int minAudioOutputChannels,
								  const int maxAudioOutputChannels,
								  const bool showMidiInputOptions,
								  const bool showMidiOutputSelector,
								  const bool showChannelsAsStereoPairs,
								  const bool hideAdvancedOptionsWithButton);

	/** Destructor */
	~AudioDeviceSelectorComponent();

	/** @internal */
	void resized();
	/** @internal */
	void comboBoxChanged (ComboBox*);
	/** @internal */
	void changeListenerCallback (ChangeBroadcaster*);
	/** @internal */
	void childBoundsChanged (Component*);

private:

	AudioDeviceManager& deviceManager;
	ScopedPointer<ComboBox> deviceTypeDropDown;
	ScopedPointer<Label> deviceTypeDropDownLabel;
	ScopedPointer<Component> audioDeviceSettingsComp;
	String audioDeviceSettingsCompType;
	const int minOutputChannels, maxOutputChannels, minInputChannels, maxInputChannels;
	const bool showChannelsAsStereoPairs;
	const bool hideAdvancedOptionsWithButton;

	class MidiInputSelectorComponentListBox;
	friend class ScopedPointer<MidiInputSelectorComponentListBox>;
	ScopedPointer<MidiInputSelectorComponentListBox> midiInputsList;
	ScopedPointer<ComboBox> midiOutputSelector;
	ScopedPointer<Label> midiInputsLabel, midiOutputLabel;

	void updateAllControls();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioDeviceSelectorComponent);
};

#endif   // __JUCE_AUDIODEVICESELECTORCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_AudioDeviceSelectorComponent.h ***/


#endif
#ifndef __JUCE_AUDIOTHUMBNAIL_JUCEHEADER__

/*** Start of inlined file: juce_AudioThumbnail.h ***/
#ifndef __JUCE_AUDIOTHUMBNAIL_JUCEHEADER__
#define __JUCE_AUDIOTHUMBNAIL_JUCEHEADER__

class AudioThumbnailCache;

/**
	Makes it easy to quickly draw scaled views of the waveform shape of an
	audio file.

	To use this class, just create an AudioThumbNail class for the file you want
	to draw, call setSource to tell it which file or resource to use, then call
	drawChannel() to draw it.

	The class will asynchronously scan the wavefile to create its scaled-down view,
	so you should make your UI repaint itself as this data comes in. To do this, the
	AudioThumbnail is a ChangeBroadcaster, and will broadcast a message when its
	listeners should repaint themselves.

	The thumbnail stores an internal low-res version of the wave data, and this can
	be loaded and saved to avoid having to scan the file again.

	@see AudioThumbnailCache
*/
class JUCE_API  AudioThumbnail    : public ChangeBroadcaster,
									public AudioFormatWriter::ThreadedWriter::IncomingDataReceiver
{
public:

	/** Creates an audio thumbnail.

		@param sourceSamplesPerThumbnailSample  when creating a stored, low-res version
						of the audio data, this is the scale at which it should be done. (This
						number is the number of original samples that will be averaged for each
						low-res sample)
		@param formatManagerToUse   the audio format manager that is used to open the file
		@param cacheToUse   an instance of an AudioThumbnailCache - this provides a background
							thread and storage that is used to by the thumbnail, and the cache
							object can be shared between multiple thumbnails
	*/
	AudioThumbnail (int sourceSamplesPerThumbnailSample,
					AudioFormatManager& formatManagerToUse,
					AudioThumbnailCache& cacheToUse);

	/** Destructor. */
	~AudioThumbnail();

	/** Clears and resets the thumbnail. */
	void clear();

	/** Specifies the file or stream that contains the audio file.

		For a file, just call
		@code
		setSource (new FileInputSource (file))
		@endcode

		You can pass a zero in here to clear the thumbnail.
		The source that is passed in will be deleted by this object when it is no longer needed.
		@returns true if the source could be opened as a valid audio file, false if this failed for
		some reason.
	*/
	bool setSource (InputSource* newSource);

	/** Gives the thumbnail an AudioFormatReader to use directly.
		This will start parsing the audio in a background thread (unless the hash code
		can be looked-up successfully in the thumbnail cache). Note that the reader
		object will be held by the thumbnail and deleted later when no longer needed.
		The thumbnail will actually keep hold of this reader until you clear the thumbnail
		or change the input source, so the file will be held open for all this time. If
		you don't want the thumbnail to keep a file handle open continuously, you
		should use the setSource() method instead, which will only open the file when
		it needs to.
	*/
	void setReader (AudioFormatReader* newReader, int64 hashCode);

	/** Resets the thumbnail, ready for adding data with the specified format.
		If you're going to generate a thumbnail yourself, call this before using addBlock()
		to add the data.
	*/
	void reset (int numChannels, double sampleRate, int64 totalSamplesInSource = 0);

	/** Adds a block of level data to the thumbnail.
		Call reset() before using this, to tell the thumbnail about the data format.
	*/
	void addBlock (int64 sampleNumberInSource, const AudioSampleBuffer& newData,
				   int startOffsetInBuffer, int numSamples);

	/** Reloads the low res thumbnail data from an input stream.

		This is not an audio file stream! It takes a stream of thumbnail data that would
		previously have been created by the saveTo() method.
		@see saveTo
	*/
	void loadFrom (InputStream& input);

	/** Saves the low res thumbnail data to an output stream.

		The data that is written can later be reloaded using loadFrom().
		@see loadFrom
	*/
	void saveTo (OutputStream& output) const;

	/** Returns the number of channels in the file. */
	int getNumChannels() const noexcept;

	/** Returns the length of the audio file, in seconds. */
	double getTotalLength() const noexcept;

	/** Draws the waveform for a channel.

		The waveform will be drawn within  the specified rectangle, where startTime
		and endTime specify the times within the audio file that should be positioned
		at the left and right edges of the rectangle.

		The waveform will be scaled vertically so that a full-volume sample will fill
		the rectangle vertically, but you can also specify an extra vertical scale factor
		with the verticalZoomFactor parameter.
	*/
	void drawChannel (Graphics& g,
					  const Rectangle<int>& area,
					  double startTimeSeconds,
					  double endTimeSeconds,
					  int channelNum,
					  float verticalZoomFactor);

	/** Draws the waveforms for all channels in the thumbnail.

		This will call drawChannel() to render each of the thumbnail's channels, stacked
		above each other within the specified area.

		@see drawChannel
	*/
	void drawChannels (Graphics& g,
					   const Rectangle<int>& area,
					   double startTimeSeconds,
					   double endTimeSeconds,
					   float verticalZoomFactor);

	/** Returns true if the low res preview is fully generated. */
	bool isFullyLoaded() const noexcept;

	/** Returns the number of samples that have been set in the thumbnail. */
	int64 getNumSamplesFinished() const noexcept;

	/** Returns the highest level in the thumbnail.
		Note that because the thumb only stores low-resolution data, this isn't
		an accurate representation of the highest value, it's only a rough approximation.
	*/
	float getApproximatePeak() const;

	/** Reads the approximate min and max levels from a section of the thumbnail.
		The lowest and highest samples are returned in minValue and maxValue, but obviously
		because the thumb only stores low-resolution data, these numbers will only be a rough
		approximation of the true values.
	*/
	void getApproximateMinMax (double startTime, double endTime, int channelIndex,
							   float& minValue, float& maxValue) const noexcept;

	/** Returns the hash code that was set by setSource() or setReader(). */
	int64 getHashCode() const;

private:

	AudioFormatManager& formatManagerToUse;
	AudioThumbnailCache& cache;

	class LevelDataSource;
	struct MinMaxValue;
	class ThumbData;
	class CachedWindow;

	friend class LevelDataSource;
	friend class ScopedPointer<LevelDataSource>;
	friend class ThumbData;
	friend class OwnedArray<ThumbData>;
	friend class CachedWindow;
	friend class ScopedPointer<CachedWindow>;

	ScopedPointer<LevelDataSource> source;
	ScopedPointer<CachedWindow> window;
	OwnedArray<ThumbData> channels;

	int32 samplesPerThumbSample;
	int64 totalSamples, numSamplesFinished;
	int32 numChannels;
	double sampleRate;
	CriticalSection lock;

	void clearChannelData();
	bool setDataSource (LevelDataSource* newSource);
	void setLevels (const MinMaxValue* const* values, int thumbIndex, int numChans, int numValues);
	void createChannels (int length);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioThumbnail);
};

#endif   // __JUCE_AUDIOTHUMBNAIL_JUCEHEADER__

/*** End of inlined file: juce_AudioThumbnail.h ***/


#endif
#ifndef __JUCE_AUDIOTHUMBNAILCACHE_JUCEHEADER__

/*** Start of inlined file: juce_AudioThumbnailCache.h ***/
#ifndef __JUCE_AUDIOTHUMBNAILCACHE_JUCEHEADER__
#define __JUCE_AUDIOTHUMBNAILCACHE_JUCEHEADER__

/**
	An instance of this class is used to manage multiple AudioThumbnail objects.

	The cache runs a single background thread that is shared by all the thumbnails
	that need it, and it maintains a set of low-res previews in memory, to avoid
	having to re-scan audio files too often.

	@see AudioThumbnail
*/
class JUCE_API  AudioThumbnailCache   : public TimeSliceThread
{
public:

	/** Creates a cache object.

		The maxNumThumbsToStore parameter lets you specify how many previews should
		be kept in memory at once.
	*/
	explicit AudioThumbnailCache (int maxNumThumbsToStore);

	/** Destructor. */
	~AudioThumbnailCache();

	/** Clears out any stored thumbnails.
	*/
	void clear();

	/** Reloads the specified thumb if this cache contains the appropriate stored
		data.

		This is called automatically by the AudioThumbnail class, so you shouldn't
		normally need to call it directly.
	*/
	bool loadThumb (AudioThumbnail& thumb, int64 hashCode);

	/** Stores the cachable data from the specified thumb in this cache.

		This is called automatically by the AudioThumbnail class, so you shouldn't
		normally need to call it directly.
	*/
	void storeThumb (const AudioThumbnail& thumb, int64 hashCode);

	/** Attempts to re-load a saved cache of thumbnails from a stream.
		The cache data must have been written by the writeToStream() method.
		This will replace all currently-loaded thumbnails with the new data.
	*/
	bool readFromStream (InputStream& source);

	/** Writes all currently-loaded cache data to a stream.
		The resulting data can be re-loaded with readFromStream().
	*/
	void writeToStream (OutputStream& stream);

private:

	class ThumbnailCacheEntry;
	friend class OwnedArray<ThumbnailCacheEntry>;
	OwnedArray<ThumbnailCacheEntry> thumbs;
	CriticalSection lock;
	int maxNumThumbsToStore;

	ThumbnailCacheEntry* findThumbFor (int64 hash) const;
	int findOldestThumb() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioThumbnailCache);
};

#endif   // __JUCE_AUDIOTHUMBNAILCACHE_JUCEHEADER__

/*** End of inlined file: juce_AudioThumbnailCache.h ***/


#endif
#ifndef __JUCE_MIDIKEYBOARDCOMPONENT_JUCEHEADER__

/*** Start of inlined file: juce_MidiKeyboardComponent.h ***/
#ifndef __JUCE_MIDIKEYBOARDCOMPONENT_JUCEHEADER__
#define __JUCE_MIDIKEYBOARDCOMPONENT_JUCEHEADER__

/**
	A component that displays a piano keyboard, whose notes can be clicked on.

	This component will mimic a physical midi keyboard, showing the current state of
	a MidiKeyboardState object. When the on-screen keys are clicked on, it will play these
	notes by calling the noteOn() and noteOff() methods of its MidiKeyboardState object.

	Another feature is that the computer keyboard can also be used to play notes. By
	default it maps the top two rows of a standard querty keyboard to the notes, but
	these can be remapped if needed. It will only respond to keypresses when it has
	the keyboard focus, so to disable this feature you can call setWantsKeyboardFocus (false).

	The component is also a ChangeBroadcaster, so if you want to be informed when the
	keyboard is scrolled, you can register a ChangeListener for callbacks.

	@see MidiKeyboardState
*/
class JUCE_API  MidiKeyboardComponent  : public Component,
										 public MidiKeyboardStateListener,
										 public ChangeBroadcaster,
										 private Timer
{
public:

	/** The direction of the keyboard.

		@see setOrientation
	*/
	enum Orientation
	{
		horizontalKeyboard,
		verticalKeyboardFacingLeft,
		verticalKeyboardFacingRight,
	};

	/** Creates a MidiKeyboardComponent.

		@param state        the midi keyboard model that this component will represent
		@param orientation  whether the keyboard is horizonal or vertical
	*/
	MidiKeyboardComponent (MidiKeyboardState& state,
						   Orientation orientation);

	/** Destructor. */
	~MidiKeyboardComponent();

	/** Changes the velocity used in midi note-on messages that are triggered by clicking
		on the component.

		Values are 0 to 1.0, where 1.0 is the heaviest.

		@see setMidiChannel
	*/
	void setVelocity (float velocity, bool useMousePositionForVelocity);

	/** Changes the midi channel number that will be used for events triggered by clicking
		on the component.

		The channel must be between 1 and 16 (inclusive). This is the channel that will be
		passed on to the MidiKeyboardState::noteOn() method when the user clicks the component.

		Although this is the channel used for outgoing events, the component can display
		incoming events from more than one channel - see setMidiChannelsToDisplay()

		@see setVelocity
	*/
	void setMidiChannel (int midiChannelNumber);

	/** Returns the midi channel that the keyboard is using for midi messages.

		@see setMidiChannel
	*/
	int getMidiChannel() const noexcept                             { return midiChannel; }

	/** Sets a mask to indicate which incoming midi channels should be represented by
		key movements.

		The mask is a set of bits, where bit 0 = midi channel 1, bit 1 = midi channel 2, etc.

		If the MidiKeyboardState has a key down for any of the channels whose bits are set
		in this mask, the on-screen keys will also go down.

		By default, this mask is set to 0xffff (all channels displayed).

		@see setMidiChannel
	*/
	void setMidiChannelsToDisplay (int midiChannelMask);

	/** Returns the current set of midi channels represented by the component.

		This is the value that was set with setMidiChannelsToDisplay().
	*/
	int getMidiChannelsToDisplay() const noexcept                   { return midiInChannelMask; }

	/** Changes the width used to draw the white keys. */
	void setKeyWidth (float widthInPixels);

	/** Returns the width that was set by setKeyWidth(). */
	float getKeyWidth() const noexcept                              { return keyWidth; }

	/** Changes the keyboard's current direction. */
	void setOrientation (Orientation newOrientation);

	/** Returns the keyboard's current direction. */
	const Orientation getOrientation() const noexcept               { return orientation; }

	/** Sets the range of midi notes that the keyboard will be limited to.

		By default the range is 0 to 127 (inclusive), but you can limit this if you
		only want a restricted set of the keys to be shown.

		Note that the values here are inclusive and must be between 0 and 127.
	*/
	void setAvailableRange (int lowestNote,
							int highestNote);

	/** Returns the first note in the available range.

		@see setAvailableRange
	*/
	int getRangeStart() const noexcept                              { return rangeStart; }

	/** Returns the last note in the available range.

		@see setAvailableRange
	*/
	int getRangeEnd() const noexcept                                { return rangeEnd; }

	/** If the keyboard extends beyond the size of the component, this will scroll
		it to show the given key at the start.

		Whenever the keyboard's position is changed, this will use the ChangeBroadcaster
		base class to send a callback to any ChangeListeners that have been registered.
	*/
	void setLowestVisibleKey (int noteNumber);

	/** Returns the number of the first key shown in the component.

		@see setLowestVisibleKey
	*/
	int getLowestVisibleKey() const noexcept                        { return (int) firstKey; }

	/** Returns the length of the black notes.

		This will be their vertical or horizontal length, depending on the keyboard's orientation.
	*/
	int getBlackNoteLength() const noexcept                         { return blackNoteLength; }

	/** If set to true, then scroll buttons will appear at either end of the keyboard
		if there are too many notes to fit them all in the component at once.
	*/
	void setScrollButtonsVisible (bool canScroll);

	/** A set of colour IDs to use to change the colour of various aspects of the keyboard.

		These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		methods.

		@see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	*/
	enum ColourIds
	{
		whiteNoteColourId               = 0x1005000,
		blackNoteColourId               = 0x1005001,
		keySeparatorLineColourId        = 0x1005002,
		mouseOverKeyOverlayColourId     = 0x1005003,  /**< This colour will be overlaid on the normal note colour. */
		keyDownOverlayColourId          = 0x1005004,  /**< This colour will be overlaid on the normal note colour. */
		textLabelColourId               = 0x1005005,
		upDownButtonBackgroundColourId  = 0x1005006,
		upDownButtonArrowColourId       = 0x1005007
	};

	/** Returns the position within the component of the left-hand edge of a key.

		Depending on the keyboard's orientation, this may be a horizontal or vertical
		distance, in either direction.
	*/
	int getKeyStartPosition (const int midiNoteNumber) const;

	/** Deletes all key-mappings.

		@see setKeyPressForNote
	*/
	void clearKeyMappings();

	/** Maps a key-press to a given note.

		@param key                  the key that should trigger the note
		@param midiNoteOffsetFromC  how many semitones above C the triggered note should
									be. The actual midi note that gets played will be
									this value + (12 * the current base octave). To change
									the base octave, see setKeyPressBaseOctave()
	*/
	void setKeyPressForNote (const KeyPress& key,
							 int midiNoteOffsetFromC);

	/** Removes any key-mappings for a given note.

		For a description of what the note number means, see setKeyPressForNote().
	*/
	void removeKeyPressForNote (int midiNoteOffsetFromC);

	/** Changes the base note above which key-press-triggered notes are played.

		The set of key-mappings that trigger notes can be moved up and down to cover
		the entire scale using this method.

		The value passed in is an octave number between 0 and 10 (inclusive), and
		indicates which C is the base note to which the key-mapped notes are
		relative.
	*/
	void setKeyPressBaseOctave (int newOctaveNumber);

	/** This sets the octave number which is shown as the octave number for middle C.

		This affects only the default implementation of getWhiteNoteText(), which
		passes this octave number to MidiMessage::getMidiNoteName() in order to
		get the note text. See MidiMessage::getMidiNoteName() for more info about
		the parameter.

		By default this value is set to 3.

		@see getOctaveForMiddleC
	*/
	void setOctaveForMiddleC (int octaveNumForMiddleC);

	/** This returns the value set by setOctaveForMiddleC().
		@see setOctaveForMiddleC
	*/
	int getOctaveForMiddleC() const noexcept            { return octaveNumForMiddleC; }

	/** @internal */
	void paint (Graphics& g);
	/** @internal */
	void resized();
	/** @internal */
	void mouseMove (const MouseEvent& e);
	/** @internal */
	void mouseDrag (const MouseEvent& e);
	/** @internal */
	void mouseDown (const MouseEvent& e);
	/** @internal */
	void mouseUp (const MouseEvent& e);
	/** @internal */
	void mouseEnter (const MouseEvent& e);
	/** @internal */
	void mouseExit (const MouseEvent& e);
	/** @internal */
	void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
	/** @internal */
	void timerCallback();
	/** @internal */
	bool keyStateChanged (bool isKeyDown);
	/** @internal */
	void focusLost (FocusChangeType cause);
	/** @internal */
	void handleNoteOn (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity);
	/** @internal */
	void handleNoteOff (MidiKeyboardState* source, int midiChannel, int midiNoteNumber);
	/** @internal */
	void colourChanged();

protected:

	/** Draws a white note in the given rectangle.

		isOver indicates whether the mouse is over the key, isDown indicates whether the key is
		currently pressed down.

		When doing this, be sure to note the keyboard's orientation.
	*/
	virtual void drawWhiteNote (int midiNoteNumber,
								Graphics& g,
								int x, int y, int w, int h,
								bool isDown, bool isOver,
								const Colour& lineColour,
								const Colour& textColour);

	/** Draws a black note in the given rectangle.

		isOver indicates whether the mouse is over the key, isDown indicates whether the key is
		currently pressed down.

		When doing this, be sure to note the keyboard's orientation.
	*/
	virtual void drawBlackNote (int midiNoteNumber,
								Graphics& g,
								int x, int y, int w, int h,
								bool isDown, bool isOver,
								const Colour& noteFillColour);

	/** Allows text to be drawn on the white notes.

		By default this is used to label the C in each octave, but could be used for other things.

		@see setOctaveForMiddleC
	*/
	virtual String getWhiteNoteText (const int midiNoteNumber);

	/** Draws the up and down buttons that change the base note. */
	virtual void drawUpDownButton (Graphics& g, int w, int h,
								   const bool isMouseOver,
								   const bool isButtonPressed,
								   const bool movesOctavesUp);

	/** Callback when the mouse is clicked on a key.

		You could use this to do things like handle right-clicks on keys, etc.

		Return true if you want the click to trigger the note, or false if you
		want to handle it yourself and not have the note played.

		@see mouseDraggedToKey
	*/
	virtual bool mouseDownOnKey (int midiNoteNumber, const MouseEvent& e);

	/** Callback when the mouse is dragged from one key onto another.

		@see mouseDownOnKey
	*/
	virtual void mouseDraggedToKey (int midiNoteNumber, const MouseEvent& e);

	/** Calculates the positon of a given midi-note.

		This can be overridden to create layouts with custom key-widths.

		@param midiNoteNumber   the note to find
		@param keyWidth         the desired width in pixels of one key - see setKeyWidth()
		@param x                the x position of the left-hand edge of the key (this method
								always works in terms of a horizontal keyboard)
		@param w                the width of the key
	*/
	virtual void getKeyPosition (int midiNoteNumber, float keyWidth,
								 int& x, int& w) const;

private:

	friend class MidiKeyboardUpDownButton;

	MidiKeyboardState& state;
	int xOffset, blackNoteLength;
	float keyWidth;
	Orientation orientation;

	int midiChannel, midiInChannelMask;
	float velocity;

	Array<int> mouseOverNotes, mouseDownNotes;
	BigInteger keysPressed, keysCurrentlyDrawnDown;
	bool shouldCheckState;

	int rangeStart, rangeEnd;
	float firstKey;
	bool canScroll, useMousePositionForVelocity, shouldCheckMousePos;
	ScopedPointer<Button> scrollDown, scrollUp;

	Array <KeyPress> keyPresses;
	Array <int> keyPressNotes;
	int keyMappingOctave;
	int octaveNumForMiddleC;

	static const uint8 whiteNotes[];
	static const uint8 blackNotes[];

	void getKeyPos (int midiNoteNumber, int& x, int& w) const;
	int xyToNote (const Point<int>&, float& mousePositionVelocity);
	int remappedXYToNote (const Point<int>&, float& mousePositionVelocity) const;
	void resetAnyKeysInUse();
	void updateNoteUnderMouse (const Point<int>&, bool isDown, int fingerNum);
	void updateNoteUnderMouse (const MouseEvent&, bool isDown);
	void repaintNote (const int midiNoteNumber);
	void setLowestVisibleKeyFloat (float noteNumber);
	Rectangle<int> getWhiteNotePos (int noteNumber) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiKeyboardComponent);
};

#endif   // __JUCE_MIDIKEYBOARDCOMPONENT_JUCEHEADER__

/*** End of inlined file: juce_MidiKeyboardComponent.h ***/


#endif
#ifndef __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__

/*** Start of inlined file: juce_AudioProcessorPlayer.h ***/
#ifndef __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__
#define __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__


/*** Start of inlined file: juce_AudioProcessor.h ***/
#ifndef __JUCE_AUDIOPROCESSOR_JUCEHEADER__
#define __JUCE_AUDIOPROCESSOR_JUCEHEADER__


/*** Start of inlined file: juce_AudioProcessorEditor.h ***/
#ifndef __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__
#define __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__

class AudioProcessor;

/**
	Base class for the component that acts as the GUI for an AudioProcessor.

	Derive your editor component from this class, and create an instance of it
	by overriding the AudioProcessor::createEditor() method.

	@see AudioProcessor, GenericAudioProcessorEditor
*/
class JUCE_API  AudioProcessorEditor  : public Component
{
protected:

	/** Creates an editor for the specified processor.
	*/
	AudioProcessorEditor (AudioProcessor* owner);

public:
	/** Destructor. */
	~AudioProcessorEditor();

	/** Returns a pointer to the processor that this editor represents. */
	AudioProcessor* getAudioProcessor() const noexcept        { return owner; }

private:

	AudioProcessor* const owner;

	JUCE_DECLARE_NON_COPYABLE (AudioProcessorEditor);
};

#endif   // __JUCE_AUDIOPROCESSOREDITOR_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorEditor.h ***/


/*** Start of inlined file: juce_AudioProcessorListener.h ***/
#ifndef __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__
#define __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__

class AudioProcessor;

/**
	Base class for listeners that want to know about changes to an AudioProcessor.

	Use AudioProcessor::addListener() to register your listener with an AudioProcessor.

	@see AudioProcessor
*/
class JUCE_API  AudioProcessorListener
{
public:

	/** Destructor. */
	virtual ~AudioProcessorListener() {}

	/** Receives a callback when a parameter is changed.

		IMPORTANT NOTE: this will be called synchronously when a parameter changes, and
		many audio processors will change their parameter during their audio callback.
		This means that not only has your handler code got to be completely thread-safe,
		but it's also got to be VERY fast, and avoid blocking. If you need to handle
		this event on your message thread, use this callback to trigger an AsyncUpdater
		or ChangeBroadcaster which you can respond to on the message thread.
	*/
	virtual void audioProcessorParameterChanged (AudioProcessor* processor,
												 int parameterIndex,
												 float newValue) = 0;

	/** Called to indicate that something else in the plugin has changed, like its
		program, number of parameters, etc.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.
	*/
	virtual void audioProcessorChanged (AudioProcessor* processor) = 0;

	/** Indicates that a parameter change gesture has started.

		E.g. if the user is dragging a slider, this would be called when they first
		press the mouse button, and audioProcessorParameterChangeGestureEnd would be
		called when they release it.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.

		@see audioProcessorParameterChangeGestureEnd
	*/
	virtual void audioProcessorParameterChangeGestureBegin (AudioProcessor* processor,
															int parameterIndex);

	/** Indicates that a parameter change gesture has finished.

		E.g. if the user is dragging a slider, this would be called when they release
		the mouse button.

		IMPORTANT NOTE: this will be called synchronously, and many audio processors will
		call it during their audio callback. This means that not only has your handler code
		got to be completely thread-safe, but it's also got to be VERY fast, and avoid
		blocking. If you need to handle this event on your message thread, use this callback
		to trigger an AsyncUpdater or ChangeBroadcaster which you can respond to later on the
		message thread.

		@see audioProcessorParameterChangeGestureBegin
	*/
	virtual void audioProcessorParameterChangeGestureEnd (AudioProcessor* processor,
														  int parameterIndex);
};

#endif   // __JUCE_AUDIOPROCESSORLISTENER_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorListener.h ***/


/*** Start of inlined file: juce_AudioPlayHead.h ***/
#ifndef __JUCE_AUDIOPLAYHEAD_JUCEHEADER__
#define __JUCE_AUDIOPLAYHEAD_JUCEHEADER__

/**
	A subclass of AudioPlayHead can supply information about the position and
	status of a moving play head during audio playback.

	One of these can be supplied to an AudioProcessor object so that it can find
	out about the position of the audio that it is rendering.

	@see AudioProcessor::setPlayHead, AudioProcessor::getPlayHead
*/
class JUCE_API  AudioPlayHead
{
protected:

	AudioPlayHead() {}

public:
	virtual ~AudioPlayHead() {}

	/** Frame rate types. */
	enum FrameRateType
	{
		fps24           = 0,
		fps25           = 1,
		fps2997         = 2,
		fps30           = 3,
		fps2997drop     = 4,
		fps30drop       = 5,
		fpsUnknown      = 99
	};

	/** This structure is filled-in by the AudioPlayHead::getCurrentPosition() method.
	*/
	struct JUCE_API  CurrentPositionInfo
	{
		/** The tempo in BPM */
		double bpm;

		/** Time signature numerator, e.g. the 3 of a 3/4 time sig */
		int timeSigNumerator;
		/** Time signature denominator, e.g. the 4 of a 3/4 time sig */
		int timeSigDenominator;

		/** The current play position, in seconds from the start of the edit. */
		double timeInSeconds;

		/** For timecode, the position of the start of the edit, in seconds from 00:00:00:00. */
		double editOriginTime;

		/** The current play position in pulses-per-quarter-note.

			This is the number of quarter notes since the edit start.
		*/
		double ppqPosition;

		/** The position of the start of the last bar, in pulses-per-quarter-note.

			This is the number of quarter notes from the start of the edit to the
			start of the current bar.

			Note - this value may be unavailable on some hosts, e.g. Pro-Tools. If
			it's not available, the value will be 0.
		*/
		double ppqPositionOfLastBarStart;

		/** The video frame rate, if applicable. */
		FrameRateType frameRate;

		/** True if the transport is currently playing. */
		bool isPlaying;

		/** True if the transport is currently recording.

			(When isRecording is true, then isPlaying will also be true).
		*/
		bool isRecording;

		/** The current cycle start position in pulses-per-quarter-note.
			Note that not all hosts or plugin formats may provide this value.
			@see isLooping
		*/
		double ppqLoopStart;

		/** The current cycle end position in pulses-per-quarter-note.
			Note that not all hosts or plugin formats may provide this value.
			@see isLooping
		*/
		double ppqLoopEnd;

		/** True if the transport is currently looping. */
		bool isLooping;

		bool operator== (const CurrentPositionInfo& other) const noexcept;
		bool operator!= (const CurrentPositionInfo& other) const noexcept;

		void resetToDefault();
	};

	/** Fills-in the given structure with details about the transport's
		position at the start of the current processing block.
	*/
	virtual bool getCurrentPosition (CurrentPositionInfo& result) = 0;
};

#endif   // __JUCE_AUDIOPLAYHEAD_JUCEHEADER__

/*** End of inlined file: juce_AudioPlayHead.h ***/

/**
	Base class for audio processing filters or plugins.

	This is intended to act as a base class of audio filter that is general enough to
	be wrapped as a VST, AU, RTAS, etc, or used internally.

	It is also used by the plugin hosting code as the wrapper around an instance
	of a loaded plugin.

	Derive your filter class from this base class, and if you're building a plugin,
	you should implement a global function called createPluginFilter() which creates
	and returns a new instance of your subclass.
*/
class JUCE_API  AudioProcessor
{
protected:

	/** Constructor.

		You can also do your initialisation tasks in the initialiseFilterInfo()
		call, which will be made after this object has been created.
	*/
	AudioProcessor();

public:
	/** Destructor. */
	virtual ~AudioProcessor();

	/** Returns the name of this processor.
	*/
	virtual const String getName() const = 0;

	/** Called before playback starts, to let the filter prepare itself.

		The sample rate is the target sample rate, and will remain constant until
		playback stops.

		The estimatedSamplesPerBlock value is a HINT about the typical number of
		samples that will be processed for each callback, but isn't any kind
		of guarantee. The actual block sizes that the host uses may be different
		each time the callback happens, and may be more or less than this value.
	*/
	virtual void prepareToPlay (double sampleRate,
								int estimatedSamplesPerBlock) = 0;

	/** Called after playback has stopped, to let the filter free up any resources it
		no longer needs.
	*/
	virtual void releaseResources() = 0;

	/** Renders the next block.

		When this method is called, the buffer contains a number of channels which is
		at least as great as the maximum number of input and output channels that
		this filter is using. It will be filled with the filter's input data and
		should be replaced with the filter's output.

		So for example if your filter has 2 input channels and 4 output channels, then
		the buffer will contain 4 channels, the first two being filled with the
		input data. Your filter should read these, do its processing, and replace
		the contents of all 4 channels with its output.

		Or if your filter has 5 inputs and 2 outputs, the buffer will have 5 channels,
		all filled with data, and your filter should overwrite the first 2 of these
		with its output. But be VERY careful not to write anything to the last 3
		channels, as these might be mapped to memory that the host assumes is read-only!

		Note that if you have more outputs than inputs, then only those channels that
		correspond to an input channel are guaranteed to contain sensible data - e.g.
		in the case of 2 inputs and 4 outputs, the first two channels contain the input,
		but the last two channels may contain garbage, so you should be careful not to
		let this pass through without being overwritten or cleared.

		Also note that the buffer may have more channels than are strictly necessary,
		but your should only read/write from the ones that your filter is supposed to
		be using.

		The number of samples in these buffers is NOT guaranteed to be the same for every
		callback, and may be more or less than the estimated value given to prepareToPlay().
		Your code must be able to cope with variable-sized blocks, or you're going to get
		clicks and crashes!

		If the filter is receiving a midi input, then the midiMessages array will be filled
		with the midi messages for this block. Each message's timestamp will indicate the
		message's time, as a number of samples from the start of the block.

		Any messages left in the midi buffer when this method has finished are assumed to
		be the filter's midi output. This means that your filter should be careful to
		clear any incoming messages from the array if it doesn't want them to be passed-on.

		Be very careful about what you do in this callback - it's going to be called by
		the audio thread, so any kind of interaction with the UI is absolutely
		out of the question. If you change a parameter in here and need to tell your UI to
		update itself, the best way is probably to inherit from a ChangeBroadcaster, let
		the UI components register as listeners, and then call sendChangeMessage() inside the
		processBlock() method to send out an asynchronous message. You could also use
		the AsyncUpdater class in a similar way.
	*/
	virtual void processBlock (AudioSampleBuffer& buffer,
							   MidiBuffer& midiMessages) = 0;

	/** Returns the current AudioPlayHead object that should be used to find
		out the state and position of the playhead.

		You can call this from your processBlock() method, and use the AudioPlayHead
		object to get the details about the time of the start of the block currently
		being processed.

		If the host hasn't supplied a playhead object, this will return 0.
	*/
	AudioPlayHead* getPlayHead() const noexcept               { return playHead; }

	/** Returns the current sample rate.

		This can be called from your processBlock() method - it's not guaranteed
		to be valid at any other time, and may return 0 if it's unknown.
	*/
	double getSampleRate() const noexcept                     { return sampleRate; }

	/** Returns the current typical block size that is being used.

		This can be called from your processBlock() method - it's not guaranteed
		to be valid at any other time.

		Remember it's not the ONLY block size that may be used when calling
		processBlock, it's just the normal one. The actual block sizes used may be
		larger or smaller than this, and will vary between successive calls.
	*/
	int getBlockSize() const noexcept                         { return blockSize; }

	/** Returns the number of input channels that the host will be sending the filter.

		If writing a plugin, your configuration macros should specify the number of
		channels that your filter would prefer to have, and this method lets
		you know how many the host is actually using.

		Note that this method is only valid during or after the prepareToPlay()
		method call. Until that point, the number of channels will be unknown.
	*/
	int getNumInputChannels() const noexcept                  { return numInputChannels; }

	/** Returns the number of output channels that the host will be sending the filter.

		If writing a plugin, your configuration macros should specify the number of
		channels that your filter would prefer to have, and this method lets
		you know how many the host is actually using.

		Note that this method is only valid during or after the prepareToPlay()
		method call. Until that point, the number of channels will be unknown.
	*/
	int getNumOutputChannels() const noexcept                 { return numOutputChannels; }

	/** Returns the name of one of the input channels, as returned by the host.

		The host might not supply very useful names for channels, and this might be
		something like "1", "2", "left", "right", etc.
	*/
	virtual const String getInputChannelName (int channelIndex) const = 0;

	/** Returns the name of one of the output channels, as returned by the host.

		The host might not supply very useful names for channels, and this might be
		something like "1", "2", "left", "right", etc.
	*/
	virtual const String getOutputChannelName (int channelIndex) const = 0;

	/** Returns true if the specified channel is part of a stereo pair with its neighbour. */
	virtual bool isInputChannelStereoPair (int index) const = 0;

	/** Returns true if the specified channel is part of a stereo pair with its neighbour. */
	virtual bool isOutputChannelStereoPair (int index) const = 0;

	/** This returns the number of samples delay that the filter imposes on the audio
		passing through it.

		The host will call this to find the latency - the filter itself should set this value
		by calling setLatencySamples() as soon as it can during its initialisation.
	*/
	int getLatencySamples() const noexcept                            { return latencySamples; }

	/** The filter should call this to set the number of samples delay that it introduces.

		The filter should call this as soon as it can during initialisation, and can call it
		later if the value changes.
	*/
	void setLatencySamples (int newLatency);

	/** Returns true if the processor wants midi messages. */
	virtual bool acceptsMidi() const = 0;

	/** Returns true if the processor produces midi messages. */
	virtual bool producesMidi() const = 0;

	/** This returns a critical section that will automatically be locked while the host
		is calling the processBlock() method.

		Use it from your UI or other threads to lock access to variables that are used
		by the process callback, but obviously be careful not to keep it locked for
		too long, because that could cause stuttering playback. If you need to do something
		that'll take a long time and need the processing to stop while it happens, use the
		suspendProcessing() method instead.

		@see suspendProcessing
	*/
	const CriticalSection& getCallbackLock() const noexcept             { return callbackLock; }

	/** Enables and disables the processing callback.

		If you need to do something time-consuming on a thread and would like to make sure
		the audio processing callback doesn't happen until you've finished, use this
		to disable the callback and re-enable it again afterwards.

		E.g.
		@code
		void loadNewPatch()
		{
			suspendProcessing (true);

			..do something that takes ages..

			suspendProcessing (false);
		}
		@endcode

		If the host tries to make an audio callback while processing is suspended, the
		filter will return an empty buffer, but won't block the audio thread like it would
		do if you use the getCallbackLock() critical section to synchronise access.

		If you're going to use this, your processBlock() method must call isSuspended() and
		check whether it's suspended or not. If it is, then it should skip doing any real
		processing, either emitting silence or passing the input through unchanged.

		@see getCallbackLock
	*/
	void suspendProcessing (bool shouldBeSuspended);

	/** Returns true if processing is currently suspended.
		@see suspendProcessing
	*/
	bool isSuspended() const noexcept                                   { return suspended; }

	/** A plugin can override this to be told when it should reset any playing voices.

		The default implementation does nothing, but a host may call this to tell the
		plugin that it should stop any tails or sounds that have been left running.
	*/
	virtual void reset();

	/** Returns true if the processor is being run in an offline mode for rendering.

		If the processor is being run live on realtime signals, this returns false.
		If the mode is unknown, this will assume it's realtime and return false.

		This value may be unreliable until the prepareToPlay() method has been called,
		and could change each time prepareToPlay() is called.

		@see setNonRealtime()
	*/
	bool isNonRealtime() const noexcept                                 { return nonRealtime; }

	/** Called by the host to tell this processor whether it's being used in a non-realime
		capacity for offline rendering or bouncing.

		Whatever value is passed-in will be
	*/
	void setNonRealtime (bool isNonRealtime) noexcept;

	/** Creates the filter's UI.

		This can return 0 if you want a UI-less filter, in which case the host may create
		a generic UI that lets the user twiddle the parameters directly.

		If you do want to pass back a component, the component should be created and set to
		the correct size before returning it. If you implement this method, you must
		also implement the hasEditor() method and make it return true.

		Remember not to do anything silly like allowing your filter to keep a pointer to
		the component that gets created - it could be deleted later without any warning, which
		would make your pointer into a dangler. Use the getActiveEditor() method instead.

		The correct way to handle the connection between an editor component and its
		filter is to use something like a ChangeBroadcaster so that the editor can
		register itself as a listener, and be told when a change occurs. This lets them
		safely unregister themselves when they are deleted.

		Here are a few things to bear in mind when writing an editor:

		- Initially there won't be an editor, until the user opens one, or they might
		  not open one at all. Your filter mustn't rely on it being there.
		- An editor object may be deleted and a replacement one created again at any time.
		- It's safe to assume that an editor will be deleted before its filter.

		@see hasEditor
	*/
	virtual AudioProcessorEditor* createEditor() = 0;

	/** Your filter must override this and return true if it can create an editor component.
		@see createEditor
	*/
	virtual bool hasEditor() const = 0;

	/** Returns the active editor, if there is one.

		Bear in mind this can return 0, even if an editor has previously been
		opened.
	*/
	AudioProcessorEditor* getActiveEditor() const noexcept             { return activeEditor; }

	/** Returns the active editor, or if there isn't one, it will create one.

		This may call createEditor() internally to create the component.
	*/
	AudioProcessorEditor* createEditorIfNeeded();

	/** This must return the correct value immediately after the object has been
		created, and mustn't change the number of parameters later.
	*/
	virtual int getNumParameters() = 0;

	/** Returns the name of a particular parameter. */
	virtual const String getParameterName (int parameterIndex) = 0;

	/** Called by the host to find out the value of one of the filter's parameters.

		The host will expect the value returned to be between 0 and 1.0.

		This could be called quite frequently, so try to make your code efficient.
		It's also likely to be called by non-UI threads, so the code in here should
		be thread-aware.
	*/
	virtual float getParameter (int parameterIndex) = 0;

	/** Returns the value of a parameter as a text string. */
	virtual const String getParameterText (int parameterIndex) = 0;

	/** The host will call this method to change the value of one of the filter's parameters.

		The host may call this at any time, including during the audio processing
		callback, so the filter has to process this very fast and avoid blocking.

		If you want to set the value of a parameter internally, e.g. from your
		editor component, then don't call this directly - instead, use the
		setParameterNotifyingHost() method, which will also send a message to
		the host telling it about the change. If the message isn't sent, the host
		won't be able to automate your parameters properly.

		The value passed will be between 0 and 1.0.
	*/
	virtual void setParameter (int parameterIndex,
							   float newValue) = 0;

	/** Your filter can call this when it needs to change one of its parameters.

		This could happen when the editor or some other internal operation changes
		a parameter. This method will call the setParameter() method to change the
		value, and will then send a message to the host telling it about the change.

		Note that to make sure the host correctly handles automation, you should call
		the beginParameterChangeGesture() and endParameterChangeGesture() methods to
		tell the host when the user has started and stopped changing the parameter.
	*/
	void setParameterNotifyingHost (int parameterIndex,
									float newValue);

	/** Returns true if the host can automate this parameter.

		By default, this returns true for all parameters.
	*/
	virtual bool isParameterAutomatable (int parameterIndex) const;

	/** Should return true if this parameter is a "meta" parameter.

		A meta-parameter is a parameter that changes other params. It is used
		by some hosts (e.g. AudioUnit hosts).

		By default this returns false.
	*/
	virtual bool isMetaParameter (int parameterIndex) const;

	/** Sends a signal to the host to tell it that the user is about to start changing this
		parameter.

		This allows the host to know when a parameter is actively being held by the user, and
		it may use this information to help it record automation.

		If you call this, it must be matched by a later call to endParameterChangeGesture().
	*/
	void beginParameterChangeGesture (int parameterIndex);

	/** Tells the host that the user has finished changing this parameter.

		This allows the host to know when a parameter is actively being held by the user, and
		it may use this information to help it record automation.

		A call to this method must follow a call to beginParameterChangeGesture().
	*/
	void endParameterChangeGesture (int parameterIndex);

	/** The filter can call this when something (apart from a parameter value) has changed.

		It sends a hint to the host that something like the program, number of parameters,
		etc, has changed, and that it should update itself.
	*/
	void updateHostDisplay();

	/** Returns the number of preset programs the filter supports.

		The value returned must be valid as soon as this object is created, and
		must not change over its lifetime.

		This value shouldn't be less than 1.
	*/
	virtual int getNumPrograms() = 0;

	/** Returns the number of the currently active program.
	*/
	virtual int getCurrentProgram() = 0;

	/** Called by the host to change the current program.
	*/
	virtual void setCurrentProgram (int index) = 0;

	/** Must return the name of a given program. */
	virtual const String getProgramName (int index) = 0;

	/** Called by the host to rename a program.
	*/
	virtual void changeProgramName (int index, const String& newName) = 0;

	/** The host will call this method when it wants to save the filter's internal state.

		This must copy any info about the filter's state into the block of memory provided,
		so that the host can store this and later restore it using setStateInformation().

		Note that there's also a getCurrentProgramStateInformation() method, which only
		stores the current program, not the state of the entire filter.

		See also the helper function copyXmlToBinary() for storing settings as XML.

		@see getCurrentProgramStateInformation
	*/
	virtual void getStateInformation (juce::MemoryBlock& destData) = 0;

	/** The host will call this method if it wants to save the state of just the filter's
		current program.

		Unlike getStateInformation, this should only return the current program's state.

		Not all hosts support this, and if you don't implement it, the base class
		method just calls getStateInformation() instead. If you do implement it, be
		sure to also implement getCurrentProgramStateInformation.

		@see getStateInformation, setCurrentProgramStateInformation
	*/
	virtual void getCurrentProgramStateInformation (juce::MemoryBlock& destData);

	/** This must restore the filter's state from a block of data previously created
		using getStateInformation().

		Note that there's also a setCurrentProgramStateInformation() method, which tries
		to restore just the current program, not the state of the entire filter.

		See also the helper function getXmlFromBinary() for loading settings as XML.

		@see setCurrentProgramStateInformation
	*/
	virtual void setStateInformation (const void* data, int sizeInBytes) = 0;

	/** The host will call this method if it wants to restore the state of just the filter's
		current program.

		Not all hosts support this, and if you don't implement it, the base class
		method just calls setStateInformation() instead. If you do implement it, be
		sure to also implement getCurrentProgramStateInformation.

		@see setStateInformation, getCurrentProgramStateInformation
	*/
	virtual void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

	/** Adds a listener that will be called when an aspect of this processor changes. */
	void addListener (AudioProcessorListener* newListener);

	/** Removes a previously added listener. */
	void removeListener (AudioProcessorListener* listenerToRemove);

	/** Tells the processor to use this playhead object.
		The processor will not take ownership of the object, so the caller must delete it when
		it is no longer being used.
	*/
	void setPlayHead (AudioPlayHead* newPlayHead) noexcept;

	/** Not for public use - this is called before deleting an editor component. */
	void editorBeingDeleted (AudioProcessorEditor* editor) noexcept;

	/** Not for public use - this is called to initialise the processor before playing. */
	void setPlayConfigDetails (int numIns, int numOuts,
							   double sampleRate,
							   int blockSize) noexcept;

protected:

	/** Helper function that just converts an xml element into a binary blob.

		Use this in your filter's getStateInformation() method if you want to
		store its state as xml.

		Then use getXmlFromBinary() to reverse this operation and retrieve the XML
		from a binary blob.
	*/
	static void copyXmlToBinary (const XmlElement& xml,
								 juce::MemoryBlock& destData);

	/** Retrieves an XML element that was stored as binary with the copyXmlToBinary() method.

		This might return 0 if the data's unsuitable or corrupted. Otherwise it will return
		an XmlElement object that the caller must delete when no longer needed.
	*/
	static XmlElement* getXmlFromBinary (const void* data, int sizeInBytes);

	/** @internal */
	AudioPlayHead* playHead;

	/** @internal */
	void sendParamChangeMessageToListeners (int parameterIndex, float newValue);

private:
	Array <AudioProcessorListener*> listeners;
	Component::SafePointer<AudioProcessorEditor> activeEditor;
	double sampleRate;
	int blockSize, numInputChannels, numOutputChannels, latencySamples;
	bool suspended, nonRealtime;
	CriticalSection callbackLock, listenerLock;

   #if JUCE_DEBUG
	BigInteger changingParams;
   #endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessor);
};

#endif   // __JUCE_AUDIOPROCESSOR_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessor.h ***/

/**
	An AudioIODeviceCallback object which streams audio through an AudioProcessor.

	To use one of these, just make it the callback used by your AudioIODevice, and
	give it a processor to use by calling setProcessor().

	It's also a MidiInputCallback, so you can connect it to both an audio and midi
	input to send both streams through the processor.

	@see AudioProcessor, AudioProcessorGraph
*/
class JUCE_API  AudioProcessorPlayer    : public AudioIODeviceCallback,
										  public MidiInputCallback
{
public:

	AudioProcessorPlayer();

	/** Destructor. */
	virtual ~AudioProcessorPlayer();

	/** Sets the processor that should be played.

		The processor that is passed in will not be deleted or owned by this object.
		To stop anything playing, pass in 0 to this method.
	*/
	void setProcessor (AudioProcessor* processorToPlay);

	/** Returns the current audio processor that is being played.
	*/
	AudioProcessor* getCurrentProcessor() const                     { return processor; }

	/** Returns a midi message collector that you can pass midi messages to if you
		want them to be injected into the midi stream that is being sent to the
		processor.
	*/
	MidiMessageCollector& getMidiMessageCollector()                 { return messageCollector; }

	/** @internal */
	void audioDeviceIOCallback (const float** inputChannelData,
								int totalNumInputChannels,
								float** outputChannelData,
								int totalNumOutputChannels,
								int numSamples);
	/** @internal */
	void audioDeviceAboutToStart (AudioIODevice* device);
	/** @internal */
	void audioDeviceStopped();
	/** @internal */
	void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message);

private:

	AudioProcessor* processor;
	CriticalSection lock;
	double sampleRate;
	int blockSize;
	bool isPrepared;

	int numInputChans, numOutputChans;
	float* channels [128];
	AudioSampleBuffer tempBuffer;

	MidiBuffer incomingMidi;
	MidiMessageCollector messageCollector;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorPlayer);
};

#endif   // __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__

/*** End of inlined file: juce_AudioProcessorPlayer.h ***/


#endif
// END_AUTOINCLUDE

}

#endif   // __JUCE_AUDIO_UTILS_JUCEHEADER__

/*** End of inlined file: juce_audio_utils.h ***/

