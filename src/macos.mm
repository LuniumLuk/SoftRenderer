#include "Cocoa/Cocoa.h"
#include <mach-o/dyld.h> // dynamic loader lib
// #include <mach/mach_time.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/platform.hpp"
#include "../include/image.hpp"

// window and IO interfaces for MacOS
// reference : http://glampert.com/2012/11-29/osx-window-without-xcode-and-ib/
// & https://github.com/zauonlok/renderer/blob/master/renderer/platforms/macos.m

struct window
{
    NSWindow *handle;
    byte_t* image_buffer;
    // image_t *surface;
    /* common data */
    // int should_close;
    bool keys[KEY_NUM];
    // char buttons[BUTTON_NUM];
    void (*keyCallback)(window_t *window, KEY_CODE key, bool pressed);
    // void (*buttonCallback)(window_t *window, MOUSE_BUTTON button, bool pressed);
    // void (*scrollCallback)(window_t *window, float offset);
    // callbacks_t callbacks;
    // void *userdata;
};

#define PATH_SIZE 256

// reference : https://github.com/zauonlok/renderer/blob/master/renderer/platforms/macos.m
static void createMenuBar()
{
    NSMenu *menu_bar, *app_menu;
    NSMenuItem *app_menu_item, *quit_menu_item;
    NSString *app_name, *quit_title;

    menu_bar = [[[NSMenu alloc] init] autorelease];
    [NSApp setMainMenu:menu_bar];

    app_menu_item = [[[NSMenuItem alloc] init] autorelease];
    [menu_bar addItem:app_menu_item];

    app_menu = [[[NSMenu alloc] init] autorelease];
    [app_menu_item setSubmenu:app_menu];

    app_name = [[NSProcessInfo processInfo] processName];
    quit_title = [@"Quit " stringByAppendingString:app_name];
    quit_menu_item = [[[NSMenuItem alloc] initWithTitle:quit_title
                                                 action:@selector(terminate:)
                                          keyEquivalent:@"q"] autorelease];
    [app_menu addItem:quit_menu_item];
}
// global variables
static NSAutoreleasePool *g_auto_release_pool;

// note : static function makes this function visible by linker
// occupying the signature in the function definition pool
static void createApplication()
{
    if (NSApp == nil) {
        g_auto_release_pool = [[NSAutoreleasePool alloc] init];
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        createMenuBar();
        [NSApp finishLaunching];
    }
}
static void setupEnvironment()
{
    char path[PATH_SIZE];
    uint32_t size = PATH_SIZE;
    // returns a full path to the executable
    _NSGetExecutablePath(path, &size);
    *strrchr(path, '/') = '\0';
    chdir(path);
    chdir("assets");
}
void terminateApplication()
{
    assert(g_auto_release_pool != NULL);
    [g_auto_release_pool drain];
    g_auto_release_pool = [[NSAutoreleasePool alloc] init];
}



// virtual-key codes : https://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes
static void handleKeyEvent(window_t *window, int virtual_key, bool pressed)
{
    KEY_CODE key;
    if (pressed)
    {
        printf("Key down: [%d]\n", key);
    }
    else
    {
        printf("Key up: [%d]\n", key);
    }
    switch (virtual_key) {
        case 0x00: key = KEY_A;     break;
        case 0x02: key = KEY_D;     break;
        case 0x01: key = KEY_S;     break;
        case 0x0D: key = KEY_W;     break;
        case 0x31: key = KEY_SPACE; break;
        default:   key = KEY_NUM;   break;
    }
    if (key < KEY_NUM) {
        window->keys[key] = pressed;
        // if (window->callbacks.key_callback) {
        //     window->callbacks.key_callback(window, key, pressed);
        // }
    }
}

static void handleMouseDrag(window_t *window, float x, float y)
{
    printf("Mouse Position: [%6.2f, %6.2f]\n", x, y);
}

@interface ContentView : NSView
@end

@implementation ContentView
{
    window_t *_window;
}

- (instancetype)initWithWindow:(window_t *)window
{
    self = [super init];
    if (self != nil)
    {
        _window = window;
    }
    return self;
}

- (void)keyDown:(NSEvent *)event {
    handleKeyEvent(_window, [event keyCode], true);
}

- (void)keyUp:(NSEvent *)event {
    handleKeyEvent(_window, [event keyCode], false);
}

- (void)mouseDragged:(NSEvent *)event {
    NSPoint locationInView = [self convertPoint:[event locationInWindow] fromView:nil];
    handleMouseDrag(_window, locationInView.x, locationInView.y);
}

// A Boolean value that indicates whether the responder accepts first responder status.
- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // image_t *surface = _window->surface;
    NSBitmapImageRep *rep = [[[NSBitmapImageRep alloc]
            initWithBitmapDataPlanes:&(_window->image_buffer)
                          pixelsWide:512
                          pixelsHigh:512
                       bitsPerSample:8
                     samplesPerPixel:3
                            hasAlpha:NO
                            isPlanar:NO
                      colorSpaceName:NSCalibratedRGBColorSpace
                         bytesPerRow:512 * 3
                        bitsPerPixel:24] autorelease];
    NSImage *nsimage = [[[NSImage alloc] init] autorelease];
    [nsimage addRepresentation:rep];
    [nsimage drawInRect:dirtyRect];
}

@end

static void present_surface(window_t *window)
{
    [[window->handle contentView] setNeedsDisplay:YES];  /* invoke drawRect */
}

using namespace Lurdr;

int main(int argc, const char * argv[])
{
    window_t _window;
    BMPImage image("assets/lenna.bmp");
    image.printImageInfo();

    UniformImage u_image(image);
    u_image.convertColorSpace(COLOR_RGB);

    _window.image_buffer = u_image.getImageBuffer();

    // Autorelease Pool:
    // Objects declared in this scope will be automatically
    // released at the end of it, when the pool is "drained".
    g_auto_release_pool = [[NSAutoreleasePool alloc] init];

    // Create a shared app instance.
    // This will initialize the global variable
    // 'NSApp' with the application instance.
    [NSApplication sharedApplication];

    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp finishLaunching];

    //
    // Create a window:
    //

    // Style flags:
    NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

    const char *title = "Viewer @ Lu Renderer";
    int width = 512;
    int height = 512;

    // Window bounds (x, y, width, height).
    NSRect windowRect = NSMakeRect(0, 0, width, height);
    NSWindow * handle = [[NSWindow alloc] initWithContentRect:windowRect
                                          styleMask:windowStyle
                                          backing:NSBackingStoreBuffered
                                          defer:NO];
    assert(handle != nil);
    [handle autorelease];
   
    ContentView *view;
    view = [[[ContentView alloc] initWithWindow:&_window] autorelease];

    [handle setTitle:[NSString stringWithUTF8String:title]];
    [handle setColorSpace:[NSColorSpace genericRGBColorSpace]];
   
    [handle setContentView:view];
    // [handle makeFirstResponder:view];

    // Window controller:
    NSWindowController * windowController = [[NSWindowController alloc] initWithWindow:handle];
    [windowController autorelease];

    // This will add a simple text view to the window,
    // so we can write a test string on it.
    // === [ORIGIN TEXT VIEW] ===
    // NSTextView * textView = [[NSTextView alloc] initWithFrame:windowRect];
    // [textView autorelease];
    // [handle setContentView:textView];
    // [textView insertText:@"Hello OSX/Cocoa world!" replacementRange:NSMakeRange(1, 0)];
    // ==========================

    // TODO: Create app delegate to handle system events.

    // TODO: Create menus (especially Quit!)
    createMenuBar();

    // Show window and run event loop.
    [handle orderFrontRegardless];
    [NSApp run];

    // Block here

    [g_auto_release_pool drain];

    return 0;
}