#include <byMe/MMEAPIWrapper.h>

int main(){
    uint nod = GetNumberOfDevices();
    HMIXER mic = OpenMixer(nod-1);
    MIXERCAPS micCaps = GetDeviceDetails(mic);
    MIXERLINE *micLines = GetDeviceLines(mic);
    //printf("%d", micCaps.cDestinations);
    MIXERCONTROL *micControls = GetLineControls(mic, micLines[0]); //0 Mute 1 Volume
    luint *value = GetControlValues(mic, micLines[0], micControls[1]);
    int temp = (int)value[0];
    
    while (true) {
        scanf("%lu", &temp);
        temp *= 655;
        if (temp >= 0) SetControlValues(mic, micLines[0], micControls[1], &value[0]); else break;
    };
    free(micLines);
    free(micControls);
    free(value);
    printf("Exiting");
    return 1;
};