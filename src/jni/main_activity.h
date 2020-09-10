#pragma once

#include <fake-jni/fake-jni.h>
#include "java_types.h"
#include "../text_input_handler.h"

class BuildVersion : public FakeJni::JObject {

public:
    DEFINE_CLASS_NAME("android/os/Build$VERSION")

    static FakeJni::JInt SDK_INT;
    static std::shared_ptr<FakeJni::JString> RELEASE;

};

class PackageInfo : public FakeJni::JObject {

public:
    DEFINE_CLASS_NAME("android/content/pm/PackageInfo")

    PackageInfo() {
        versionName = std::make_shared<FakeJni::JString>("1.16.20.03");
    }
    std::shared_ptr<FakeJni::JString> versionName;

};

class PackageManager : public FakeJni::JObject {

public:
    DEFINE_CLASS_NAME("android/content/pm/PackageManager")

    std::shared_ptr<PackageInfo> getPackageInfo(std::shared_ptr<FakeJni::JString> packageName, FakeJni::JInt flags) {
        return std::make_shared<PackageInfo>();
    }

};

class Context : public FakeJni::JObject {

public:
    DEFINE_CLASS_NAME("android/content/Context")

    virtual std::shared_ptr<File> getFilesDir() = 0;

    virtual std::shared_ptr<File> getCacheDir() = 0;

    std::shared_ptr<ClassLoader> getClassLoader() {
        return ClassLoader::getInstance();
    }

    std::shared_ptr<Context> getApplicationContext() {
        return std::static_pointer_cast<Context>(shared_from_this());
    }

    std::shared_ptr<FakeJni::JString> getPackageName() {
        return std::make_shared<FakeJni::JString>("com.mojang.minecraftpe");
    }

    std::shared_ptr<PackageManager> getPackageManager() {
        return std::make_shared<PackageManager>();
    }

    std::shared_ptr<FakeJni::JString> getSecureStorageKey(std::shared_ptr<FakeJni::JString> str) {
        auto st = str->asStdString();
        return std::make_shared<FakeJni::JString>("1.1.0");
    }

};

class ContextWrapper : public Context {

public:
    DEFINE_CLASS_NAME("android/content/ContextWrapper", Context)

};

class Activity : public ContextWrapper {

public:
    DEFINE_CLASS_NAME("android/app/Activity", ContextWrapper)

};

class NativeActivity : public Activity {

public:
    DEFINE_CLASS_NAME("android/app/NativeActivity", Activity)

};

class HardwareInfo : public FakeJni::JObject {

public:
    DEFINE_CLASS_NAME("com/mojang/minecraftpe/HardwareInformation")

    static std::shared_ptr<FakeJni::JString> getAndroidVersion() {
        return std::make_shared<FakeJni::JString>("Linux");
    }

    std::shared_ptr<FakeJni::JString> getInstallerPackageName() {
        return std::make_shared<FakeJni::JString>("com.mojang.minecraftpe");
    }

};
#include <fstream>
class MainActivity : public NativeActivity {

private:
    bool ignoreNextHideKeyboard = false;

public:
    unsigned char* (*stbi_load_from_memory)(unsigned char const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels);
    void (*stbi_image_free)(void *retval_from_stbi_load);

    DEFINE_CLASS_NAME("com/mojang/minecraftpe/MainActivity", NativeActivity)

    std::string storageDirectory;
    TextInputHandler *textInput = nullptr;
    std::function<void ()> quitCallback;

    int getAndroidVersion() {
        return 27;
    }

    FakeJni::JBoolean isNetworkEnabled(FakeJni::JBoolean wifi) {
        return true;
    }

    std::shared_ptr<FakeJni::JString> getLocale() {
        return std::make_shared<FakeJni::JString>("en");
    }

    std::shared_ptr<FakeJni::JString> getDeviceModel() {
        return std::make_shared<FakeJni::JString>("Linux");
    }

    std::shared_ptr<File> getFilesDir() override {
        return std::make_shared<File>(storageDirectory);
    }

    std::shared_ptr<File> getCacheDir() override {
        return std::make_shared<File>(storageDirectory);
    }

    std::shared_ptr<FakeJni::JString> getExternalStoragePath() {
        return std::make_shared<FakeJni::JString>(storageDirectory);
    }

    FakeJni::JBoolean hasWriteExternalStoragePermission() {
        return true;
    }

    std::shared_ptr<HardwareInfo> getHardwareInfo() {
        return std::make_shared<HardwareInfo>();
    }

    std::shared_ptr<FakeJni::JString> createUUID();

    std::shared_ptr<FakeJni::JByteArray> getFileDataBytes(std::shared_ptr<FakeJni::JString> path) {
        std::ifstream file(path->asStdString(), std::ios::binary | std::ios::ate);
        if(file.is_open()) {
            auto length = file.tellg();
            auto value = new jbyte[(size_t)length + 1];
            file.seekg(0, std::ios::beg);
            file.read((char*)value, length);
            value[length] = 0;
            return std::make_shared<FakeJni::JByteArray>(value, (size_t)length);
        } else {
            if(!path->compare(0, 20, "file:/android_asset/")) {
                return getFileDataBytes(std::make_shared<FakeJni::JString>(path->data() + 20));
            }
            return std::make_shared<FakeJni::JByteArray>();
        }
    }

    std::shared_ptr<FakeJni::JArray<FakeJni::JString>> getIPAddresses() {
        return std::make_shared<FakeJni::JArray<FakeJni::JString>>();
    }

    std::shared_ptr<FakeJni::JArray<FakeJni::JString>> getBroadcastAddresses() {
        return std::make_shared<FakeJni::JArray<FakeJni::JString>>();
    }

    void showKeyboard(std::shared_ptr<FakeJni::JString> text, FakeJni::JInt maxLen, FakeJni::JBoolean ignored,
            FakeJni::JBoolean ignored2, FakeJni::JBoolean multiline) {
        ignoreNextHideKeyboard = false;
        if (textInput)
            textInput->enable(text->asStdString(), multiline);
    }

    void hideKeyboard() {
        if (ignoreNextHideKeyboard) {
            ignoreNextHideKeyboard = false;
            return;
        }
        if (textInput)
            textInput->disable();
    }

    void updateTextboxText(std::shared_ptr<FakeJni::JString> newText) {
        if (textInput)
            textInput->update(newText->asStdString());
        ignoreNextHideKeyboard = true;
    }

    FakeJni::JInt getCursorPosition() {
        ignoreNextHideKeyboard = false;
        if (textInput)
            return textInput->getCursorPosition();
        return 0;
    }

    FakeJni::JLong getUsedMemory();

    FakeJni::JLong getFreeMemory();

    FakeJni::JLong getTotalMemory();

    FakeJni::JLong getMemoryLimit();

    FakeJni::JLong getAvailableMemory();

    void pickImage(FakeJni::JLong callback);

    void initializeXboxLive(FakeJni::JLong xalinit, FakeJni::JLong xblinit);

    std::shared_ptr<FakeJni::JIntArray> getImageData(std::shared_ptr<FakeJni::JString> filename, jboolean b);
};

class JellyBeanDeviceManager : public FakeJni::JObject {

public:
    DEFINE_CLASS_NAME("com/mojang/minecraftpe/input/JellyBeanDeviceManager")

};