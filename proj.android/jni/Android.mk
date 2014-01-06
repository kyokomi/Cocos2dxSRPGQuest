LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/title/TitleScene.cpp \
                   ../../Classes/title/TitleSceneLoader.cpp \
                   ../../Classes/map/MapManager.cpp \
                   ../../Classes/world_map/MapScene.cpp \
                   ../../Classes/world_map/MapSceneLoader.cpp \
                   ../../Classes/actor/ActorSprite.cpp \
                   ../../Classes/game_object/DropItemSprite.cpp \
                   ../../Classes/ui/table/CustomTableViewCell.cpp \
                   ../../Classes/ui/table/TableViewTestLayer.cpp \
                   ../../Classes/quest/SRPGScene.cpp \
                   ../../Classes/quest/SRPGMapLayer.cpp \
                   ../../Classes/logic/BattleLogic.cpp \
                   ../../Classes/rogue/RogueScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                   $(LOCAL_PATH)/../../Classes/title \
                   $(LOCAL_PATH)/../../Classes/world_map \
                   $(LOCAL_PATH)/../../Classes/quest \
                   $(LOCAL_PATH)/../../Classes/actor \
                   $(LOCAL_PATH)/../../Classes/logic \
                   $(LOCAL_PATH)/../../Classes/game_object \
                   $(LOCAL_PATH)/../../Classes/ui/table \
                   $(LOCAL_PATH)/../../Classes/map \
                   $(LOCAL_PATH)/../../Classes/rogue 

LOCAL_WHOLE_STATIC_LIBRARIES := cocosbuilder_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,extensions)
$(call import-module,editor-support/cocosbuilder)
$(call import-module,2d)
$(call import-module,audio/android)
