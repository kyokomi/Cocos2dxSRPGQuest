LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/title/TitleScene.cpp \
                   ../../Classes/title/TitleSceneLoader.cpp \
                   ../../Classes/world_map/MapScene.cpp \
                   ../../Classes/world_map/MapSceneLoader.cpp \
                   ../../Classes/quest/SRPGScene.cpp \
                   ../../Classes/quest/SRPGMapLayer.cpp \
                   ../../Classes/quest/actor/ActorSprite.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                   ../../Classes/title \
                   ../../Classes/world_map \
                   ../../Classes/quest \
                   ../../Classes/quest/actor
                   
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dxandroid_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,cocos2dx/platform/android)
