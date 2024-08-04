// ======================================================================
// \title  Led.cpp
// \author tcanham
// \brief  cpp file for Led component implementation class
// ======================================================================

#include "Components/Led/Led.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Led ::
    Led(const char* const compName) :
      LedComponentBase(compName),
        state(Fw::On::OFF),
        transitions(0),
        count(0),
        blinking(false)
  {

  }

  Led ::
    ~Led()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void Led ::
    BLINKING_ON_OFF_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Fw::On on_off
    )
  {
    // Create a variable to represent the command response
    auto cmdResp = Fw::CmdResponse::OK;

    // Verify if on_off is a valid argument.
    // Note: isValid is an autogenerate helper function for enums defined in fpp.
    if(!on_off.isValid())
    {
        this->log_WARNING_LO_InvalidBlinkArgument(on_off);
        // Update command response with a validation error
        cmdResp = Fw::CmdResponse::VALIDATION_ERROR;
    }
    else
    {
      this->count = 0; // Reset count on any successful command
      this->lock.lock();
      this->blinking = Fw::On::ON == on_off; // Update blinking state
      this->lock.unlock();

      this->log_ACTIVITY_HI_SetBlinkingState(on_off);
      this->tlmWrite_BlinkingState(on_off);    }

      // Provide command response
      this->cmdResponse_out(opCode,cmdSeq,cmdResp);
  }
}
