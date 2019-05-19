LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_IMG_PATH := ../SDL_image
SDL_TTF_PATH := ../SDL_ttf

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/$(SDL_IMG_PATH) $(LOCAL_PATH)/$(SDL_TTF_PATH)

# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(*.c))

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
