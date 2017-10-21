#include "com_ring0_opensl_OpenSLHelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <android/log.h>

static       SLObjectItf                   g_sl_object          = 0;
static       SLEngineItf                   g_sl_engine          = 0;
static       SLObjectItf                   g_sl_mixobject       = 0;
static       SLEnvironmentalReverbItf      g_sl_reverb          = 0;
static const SLEnvironmentalReverbSettings g_sl_reverb_settings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

static       SLObjectItf                   l_sl_player          = 0;
static       SLPlayItf                     l_sl_play            = 0;
static       SLSeekItf                     l_sl_seek            = 0;
static       SLMuteSoloItf                 l_sl_solo            = 0;
static       SLVolumeItf                   l_sl_volume          = 0;
static       SLAndroidSimpleBufferQueueItf l_sl_queue           = 0;
static       SLEffectSendItf               l_sl_effect          = 0;

void android_opensles_create_engine();
void android_opensles_stop();
void android_opensles_play_buffer(void*, int, int, int);
void android_opensles_callback(SLAndroidSimpleBufferQueueItf, void*);

struct Data {
    void *buffer;
    int   size;
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    android_opensles_create_engine();
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    android_opensles_stop();
}

JNIEXPORT void JNICALL Java_com_ring0_opensl_OpenSLHelper_opensl_1play
  (JNIEnv *env, jclass, jbyteArray jbuffer, jint size, jint channel, jint sample, jint pcm_type) {
    void *buffer = env->GetByteArrayElements(jbuffer, 0);
    android_opensles_play_buffer(buffer, size, channel, sample);
    env->ReleaseByteArrayElements(jbuffer, (jbyte*)buffer, 0);
}

void android_opensles_create_engine() {
    // 创建播放引擎
    slCreateEngine(&g_sl_object, 0, 0, 0, 0, 0);
    // 非同步
    (*g_sl_object)->Realize(g_sl_object, SL_BOOLEAN_FALSE);
    // 获取 opensles SLEngineITF 的对象
    (*g_sl_object)->GetInterface(g_sl_object, SL_IID_ENGINE, &g_sl_engine);

    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean     req[1] = {SL_BOOLEAN_FALSE};
    // 获取混音器
    (*g_sl_engine)->CreateOutputMix(g_sl_engine, &g_sl_mixobject, 1, ids, req);
    // 初始化混音器
    (*g_sl_mixobject)->Realize(g_sl_mixobject, SL_BOOLEAN_FALSE);
    SLresult result = (*g_sl_mixobject)->GetInterface(g_sl_mixobject, SL_IID_ENVIRONMENTALREVERB, &g_sl_reverb);
    if (SL_RESULT_SUCCESS == result) {
        (*g_sl_reverb)->SetEnvironmentalReverbProperties(g_sl_reverb, &g_sl_reverb_settings);
    }
}

void android_opensles_stop() {
    if (l_sl_play) {
        (*l_sl_play)->SetPlayState(l_sl_play, SL_PLAYSTATE_STOPPED);
        (*l_sl_player)->Destroy(l_sl_player);
        l_sl_play   = 0;
        l_sl_player = 0;
        l_sl_seek   = 0;
        l_sl_solo   = 0;
        l_sl_volume = 0;
    }
}

void android_opensles_play_buffer(void* buffer, int size, int channel, int sample) {
    Data *pData = (Data*)malloc(sizeof(struct Data));
    pData->buffer = buffer;
    pData->size   = size;

    SLDataLocator_AndroidSimpleBufferQueue sl_queue = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM sl_pcm = {
            SL_DATAFORMAT_PCM,
            channel,                      // numChannels
            sample * 1000,                // samplesPerSec
            SL_PCMSAMPLEFORMAT_FIXED_8,   // bitsPerSample
            SL_PCMSAMPLEFORMAT_FIXED_8,   // containerSize
            SL_SPEAKER_FRONT_CENTER,      // channelMask
            SL_BYTEORDER_LITTLEENDIAN     // endianness
    };
    SLDataSource            sl_source = {&sl_queue, &sl_pcm};
    SLDataLocator_OutputMix sl_mix    = {SL_DATALOCATOR_OUTPUTMIX, g_sl_mixobject};
    SLDataSink              sl_sink   = {&sl_mix, 0};

    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean     req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    (*g_sl_engine)->CreateAudioPlayer(g_sl_engine, &l_sl_player,  &sl_source, &sl_sink, 3, ids, req);
    (*l_sl_player)->Realize(        l_sl_player, SL_BOOLEAN_FALSE);
    (*l_sl_player)->GetInterface(   l_sl_player, SL_IID_PLAY,                 &l_sl_play);
    (*l_sl_player)->GetInterface(   l_sl_player, SL_IID_BUFFERQUEUE,          &l_sl_queue);
    (*l_sl_player)->GetInterface(   l_sl_player, SL_IID_EFFECTSEND,           &l_sl_effect);
    (*l_sl_player)->GetInterface(   l_sl_player, SL_IID_VOLUME,               &l_sl_volume);

    (*l_sl_queue)->RegisterCallback(l_sl_queue,  android_opensles_callback,   pData);
    (*l_sl_play)->SetPlayState(     l_sl_play,   SL_PLAYSTATE_PLAYING);
    android_opensles_callback(l_sl_queue, pData);
}

void android_opensles_callback(SLAndroidSimpleBufferQueueItf queue, void *data) {
    Data *pData = (Data*)data;
    void *buffer_byte = pData->buffer;
    int   buffer_size = pData->size;
    if (buffer_byte && buffer_size) {
        (*queue)->Enqueue(queue, buffer_byte, buffer_size);
    }
}
