#include <gtest-wrapper.h>

#include <MIDI_Parsers/SerialMIDI_Parser.hpp>
#include <MIDI_Parsers/USBMIDI_Parser.hpp>

using namespace CS;

using SysExVector = std::vector<uint8_t>;

// ---------------------------- USB PARSER TESTS ---------------------------- //

TEST(USBMIDIParser, noteOff) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x08, 0x82, 0x20, 0x7F};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = uparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x82);
    EXPECT_EQ(msg.data1, 0x20);
    EXPECT_EQ(msg.data2, 0x7F);
}

TEST(USBMIDIParser, noteOn) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x09, 0x93, 0x2A, 0x7E};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = uparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x93);
    EXPECT_EQ(msg.data1, 0x2A);
    EXPECT_EQ(msg.data2, 0x7E);
}

TEST(USBMIDIParser, noteOnCN) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x59, 0x93, 0x2A, 0x7E};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = uparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x93);
    EXPECT_EQ(msg.data1, 0x2A);
    EXPECT_EQ(msg.data2, 0x7E);
    EXPECT_EQ(msg.CN, 0x05);
}

TEST(USBMIDIParser, sysEx2Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x06, 0xF0, 0xF7, 0x00};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExLength(), 2);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 2);
    const SysExVector expected = {0xF0, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(USBMIDIParser, sysEx3Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x07, 0xF0, 0x10, 0xF7};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExLength(), 3);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 3);
    const SysExVector expected = {0xF0, 0x10, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(USBMIDIParser, sysEx4Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet1[4] = {0x04, 0xF0, 0x11, 0x12};
    EXPECT_EQ(uparser.parse(packet1), MIDIReadEvent::NO_MESSAGE);
    uint8_t packet2[4] = {0x05, 0xF7, 0x00, 0x00};
    EXPECT_EQ(uparser.parse(packet2), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExLength(), 4);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 4);
    const SysExVector expected = {0xF0, 0x11, 0x12, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(USBMIDIParser, sysEx5Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet1[4] = {0x04, 0xF0, 0x21, 0x22};
    EXPECT_EQ(uparser.parse(packet1), MIDIReadEvent::NO_MESSAGE);
    uint8_t packet2[4] = {0x06, 0x23, 0xF7, 0x00};
    EXPECT_EQ(uparser.parse(packet2), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExLength(), 5);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 5);
    const SysExVector expected = {0xF0, 0x21, 0x22, 0x23, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(USBMIDIParser, sysEx6Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet1[4] = {0x04, 0xF0, 0x31, 0x32};
    EXPECT_EQ(uparser.parse(packet1), MIDIReadEvent::NO_MESSAGE);
    uint8_t packet2[4] = {0x07, 0x33, 0x34, 0xF7};
    EXPECT_EQ(uparser.parse(packet2), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExLength(), 6);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 6);
    const SysExVector expected = {0xF0, 0x31, 0x32, 0x33, 0x34, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(USBMIDIParser, sysEx7Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet1[4] = {0x04, 0xF0, 0x41, 0x42};
    EXPECT_EQ(uparser.parse(packet1), MIDIReadEvent::NO_MESSAGE);
    uint8_t packet2[4] = {0x04, 0x43, 0x44, 0x45};
    EXPECT_EQ(uparser.parse(packet2), MIDIReadEvent::NO_MESSAGE);
    uint8_t packet3[4] = {0x05, 0xF7, 0x00, 0x00};
    EXPECT_EQ(uparser.parse(packet3), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExLength(), 7);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 7);
    const SysExVector expected = {0xF0, 0x41, 0x42, 0x43, 0x44, 0x45, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(USBMIDIParser, sysEx12Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet1[4] = {0x04, 0xF0, 0x51, 0x52};
    EXPECT_EQ(uparser.parse(packet1), MIDIReadEvent::NO_MESSAGE);
    uint8_t packet2[4] = {0x04, 0x53, 0x54, 0x55};
    EXPECT_EQ(uparser.parse(packet2), MIDIReadEvent::NO_MESSAGE);
    uint8_t packet3[4] = {0x04, 0x56, 0x57, 0x58};
    EXPECT_EQ(uparser.parse(packet3), MIDIReadEvent::NO_MESSAGE);
    uint8_t packet4[4] = {0x07, 0x59, 0x5A, 0xF7};
    EXPECT_EQ(uparser.parse(packet4), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(uparser.getSysExLength(), 12);
    const SysExVector result(uparser.getSysExBuffer(),
                             uparser.getSysExBuffer() + 12);
    const SysExVector expected = {0xF0, 0x51, 0x52, 0x53, 0x54, 0x55,
                                  0x56, 0x57, 0x58, 0x59, 0x5A, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(USBMIDIParser, Realtime) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x3F, 0xF8, 0x00, 0x00};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage expected = {MIDIMessageType::TIMING_CLOCK, 3};
    EXPECT_EQ(uparser.getRealTimeMessage(), expected);
}

TEST(USBMIDIParser, sysExContinueWithoutStarting) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x07, 0x33, 0x34, 0xF7};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::NO_MESSAGE);
}

TEST(USBMIDIParser, sysExEndsWithoutStarting1Byte) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x05, 0xF7, 0x00, 0x00};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::NO_MESSAGE);
}

TEST(USBMIDIParser, sysExEndsWithoutStarting2Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x06, 0x10, 0xF7, 0x00};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::NO_MESSAGE);
}

TEST(USBMIDIParser, sysExEndsWithoutStarting3Bytes) {
    USBMIDI_Parser uparser;
    uint8_t packet[4] = {0x07, 0x10, 0x11, 0xF7};
    EXPECT_EQ(uparser.parse(packet), MIDIReadEvent::NO_MESSAGE);
}

// -------------------------- SERIAL PARSER TESTS --------------------------- //

TEST(SerialMIDIParser, noteOff) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0x82), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x20), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x7F), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x82);
    EXPECT_EQ(msg.data1, 0x20);
    EXPECT_EQ(msg.data2, 0x7F);
}

TEST(SerialMIDIParser, noteOn) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0x93), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x2A), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x7E), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x93);
    EXPECT_EQ(msg.data1, 0x2A);
    EXPECT_EQ(msg.data2, 0x7E);
}

TEST(SerialMIDIParser, noteOnRunningStatus) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0x9A), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x10), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x11), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0x9A);
    EXPECT_EQ(msg1.data1, 0x10);
    EXPECT_EQ(msg1.data2, 0x11);

    EXPECT_EQ(sparser.parse(0x12), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x13), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg2 = sparser.getChannelMessage();
    EXPECT_EQ(msg2.header, 0x9A);
    EXPECT_EQ(msg2.data1, 0x12);
    EXPECT_EQ(msg2.data2, 0x13);
}

TEST(SerialMIDIParser, afterTouch) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xA1), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x01), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x02), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xA1);
    EXPECT_EQ(msg.data1, 0x01);
    EXPECT_EQ(msg.data2, 0x02);
}

TEST(SerialMIDIParser, controlChange) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xBB), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x03), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x04), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xBB);
    EXPECT_EQ(msg.data1, 0x03);
    EXPECT_EQ(msg.data2, 0x04);
}

TEST(SerialMIDIParser, programChange) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xC6), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x7A), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xC6);
    EXPECT_EQ(msg.data1, 0x7A);
}

TEST(SerialMIDIParser, programChangeRunningStatus) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xC6), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x7B), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg1 = sparser.getChannelMessage();
    EXPECT_EQ(msg1.header, 0xC6);
    EXPECT_EQ(msg1.data1, 0x7B);

    EXPECT_EQ(sparser.parse(0x7C), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg2 = sparser.getChannelMessage();
    EXPECT_EQ(msg2.header, 0xC6);
    EXPECT_EQ(msg2.data1, 0x7C);
}

TEST(SerialMIDIParser, channelPressure) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xD7), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x16), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xD7);
    EXPECT_EQ(msg.data1, 0x16);
}

TEST(SerialMIDIParser, pitchBend) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xE0), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x55), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x66), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0xE0);
    EXPECT_EQ(msg.data1, 0x55);
    EXPECT_EQ(msg.data2, 0x66);
}

TEST(SerialMIDIParser, sysEx2Bytes) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExLength(), 2);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 2);
    const SysExVector expected = {0xF0, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(SerialMIDIParser, sysEx3Bytes) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x10), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExLength(), 3);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 3);
    const SysExVector expected = {0xF0, 0x10, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(SerialMIDIParser, sysEx7Bytes) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x41), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x42), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x43), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x44), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x45), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExLength(), 7);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 7);
    const SysExVector expected = {0xF0, 0x41, 0x42, 0x43, 0x44, 0x45, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(SerialMIDIParser, sysEx2BytesBeforeOther) {
    SerialMIDI_Parser sparser;
    {
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 2);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 2);
        const SysExVector expected = {0xF0, 0xF7};
        EXPECT_EQ(result, expected);
    }
    {
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x12), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 3);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 3);
        const SysExVector expected = {0xF0, 0x12, 0xF7};
        EXPECT_EQ(result, expected);
    }
}

TEST(SerialMIDIParser, sysExDataAfterEnd) {
    SerialMIDI_Parser sparser;
    {
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 2);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 2);
        const SysExVector expected = {0xF0, 0xF7};
        EXPECT_EQ(result, expected);
    }
    {
        EXPECT_EQ(sparser.parse(0x12), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x13), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x14), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x15), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 4);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 4);
        const SysExVector expected = {0xF0, 0x14, 0x15, 0xF7};
        EXPECT_EQ(result, expected);
    }
    {
        EXPECT_EQ(sparser.parse(0x16), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x17), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x18), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x19), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 4);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 4);
        const SysExVector expected = {0xF0, 0x18, 0x19, 0xF7};
        EXPECT_EQ(result, expected);
    }
}

TEST(SerialMIDIParser, RealTime) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xF8), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage expected = {MIDIMessageType::TIMING_CLOCK, 0};
    EXPECT_EQ(sparser.getRealTimeMessage(), expected);
}

TEST(SerialMIDIParser, noteOffInterruptedByRealTime) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0x82), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF8), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage rt1 = {MIDIMessageType::TIMING_CLOCK, 0};
    EXPECT_EQ(sparser.getRealTimeMessage(), rt1);
    EXPECT_EQ(sparser.parse(0x20), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF9), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage rt2 = {MIDIMessageType::UNDEFINED_REALTIME_1, 0};
    EXPECT_EQ(sparser.getRealTimeMessage(), rt2);
    EXPECT_EQ(sparser.parse(0x7F), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x82);
    EXPECT_EQ(msg.data1, 0x20);
    EXPECT_EQ(msg.data2, 0x7F);
}

TEST(SerialMIDIParser, sysExInterruptedByRealTime) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x01), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x02), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF8), MIDIReadEvent::REALTIME_MESSAGE);
    RealTimeMessage rt1 = {MIDIMessageType::TIMING_CLOCK, 0};
    EXPECT_EQ(sparser.getRealTimeMessage(), rt1);
    EXPECT_EQ(sparser.parse(0x03), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExLength(), 5);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 5);
    const SysExVector expected = {0xF0, 0x01, 0x02, 0x03, 0xF7};
    EXPECT_EQ(result, expected);
}

TEST(SerialMIDIParser, sysExEndsByChannelMessage) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x01), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x02), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x03), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x8F), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExLength(), 5);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 5);
    const SysExVector expected = {0xF0, 0x01, 0x02, 0x03, 0xF7};
    EXPECT_EQ(result, expected);
    EXPECT_EQ(sparser.parse(0x10), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x12), MIDIReadEvent::CHANNEL_MESSAGE);
    ChannelMessage msg = sparser.getChannelMessage();
    EXPECT_EQ(msg.header, 0x8F);
    EXPECT_EQ(msg.data1, 0x10);
    EXPECT_EQ(msg.data2, 0x12);
}

TEST(SerialMIDIParser, sysExEndsWithoutStarting1Byte) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsWithoutStarting2Bytes) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0x51), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsWithoutStarting3Bytes) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0x52), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x53), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExEndsMultipleTimesWithoutStarting) {
    SerialMIDI_Parser sparser;
    EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0x10), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.parse(0x20), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::NO_MESSAGE);
}

TEST(SerialMIDIParser, sysExStartsTwice) {
    SerialMIDI_Parser sparser;
    {
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 2);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 2);
        const SysExVector expected = {0xF0, 0xF7};
        EXPECT_EQ(result, expected);
    }
    {
        EXPECT_EQ(sparser.parse(0x20), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x30), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 4);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 4);
        const SysExVector expected = {0xF0, 0x20, 0x30, 0xF7};
        EXPECT_EQ(result, expected);
    }
}

TEST(SerialMIDIParser, sysExEndsWithSysExStart) {
    SerialMIDI_Parser sparser;
    {
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x10), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 3);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 3);
        const SysExVector expected = {0xF0, 0x10, 0xF7};
        EXPECT_EQ(result, expected);
    }
    {
        EXPECT_EQ(sparser.parse(0x20), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x30), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 4);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 4);
        const SysExVector expected = {0xF0, 0x20, 0x30, 0xF7};
        EXPECT_EQ(result, expected);
    }
}

TEST(SerialMIDIParser, sysExRecoverAfterBufferOverflow) {
    SerialMIDI_Parser sparser;
    {
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        SysExVector expected = {0xF0};
        expected.resize(SYSEX_BUFFER_SIZE);
        for (size_t i = 1; i < SYSEX_BUFFER_SIZE; ++i) {
            EXPECT_EQ(sparser.parse(i / 2), MIDIReadEvent::NO_MESSAGE);
            expected[i] = i / 2;
        }
        EXPECT_EQ(sparser.parse(0x7F), MIDIReadEvent::NO_MESSAGE); // Overflow
        EXPECT_EQ(sparser.parse(0x7F), MIDIReadEvent::NO_MESSAGE); // Overflow
        EXPECT_EQ(sparser.parse(0x7F), MIDIReadEvent::NO_MESSAGE); // Overflow
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), SYSEX_BUFFER_SIZE);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + SYSEX_BUFFER_SIZE);
        EXPECT_EQ(result, expected);
    }
    {
        EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x20), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0x30), MIDIReadEvent::NO_MESSAGE);
        EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
        EXPECT_EQ(sparser.getSysExLength(), 4);
        const SysExVector result(sparser.getSysExBuffer(),
                                 sparser.getSysExBuffer() + 4);
        const SysExVector expected = {0xF0, 0x20, 0x30, 0xF7};
        EXPECT_EQ(result, expected);
    }
}

TEST(SerialMIDIParser, realSysEx) {
    SerialMIDI_Parser sparser;
    SysExVector data = {
        0x0,  0x0,  0x66, 0x14, 0x12, 0x0,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x4d, 0x61, 0x63,
        0x6b, 0x69, 0x65, 0x20, 0x43, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x20,
        0x55, 0x6e, 0x69, 0x76, 0x65, 0x72, 0x73, 0x61, 0x6c, 0x20, 0x2d, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    };
    EXPECT_EQ(sparser.parse(0xF0), MIDIReadEvent::NO_MESSAGE);
    for (uint8_t d : data)
        EXPECT_EQ(sparser.parse(d), MIDIReadEvent::NO_MESSAGE);
    EXPECT_EQ(sparser.parse(0xF7), MIDIReadEvent::SYSEX_MESSAGE);
    EXPECT_EQ(sparser.getSysExLength(), 120);
    const SysExVector result(sparser.getSysExBuffer(),
                             sparser.getSysExBuffer() + 120);
    const SysExVector expected = {
        0xF0, 0x0,  0x0,  0x66, 0x14, 0x12, 0x0,  0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x20, 0x4d, 0x61,
        0x63, 0x6b, 0x69, 0x65, 0x20, 0x43, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c,
        0x20, 0x55, 0x6e, 0x69, 0x76, 0x65, 0x72, 0x73, 0x61, 0x6c, 0x20, 0x2d,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF7,
    };
    EXPECT_EQ(result, expected);
}