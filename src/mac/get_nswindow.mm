#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

const void* getNSWindow(const void* nsView) {
  return (__bridge const void*)[(__bridge NSView*)nsView window];
}