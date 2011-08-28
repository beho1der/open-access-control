#ifndef PINCATCHER_IMPL_H
#define PINCATCHER_IMPL_H

#include <stdint.h>

// forward decleration
class PinCatcher;

/// The PinCatcher Implementattion class
///
/// @verbatim
///
/// Pins on the official Arduino ATmega168/328 platforms are set up into 3 ports/banks.  Basically as follows:
///   0-7  bank 0 (Wich is actually port D)
///   8-13 bank 1 (port B)
///  14-19 bank 2 (port C)
///
/// Official Arduino (v0022, anyway) doesn't support pin change interupts.  At least not directly.
///
/// ATMega1280 and ATMega2560, and thus Arduino Megas, are not nearly as clearly set up.  Essentially, the equivalent 20 pins on
/// the Arduino Megas actually span 5 ports (E, G, H, B, and F).
///
/// This work was produced after reviewing the code in PinChangeInt and PCint.  PinChangeInt was created whith the goal of
/// generating efficient code at the cost of readabiliity.  I feel the sacrifice was NOT worth it.
///
/// After extended review, I don't think either of the previously mentioned code works on Megas.  Both make assumptions about the
/// return value of portInputRegister().  The assumptions destroy any amount of portability the code might have had.  In terms of
/// efficiency, far better to test chipset and hard code the known values.
/// 
/// After considering both libraries, and my end goal (having the Pin based ISR call C++ member methods), I felt that a simpler
/// library than either of the aforementioned could be both faster AND easier to read.
///
/// PinCatcher.
/// 
/// @endverbatim
///
/// @TODO fix this so a fault is generated if an attachement is attempted when a pin slot already has a listener
///
class PinCatcher_impl
{
public:
   /// Constructor
   PinCatcher_impl();
   /// Destructor
   ~PinCatcher_impl();

   /// attach a PinCatcher class
   void attachPin(unsigned pin, PinCatcher* who);
   // detach a PinCatcher class
   void detachPin(unsigned pin);

   
   /// HandlePins0 is called by the ISR function to test for state changes in bank 0.
   void handlePins0();
   /// HandlePins1 is called by the ISR function to test for state changes in bank 1.
   void handlePins1();
   /// HandlePins2 is called by the ISR function to test for state changes in bank 2.
   void handlePins2();
   

   
protected:
   PinCatcher* who_[20];         ///< listner class pointers

   
   volatile uint8_t& pins0_;    ///< Bank 0 pin state register
   volatile uint8_t& pins1_;    ///< Bank 1 pin state register
   volatile uint8_t& pins2_;    ///< Bank 2 pin state register

   uint8_t last0_;              ///< cached pin states for Bank 0
   uint8_t last1_;              ///< cached pin states for Bank 1
   uint8_t last2_;              ///< cached pin states for Bank 2

   uint8_t mask0_;              ///< pins of interest for Bank 0, this mask is copied into the 
   uint8_t mask1_;              ///< pins of interest for Bank 1
   uint8_t mask2_;              ///< pins of interest for Bank 2 
};

   
#endif
