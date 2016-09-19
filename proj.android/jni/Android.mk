LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := dynacpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Boss/BossBase.cpp \
				   ../../Classes/Boss/Cyclop.cpp \
				   ../../Classes/Boss/Electro.cpp \
				   ../../Classes/Boss/Human.cpp \
				   ../../Classes/Boss/Iron.cpp \
				   ../../Classes/Boss/Snake.cpp \
				   ../../Classes/Model/Bomb.cpp \
				   ../../Classes/Model/Brick.cpp \
				   ../../Classes/Model/BrickBonus.cpp \
				   ../../Classes/Model/BrickDoor.cpp \
				   ../../Classes/Model/GameSettings.cpp \
				   ../../Classes/Model/GameSounds.cpp \
				   ../../Classes/Model/MapData.cpp \
				   ../../Classes/Model/NPC.cpp \
				   ../../Classes/Model/NPCData.cpp \
				   ../../Classes/Model/Player.cpp \
				   ../../Classes/Model/Timer.cpp \
				   ../../Classes/Model/WorldObject.cpp \
				   ../../Classes/Model/ControlButton.cpp \
				   ../../Classes/Model/ControlKeyBoard.cpp \
				   ../../Classes/Model/ControlJoystick.cpp \
				   ../../Classes/Scene/AbstractWorldScene.cpp \
				   ../../Classes/Scene/BattleScene.cpp \
				   ../../Classes/Scene/FinalScene.cpp \
				   ../../Classes/Scene/GameOverScene.cpp \
				   ../../Classes/Scene/LoadLevelScene.cpp \
				   ../../Classes/Scene/MenuScene.cpp \
				   ../../Classes/Scene/PasswordScene.cpp \
				   ../../Classes/Scene/PreloadBattleScene.cpp \
				   ../../Classes/Scene/StartingScene.cpp \
				   ../../Classes/Scene/WorldScene.cpp \
				   ../../Classes/Scene/SetupScene.cpp \
				   ../../Classes/utils/CSVReader.cpp \
				   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
