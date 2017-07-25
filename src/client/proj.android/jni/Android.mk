LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos2dcpp_shared
LOCAL_MODULE_FILENAME := libcocos2dcpp
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   myUtilsConfig.cpp \
                   Java_tools_CommonFunc.cpp \
                   ../../AppDelegate.cpp \
                   ../../b2Sprite.cpp \
                   ../../backgroundLayer.cpp \
                   ../../bezierTest.cpp \
                   ../../box2dHandler.cpp \
                   ../../checkinLayer.cpp \
                   ../../CommonFunc.cpp \
                   ../../exchangeDict.cpp \
                   ../../fishDict.cpp \
                   ../../fishRoute.cpp \
                   ../../formationDict.cpp \
                   ../../gameScene.cpp \
                   ../../GLES-Render.cpp \
                   ../../levelDict.cpp \
                   ../../levelUpgradeLayer.cpp \
                   ../../menuLayer.cpp \
                   ../../menuScene.cpp \
                   ../../myGame.cpp \
                   ../../spriteLayer.cpp \
                   ../../storeScene.cpp \
                   ../../taskDict.cpp \
                   ../../taskUI.cpp \
                   ../../testinAdapter.cpp \
                   ../../tipsDict.cpp \
                   ../../touchLayer.cpp \
                   ../../rewardDict.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../.. $(LOCAL_PATH)/../../bin

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += testin_crash_helper_static 

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
$(call import-module,external/crashhelper)