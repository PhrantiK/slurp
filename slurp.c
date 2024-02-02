/*
    ┌──────────────────────────────────┐
    │             __                   │░
    │       _____/ /_  ___________     │░
    │      / ___/ / / / / ___/ __ \    │░
    │     (__  ) / /_/ / /  / /_/ /    │░
    │    /____/_/\__,_/_/  / .___/     │░
    │                     /_/          │░
    │                                  │░
    └──────────────────────────────────┘░
     ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
 */

// #include "clip.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <objc/objc-runtime.h>
#include <stdio.h>
#include <stdlib.h>

void copyToClipboard(const char *text) {
  Class NSString = objc_getClass("NSString");
  id nsText = ((id(*)(Class, SEL, const char *))objc_msgSend)(
      NSString, sel_registerName("stringWithUTF8String:"), text);

  Class NSPasteboard = objc_getClass("NSPasteboard");
  id pasteboard = ((id(*)(Class, SEL))objc_msgSend)(
      NSPasteboard, sel_registerName("generalPasteboard"));

  ((void (*)(id, SEL))objc_msgSend)(pasteboard,
                                    sel_registerName("clearContents"));

  // Create an NSArray with one element, nsText
  Class NSArray = objc_getClass("NSArray");
  id objectsToWrite = ((id(*)(Class, SEL, id))objc_msgSend)(
      NSArray, sel_registerName("arrayWithObject:"), nsText);

  ((void (*)(id, SEL, id))objc_msgSend)(
      pasteboard, sel_registerName("writeObjects:"), objectsToWrite);
}

int main(int argc, const char **argv) {
  CGDirectDisplayID displayID = kCGDirectMainDisplay;
  // CGDisplayErr err;
  CGImageRef image;
  CGEventRef event;
  CGPoint cursor;
  CGColorSpaceRef colorSpace;
  CGFloat red, green, blue;

  event = CGEventCreate(NULL);
  cursor = CGEventGetLocation(event);
  CFRelease(event);

  image = CGDisplayCreateImageForRect(displayID,
                                      CGRectMake(cursor.x, cursor.y, 1, 1));
  if (!image) {
    fprintf(stderr, "Error: failed to create image.\n");
    exit(EXIT_FAILURE);
  }

  colorSpace = CGColorSpaceCreateDeviceRGB();
  if (!colorSpace) {
    fprintf(stderr, "Error: failed to create color space.\n");
    exit(EXIT_FAILURE);
  }

  CGContextRef context = CGBitmapContextCreate(NULL, 1, 1, 8, 0, colorSpace,
                                               kCGImageAlphaNoneSkipLast);
  if (!context) {
    fprintf(stderr, "Error: failed to create bitmap context.\n");
    exit(EXIT_FAILURE);
  }

  CGContextDrawImage(context, CGRectMake(0, 0, 1, 1), image);

  const uint8_t *pixelData = CGBitmapContextGetData(context);
  red = (CGFloat)pixelData[0] / 255.0;
  green = (CGFloat)pixelData[1] / 255.0;
  blue = (CGFloat)pixelData[2] / 255.0;

  char hexCode[8];
  snprintf(hexCode, sizeof(hexCode), "#%02x%02x%02x", (int)(round(red * 255)),
           (int)(round(green * 255)), (int)(round(blue * 255)));

  copyToClipboard(hexCode);

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-p") == 0) {
      printf("#%02x%02x%02x\n", (int)(red * 255), (int)(green * 255),
             (int)(blue * 255));
    } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      printf("           __                  \n");
      printf("     _____/ /_  ___________    \n");
      printf("    / ___/ / / / / ___/ __ \\   \n");
      printf("   (__  ) / /_/ / /  / /_/ /   \n");
      printf("  /____/_/\\__,_/_/  / .___/    \n");
      printf("                   /_/         \n");
      printf("\n");
      printf("Usage: %s [-c] [--help]\n", argv[0]);
      printf("\n");
      printf("Options:\n");
      printf("  -p         Print hex code to stdout\n");
      printf("  --help, -h Print usage information\n");
      return 0;
    }
  }

  CGColorSpaceRelease(colorSpace);
  CGContextRelease(context);
  CGImageRelease(image);

  return 0;
}
