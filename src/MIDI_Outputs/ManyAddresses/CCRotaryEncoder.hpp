#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIRotaryEncoder.hpp>
#include <MIDI_Senders/RelativeCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {
namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **quadrature
 *          (rotary) encoder** and send out relative MIDI **Control Change**
 *          events.
 * 
 * This version can be banked using an arbitrary list of alternative
 * addresses.
 *          
 * @tparam  NumBanks
 *          The number of variants/alternative addresses the element has.
 *
 * @ingroup ManyAddressesMIDIOutputElements
 */
template <setting_t NumBanks>
class CCRotaryEncoder
    : public MIDIRotaryEncoder<ManyAddresses<NumBanks>, RelativeCCSender> {
  public:
    /**
     * @brief   Construct a new Bankable CCRotaryEncoder object with the given 
     *          pins, controller, channel, speed factor, and number of pulses
     *          per step.
     * 
     * @param   bank
     *          The bank that selects the address to use.
     * @param   pins
     *          A list of the two pins connected to the A and B outputs of the
     *          encoder.  
     *          The internal pull-up resistors will be enabled by the Encoder
     *          library.
     * @param   addresses
     *          The list of MIDI addresses containing the controller number 
     *          [0, 119], channel [CHANNEL_1, CHANNEL_16], and optional cable 
     *          number [0, 15].
     * @param   speedMultiply
     *          A constant factor to increase the speed of the rotary encoder.
     *          The difference in position will just be multiplied by this 
     *          factor. 
     * @param   pulsesPerStep
     *          The number of pulses per physical click of the encoder.
     *          For a normal encoder, this is 4. If you want to increase the
     *          resolution, for the use of Jog wheels, for example, you can go
     *          as 1.  
     *          Whereas a greater speedMultiply factor will increase the 
     *          speed, increasing the number of pulsesPerStep will result in a 
     *          lower speed.
     */
    CCRotaryEncoder(const Bank<NumBanks> &bank, const EncoderPinList &pins,
                    const Array<MIDIAddress, NumBanks> &addresses,
                    int8_t speedMultiply = 1, uint8_t pulsesPerStep = 4)
        : MIDIRotaryEncoder<ManyAddresses<NumBanks>, RelativeCCSender>(
              {bank, addresses}, pins, speedMultiply, pulsesPerStep, {}) {}

// For tests only (PJRC Encoder library's copy constructor doesn't work)
#ifndef ARDUINO
    CCRotaryEncoder(const Bank<NumBanks> &bank, const Encoder &encoder,
                    const Array<MIDIAddress, NumBanks> &addresses,
                    int8_t speedMultiply = 1, uint8_t pulsesPerStep = 4)
        : MIDIRotaryEncoder<ManyAddresses<NumBanks>, RelativeCCSender>(
              {bank, addresses}, encoder, speedMultiply, pulsesPerStep, {}) {}
#endif
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE