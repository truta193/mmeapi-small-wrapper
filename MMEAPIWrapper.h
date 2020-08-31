/* date = August 24th 2020 2:18 am */

#ifndef _M_M_E_A_P_I_WRAPPER_H
#define _M_M_E_A_P_I_WRAPPER_H

#include <stdio.h>
#include <windows.h>

typedef enum {false, true} bool;
typedef unsigned int uint;
typedef long unsigned int luint;

uint GetNumberOfDevices() {
    return (uint)mixerGetNumDevs();
};

HMIXER OpenMixer(uint id) {
    HMIXER handle;
    if (mixerOpen(&handle, id, 0, 0, 0) != MMSYSERR_NOERROR) printf("Error at OpenMixer, mixerOpen\n");
    return handle;
};

MIXERCAPS GetDeviceDetails(HMIXER handle) {
    MIXERCAPS mixercaps;
    if (mixerGetDevCaps(handle, &mixercaps, sizeof(MIXERCAPS)) != MMSYSERR_NOERROR)   printf("Error at GetDeviceDetails\n");
    return mixercaps;
};

MIXERLINE* GetDeviceLines(HMIXER handle){
    MIXERCAPS mixercaps;
    mixerGetDevCaps(handle, &mixercaps, sizeof(MIXERCAPS));
    
    MIXERLINE *lines = malloc(sizeof(MIXERLINE)*mixercaps.cDestinations);
    for (int i = 0; i < mixercaps.cDestinations; i++) {
        lines[i].cbStruct = sizeof(MIXERLINE);
        lines[i].dwDestination = i;
        if (mixerGetLineInfo((HMIXEROBJ)handle, &lines[i], MIXER_GETLINEINFOF_DESTINATION) != MMSYSERR_NOERROR) printf("Error at GetDeviceLines, mixerGetLineInfo\n");
        
    };
    return lines;
};

MIXERCONTROL* GetLineControls(HMIXER handle, MIXERLINE line) {
    MIXERCONTROL *controls = malloc(sizeof(MIXERCONTROL)*line.cControls);
    MIXERLINECONTROLS mixerlineControls;
    mixerlineControls.cbStruct = sizeof(MIXERLINECONTROLS);
    mixerlineControls.cControls = line.cControls;
    mixerlineControls.dwLineID = line.dwLineID;
    mixerlineControls.pamxctrl = controls;
    mixerlineControls.cbmxctrl = sizeof(MIXERCONTROL);
    if (mixerGetLineControls((HMIXEROBJ)handle, &mixerlineControls, MIXER_GETLINECONTROLSF_ALL) != MMSYSERR_NOERROR) printf("Error at GetLineControls, mixerGetLineControls\n");
    return controls;
    
};

luint* GetControlValues(HMIXER handle, MIXERLINE line, MIXERCONTROL control){
    MIXERCONTROLDETAILS mixerControlDetails;
    MIXERCONTROLDETAILS_UNSIGNED values[line.cChannels];
    mixerControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mixerControlDetails.dwControlID = control.dwControlID;
    mixerControlDetails.cChannels = line.cChannels;
    mixerControlDetails.hwndOwner = NULL;
    mixerControlDetails.cMultipleItems = 0;
    mixerControlDetails.paDetails = values;
    mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
    if (mixerGetControlDetails((HMIXEROBJ)handle, &mixerControlDetails, MIXER_GETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR) printf("Error at GetControlValues, mixerGetControlDetails\n");
    luint *returnValues = malloc(sizeof(luint)*line.cChannels);
    for (int i = 0; i < line.cChannels; i++) {
        returnValues[i] = values[i].dwValue;
    };
    return returnValues;
};

void SetControlValues(HMIXER handle, MIXERLINE line, MIXERCONTROL control, luint *values) {
    MIXERCONTROLDETAILS mixerControlDetails;
    MIXERCONTROLDETAILS_UNSIGNED value[line.cChannels];
    mixerControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mixerControlDetails.dwControlID = control.dwControlID;
    mixerControlDetails.cChannels = line.cChannels;
    mixerControlDetails.cMultipleItems = 0;
    mixerControlDetails.paDetails = value;
    mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
    for (int i = 0; i < line.cChannels; i++) {
        value[i].dwValue = values[i];
    };
    if (mixerSetControlDetails((HMIXEROBJ)handle, &mixerControlDetails, MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR) printf("Error at SetControlValues, mixerSetControlDetails");
};

uint GetControlValueMinimum(MIXERCONTROL control){
    return control.Bounds.dwMinimum;
};

uint GetControlValueMaximum(MIXERCONTROL control) {
    return control.Bounds.dwMaximum;
};

uint AuxGetNumberOfDevices() {
    return auxGetNumDevs();
};

void AuxGetVolume(uint id, uint *result) {
    auxGetVolume(id, result);
};

void AuxSetVolume(uint id, uint value) {
    auxSetVolume(id, value);
};

void AuxGetDeviceCaps(uint id, AUXCAPS *auxCaps) {
    auxGetDevCaps(id, auxCaps, sizeof(AUXCAPS));
};



#endif //_M_M_E_A_P_I_WRAPPER_H
