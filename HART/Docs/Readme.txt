Minden, 2012-06-21   Stefan Tabelander (IIMS)

Release 1.00.01

NOTE:
The follwoing instructions shall serve as a help when integration the HART 7 subsystem in a new framework environment.  
It is suggested to use framework version 2.3.0.  Earlier versions do not cause neccessarily problems but have not been tested o far.  The subsystem
is developed for use with an M16C (62,63,65) derivative but may be used on other controlers as well.  All testing so far took place on an M16C-63 with  
an IAR compiler verson 3.40.5.40066.

In order to visualize the interfaces the HART 7 subsystem assumes on a target device, a dedicated "Application" subsystem was introduced.
This subsystem accommodates those parameters and functions as dummy implementations that shall be replaced by the specific and corresponding
equivalent in the target device software. The "Application" (dummy) subsystem shall not be used on the final target device.

In order to help find those places that need to be modified, a special comment was introduced.  The developer simply needs to search for 
// @@adjust
The occurances are assumed to be self-explaining and contain hardware dependencies, places where "Application" functions or objects are used 
or places where the developer is supposed to remove or add specific code.

Please note the current implementation is based on a soft modem solution using an MSP430.  The soft modem accommodates the complete Layer 1.  
Other implementions may not have the soft modem.  E.g. the soft modem may reside in the main micro-controller or a special HART ASIC is used.  
In these cases the developer is supposed to modify the subsystem in such a way, that a dedicated/custom Layer 1 will be added.

Note, that the UART receive- and transmit interrupt hardware for HART communication must be initialized in such a way that the interrupt is initiated in case the 
--- transmit buffer is empty! ---.  This must be done in order to comply with the in HART specified timing (e.g. for burst, gap times, etc.)  
If the UART is not initialized this way the timers will not be started at the correct locations in the code as the interrupt is generated at a different time.

The HART subsystem provides means to add a second Layer 2.  A template for the interfaces is provided and the interaction with the Layer 7 is available.
Still the implementation of the second Layer 2 lies in the responsibility of the device developer.  This is for the reason, that the Second Layer 2 is
not specified and different devices have different/diverting requirements for it.  In principle the developer may reuse the Layer2 and modify (reduce) it
to his/her needs.