/* MD5: 05408bd2528d37e1f1795c3c5d3d77be*/
#if !defined(__PRODUCT_CONFIG_AUDIO_H__)
#define __PRODUCT_CONFIG_AUDIO_H__

#ifndef TENSILICA_BUILDS_HIFI
#define TENSILICA_BUILDS_HIFI $(ROOT_XTENSA_PATH_W)/XtDevTools/install/builds/RE-2014.5-linux 
#endif 

#ifndef TENSILICA_TOOLS_HIFI
#define TENSILICA_TOOLS_HIFI $(ROOT_XTENSA_PATH_W)/XtDevTools/install/tools/RE-2014.5-linux 
#endif 

#ifndef FEATURE_HIFI
#define FEATURE_HIFI FEATURE_OFF 
#endif 

#ifndef FEATURE_HIFI_USE_ICC
#define FEATURE_HIFI_USE_ICC FEATURE_OFF 
#endif 

#ifndef FEATURE_MBB_MODULE_AUDIO
#define FEATURE_MBB_MODULE_AUDIO FEATURE_OFF 
#endif 

#ifndef FEATURE_VOICE_UP
#define FEATURE_VOICE_UP FEATURE_OFF 
#endif 

#ifdef OBC_IMAGE_MODEM
#endif 

#ifdef OBC_IMAGE_APP
#endif 

#ifdef OBC_IMAGE_BOOTROM
#endif 

#ifdef OBC_IMAGE_BOOTLOADER
#endif 

#endif /*__PRODUCT_CONFIG_H__*/ 
