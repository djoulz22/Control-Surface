#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIAbsoluteEncoder.hpp>
#include <MIDI_Senders/ContinuousCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **quadrature
 *          (rotary) encoder** and send out relative MIDI **Control Change**
 *          events.
 * 
 * This version can be banked.
 *
 * @note    To use this class, include the [PJRC Encoder library]
 *          (https://github.com/PaulStoffregen/Encoder) before the
 *          Control-Surface library.
 *
 * @ingroup BankableMIDIOutputElements
 */
template <size_t NumBanks>
class CCAbsoluteEncoder
    : public MIDIAbsoluteEncoder<NumBanks, SingleAddress, ContinuousCCSender> {
  public:
    /**
     * @brief   Construct a new Bankable CCAbsoluteEncoder object with the given 
     *          pins, controller, channel, speed factor, and number of pulses
     *          per step.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   pins
     *          A list of the two pins connected to the A and B outputs of the
     *          encoder.  
     *          The internal pull-up resistors will be enabled by the Encoder
     *          library.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [0, 15].
     * @param   multiplier
     *          A constant factor to increase the speed of the rotary encoder.
     *          The position will just be multiplied by this factor. 
     * @param   pulsesPerStep
     *          The number of pulses per physical click of the encoder.
     *          For a normal encoder, this is 4. If you want to increase the
     *          resolution, for the use of Jog wheels, for example, you can go
     *          as 1.  
     *          Whereas a greater speedMultiplier factor will increase the 
     *          speed, increasing the number of pulsesPerStep will result in a 
     *          lower speed.
     */
    CCAbsoluteEncoder(OutputBankConfig<NumBanks> config, const EncoderPinList &pins,
                      const MIDIAddress &address, int16_t multiplier = 1,
                      uint8_t pulsesPerStep = 4)
        : MIDIAbsoluteEncoder({config, address}, pins, multiplier,
                              pulsesPerStep, {}) {}
};

} // namespace Bankable

END_CS_NAMESPACE