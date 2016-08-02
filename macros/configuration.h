#ifndef SETUP_CFG
#define SETUP_CFG

// Channels as given be physical configuration of delay lines plugged into NIM-ECL-NIM unit
// Positron detector
#define CFG_CHANNEL_POS_CP2 16//24//16
#define CFG_CHANNEL_POS_U1_S 17//25//17
#define CFG_CHANNEL_POS_U2_S 18//26//18
#define CFG_CHANNEL_POS_V1_S 19//27//19
#define CFG_CHANNEL_POS_V2_S 20//28//20
#define CFG_CHANNEL_POS_W1_S 21//29//21
#define CFG_CHANNEL_POS_W2_S 22//30//22
// Electron detector
#define CFG_CHANNEL_ELEC_CP2 24//16//24
#define CFG_CHANNEL_ELEC_U1_S 25//21//27
#define CFG_CHANNEL_ELEC_U2_S 26//18//28
#define CFG_CHANNEL_ELEC_V1_S 27//19//25
#define CFG_CHANNEL_ELEC_V2_S 28//20//26
#define CFG_CHANNEL_ELEC_W1_S 29//17//29
#define CFG_CHANNEL_ELEC_W2_S 30//22//30

// External delays given by cabling from the detector potential divider to the NIM-ECL-NIM unit in ns NOT bins
// Positron detector
#define CFG_DELAY_POS_CP2 10
#define CFG_DELAY_POS_U1_S 10
#define CFG_DELAY_POS_U2_S 10
#define CFG_DELAY_POS_V1_S 10
#define CFG_DELAY_POS_V2_S 10
#define CFG_DELAY_POS_W1_S 10
#define CFG_DELAY_POS_W2_S 10
// 10 detector
/* 2015 settings
#define CFG_DELAY_ELEC_CP2 41.2355
#define CFG_DELAY_ELEC_U1_S 47.1645
#define CFG_DELAY_ELEC_U2_S 43.5455
#define CFG_DELAY_ELEC_V1_S 67.694
#define CFG_DELAY_ELEC_V2_S 66.969
#define CFG_DELAY_ELEC_W1_S 63.0815
#define CFG_DELAY_ELEC_W2_S 63.993
*/

#define CFG_DELAY_ELEC_CP2 56.2
#define CFG_DELAY_ELEC_U1_S 57.6
#define CFG_DELAY_ELEC_U2_S 57.6
#define CFG_DELAY_ELEC_V1_S 57.6
#define CFG_DELAY_ELEC_V2_S 57.6
#define CFG_DELAY_ELEC_W1_S 57.6
#define CFG_DELAY_ELEC_W2_S 57.6
//setting max and min positron times


//setting max and min ion times


#endif