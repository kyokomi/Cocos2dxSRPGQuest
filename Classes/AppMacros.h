//
//  AppMacros.h
//  nyandash
//
//  Created by kyokomi on 2013/09/27.
//
//

#ifndef nyandash_AppMacros_h
#define nyandash_AppMacros_h

#include "extensions/cocos-ext.h"

typedef struct tagResource
{
    cocos2d::Size size;
    char directory[100];
} Resource;

static Resource smallResource = { cocos2d::Size(512, 384), "resources-small" };
static Resource largeResource = { cocos2d::Size(1024, 768), "resources-large" };
static Resource xlargeResource = { cocos2d::Size(2048, 1536), "resources-xlarge" };

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 577);

// マルチレゾリューション対応
#define HOGE() Director::getInstance()->setOpenGLView(eglView); \
Size frameSize = eglView->getFrameSize(); \
std::vector<std::string> searchPath; \
if (frameSize.height > largeResource.size.height) { searchPath.push_back(xlargeResource.directory);Director::getInstance()->setContentScaleFactor(MIN(xlargeResource.size.height / designResolutionSize.height, xlargeResource.size.width / designResolutionSize.width )); \
} else if (frameSize.height > smallResource.size.height) { searchPath.push_back(largeResource.directory); Director::getInstance()->setContentScaleFactor(MIN(largeResource.size.height / designResolutionSize.height, largeResource.size.width / designResolutionSize.width )); \
} else { searchPath.push_back(smallResource.directory); Director::getInstance()->setContentScaleFactor(MIN(smallResource.size.height / designResolutionSize.height, smallResource.size.width / designResolutionSize.width )); } \
FileUtils* fileUtils = FileUtils::getInstance(); \
fileUtils->setSearchPaths(searchPath);

#endif
