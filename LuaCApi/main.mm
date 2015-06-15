//
//  main.m
//  LuaCApi
//
//  Created by zengqiang on 15/5/15.
//
//

#include <stdio.h>
#import <UIKit/UIKit.h>
#import <objc/NSObject.h>

#include "LuaObject.h"

void doLuaFile(const char *);
void initLuaState();

char AppPath[200];



void TestDoLuaFile()
{
    char file_name[500];
    
    sprintf(file_name,"%s%s",AppPath,"test.lua");
    initLuaState();
    doLuaFile(file_name);
}

int main(int argc, char * argv[])
{
    NSString* readPath = [NSString stringWithFormat:@"%@%@", [[NSBundle mainBundle] bundlePath], @"/"];
    sprintf(AppPath,"%s",[readPath UTF8String]);
    
    
    TestDoLuaFile();
    return 0;
}



