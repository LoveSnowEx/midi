#include "basic.h"

typedef enum _eEventType {
	MIDI_EVENT_NOTE_OFF = 0x80,
	MIDI_EVENT_NOTE_ON = 0x90,
	MIDI_EVENT_NOTE_AFTERTOUCH = 0xA0,
	MIDI_EVENT_CONTROLLER = 0xB0,
	MIDI_EVENT_PROGRAM_CHANGE = 0xC0,
	MIDI_EVENT_CHANNEL_AFTERTOUCH = 0xD0,
	MIDI_EVENT_PITCH_BLEND = 0xE0,
	SYSEX_EVENT_1 = 0xF0,
	SYSEX_EVENT_2 = 0xF7,
	META_EVENT = 0xFF,
} eEventType;

typedef enum _eMetaEventType {
	META_EVENT_SEQ_NUM = 0x00,             // 00 02  ss ss or 00 00
	META_EVENT_TEXT = 0x01,                // 01 len text
	META_EVENT_COPYRIGHT = 0x02,           // 02 len text
	META_EVENT_TRACK_NAME = 0x03,          // 03 len text
	META_EVENT_INSTRUMENT_NAME = 0x04,     // 04 len text
	META_EVENT_LYRIC = 0x05,               // 05 len text
	META_EVENT_MARKER = 0x06,              // 06 len text
	META_EVENT_CUE_POINT = 0x07,           // 07 len text
	META_EVENT_PROGRAM_NAME = 0x08,        // 08 len text
	META_EVENT_DEVICE_NAME = 0x09,         // 09 len text
	META_EVENT_CHANNEL_PREFIX = 0x20,      // 20 01  cc
	META_EVENT_MIDI_PORT = 0x21,           // 21 01  pp
	META_EVENT_END_OF_TRACK = 0x2F,        // 2F 00
	META_EVENT_TEMPO = 0x51,               // 51 03  tt tt tt
	META_EVENT_SMPTE_OFFSET = 0x54,        // 54 05  hr mn se fr ff
	META_EVENT_TIME_SIGNATURE = 0x58,      // 58 04  nn dd cc bb
	META_EVENT_KEY_SIGNATURE = 0x59,       // 59 02  sf mi
	META_EVENT_SEQUENCER_SPECIFIC = 0x7F,  // 7F len data
} eMetaEventType;

typedef struct _sDivision {
	union {
		uint16_t ticks_per_qtr;
		struct {
			uint8_t ticks_per_frame;
			uint8_t frames_per_sec;
		};
	};
} __attribute__((packed, scalar_storage_order("big-endian"))) sDivision;

typedef struct _sMthdChunk {
	// Here's the 8 byte header that all chunks must have
	char mthd[4];   // This will be 'M','T','h','d'
	uint32_t size;  // This will be 6

	// Here are the 6 bytes
	uint16_t format;     // 00, 01, 02
	uint16_t numTracks;  // how many MTrk chunks in this file
	sDivision division;  //
} __attribute__((packed, scalar_storage_order("big-endian"))) sMthdChunk;

typedef struct _sMidiEvent {
	// Here are the data bytes
	uint8_t *data;
}  sMidiEvent;

typedef struct _sSysExEvent {
	uint32_t size;  // This will be the actual size of data[]

	// Here are the data bytes
	uint8_t *data;
} sSysExEvent;

typedef struct _sMetaEvent {
	uint8_t type;   // Type of the meta event
	uint32_t size;  // This will be the actual size of data[]

	// Here are the data bytes
	uint8_t *data;
}  sMetaEvent;

typedef struct _sEventBlock {
	uint32_t deltaTime;  // This is referenced from the previous event.
	uint8_t type;        // Type of the event
	union _uEvent {
		sMidiEvent midiEvent;
		sSysExEvent sysExEvent;
		sMetaEvent metaEvent;
	} uEvent;
} __attribute__((packed, scalar_storage_order("big-endian"))) sEventBlock;

typedef struct _sMtrkChunk {
	// Here's the 8 byte header that all chunks must have
	char mtrk[4];   // This will be 'M','T','r','k'
	uint32_t size;  // This will be the actual size of events[]

	// Here are the events bytes
	sEventBlock *events;  // Its actual size is events[numEvent]
	uint32_t numEvent;   // Number of events in this chunk
} __attribute__((packed, scalar_storage_order("big-endian"))) sMtrkChunk;