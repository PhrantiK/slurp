// #import "clip.h"
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

void copyToClipboard(const char *text) {
  @autoreleasepool {
    NSString *nsText = [NSString stringWithUTF8String:text];
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
    [pasteboard writeObjects:@[ nsText ]];
  }
}
