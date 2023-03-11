// WriteAccessAllowed.h

TBOOL WriteAccessAllowed(TBOOL DataSpecific, TUSIGN8 ssIdx, TUSIGN8 objIdx, TUSIGN8 attribIdx);

// ----------------------------------------------------------------------------------------------
// TBOOL WriteAccessAllowed(TBOOL DataSpecific, TUSIGN8 ssIdx, TUSIGN8 objIdx, TUSIGN8 attribIdx)
//
//  If this function is called with "DataSpecific" == eTRUE ...
//  ...Then the HMI is asking if it can edit/write a specific data object (identified by ssIdx, objIdx & attribIdx)
//     return the write enable state specific to the data identified:- eFALSE if Write Access Not Allowed, eTRUE if Write Access Allowed
//
//  If this function is called with "DataSpecific" == eFALSE ...
//  ...Then the HMI is asking for the general write protection state for the device (values of ssIdx, objIdx & attribIdx are not relevant and can be ignored)
//     return the general write protection state for the device:- eFALSE if device is write protected, eTRUE if device not write protected
//
// This function is overrules the operation of the Access Rights Manager (if fitted)
// If this function returns eFALSE then the HMI will assume write protection and will not access the ARM
// if this function returns eTRUE then the HMI will access the ARM separately to check login levels etc
//
//+@ HMIGen WriteAccessAllowed @+
//
// ADD DEVICE SPECIFIC INCLUDED FILES HERE.........

//
TBOOL WriteAccessAllowed(TBOOL DataSpecific, TUSIGN8 ssIdx, TUSIGN8 objIdx, TUSIGN8 attribIdx)
{
    // Add Write access test here
 //   #error Write access test function not complete
    return eTRUE;
}
//-@ HMIGen WriteAccessAllowed @-
