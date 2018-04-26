#include "inc/fmod.hpp"
#include <jni.h>
#include <stdlib.h>
#include <unistd.h>
#include <android/log.h>


#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"zph",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"zph",FORMAT,##__VA_ARGS__);


#define MODE_NORMAL 0
#define MODE_LUOLI 1
#define MODE_DASHU 2
#define MODE_JINGSONG 3
#define MODE_GAOGUAI 4
#define MODE_KONGLING 5


using namespace FMOD;

extern "C"
JNIEXPORT void JNICALL
Java_zzw_com_fmoddemo_VoiceHelper_fix(JNIEnv *env, jclass type_, jstring path_, jint type) {


    //声音引擎
    System *system;
    //声音
    Sound *sound = NULL;
    //数字处理(音效)
    DSP *dsp;
    //是否正在播放
    bool playing = true;
    //音乐轨道
    Channel *channel = NULL;
    //播放速度
    float frequency = 0;
    //音频地址
    const char *path = env->GetStringUTFChars(path_, 0);

    System_Create(&system);
    system->init(32, FMOD_INIT_NORMAL, NULL);
    try {
        //创建声音
        system->createSound(path, FMOD_DEFAULT, NULL, &sound);
        switch (type) {
            case MODE_NORMAL:
                //播放原生
                system->playSound(sound, 0, false, &channel);
                break;

            case MODE_LUOLI:
                //提升或者降低音调的一种音效
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
                //设置音调的参数
                dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 1.8);
                //添加进到channel，添加进轨道
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0, dsp);

                break;

            case MODE_DASHU:
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
                dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.8);
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0, dsp);
                break;
            case MODE_JINGSONG:
                system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
                dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8);
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0, dsp);
                break;
            case MODE_GAOGUAI:
                //提高说话的速度
                system->playSound(sound, 0, false, &channel);
                channel->getFrequency(&frequency);
                frequency = frequency * 2;
                channel->setFrequency(frequency);
                break;
            case MODE_KONGLING:
                system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
                dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
                dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20);
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0, dsp);
                break;
        }

    } catch (...) {
        LOGE("%s", "发生异常");
        goto end;
    }
    system->update();

    //单位是微妙
    //每秒钟判断下是否是播放
    while (playing) {
        channel->isPlaying(&playing);
        usleep(1000);
    }
    goto end;

    //释放资源
    end:
    env->ReleaseStringUTFChars(path_, path);
    sound->release();
    system->close();
    system->release();


}
