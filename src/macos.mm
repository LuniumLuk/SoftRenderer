#include "Cocoa/Cocoa.h"
#include <mach-o/dyld.h> // dynamic loader lib
// #include <mach/mach_time.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lplatform.h"
#include "../include/limage.h"

// window and IO interfaces for MacOS
// reference : http://glampert.com/2012/11-29/osx-window-without-xcode-and-ib/
// & https://github.com/zauonlok/renderer/blob/master/renderer/platforms/macos.m

struct window
{
    NSWindow *handle;
    unsigned char* image_buffer;
    // image_t *surface;
    /* common data */
    // int should_close;
    // char keys[KEY_NUM];
    // char buttons[BUTTON_NUM];
    // callbacks_t callbacks;
    // void *userdata;
};

#define PATH_SIZE 256

static void create_menubar(void)
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
static void create_application()
{
    if (NSApp == nil) {
        g_auto_release_pool = [[NSAutoreleasePool alloc] init];
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        create_menubar();
        [NSApp finishLaunching];
    }
}
static void initialize_path()
{
    char path[PATH_SIZE];
    uint32_t size = PATH_SIZE;
    // returns a full path to the executable
    _NSGetExecutablePath(path, &size);
    // returns a pointer to the last occurrence of character in the C string str
    *strrchr(path, '/') = '\0';
    chdir(path);
    chdir("assets");
}
static void terminate_application()
{
    assert(g_auto_release_pool != NULL);
    [g_auto_release_pool drain];
    g_auto_release_pool = [[NSAutoreleasePool alloc] init];
}

// @interface WindowDelegate : NSObject <NSWindowDelegate>

// @end

// @implementation WindowDelegate {
//     window_t *_window;
// }

// - (instancetype)initWithWindow:(window_t *)window {
//     self = [super init];
//     if (self != nil) {
//         _window = window;
//     }
//     return self;
// }

// - (BOOL)windowShouldClose:(NSWindow *)sender {
//     UNUSED_VAR(sender);
//     _window->should_close = 1;
//     // YES to allow sender to be closed; otherwise, NO.
//     return NO;
// }

// @end

// /*
//  * for virtual-key codes, see
//  * https://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes
//  */
// static void handle_key_event(window_t *window, int virtual_key, char pressed) {
//     keycode_t key;
//     switch (virtual_key) {
//         case 0x00: key = KEY_A;     break;
//         case 0x02: key = KEY_D;     break;
//         case 0x01: key = KEY_S;     break;
//         case 0x0D: key = KEY_W;     break;
//         case 0x31: key = KEY_SPACE; break;
//         default:   key = KEY_NUM;   break;
//     }
//     if (key < KEY_NUM) {
//         window->keys[key] = pressed;
//         if (window->callbacks.key_callback) {
//             window->callbacks.key_callback(window, key, pressed);
//         }
//     }
// }


// @interface ContentView : NSView
// @end

// @implementation ContentView {
//     window_t *_window;
// }

// - (instancetype)initWithWindow:(window_t *)window {
//     self = [super init];
//     if (self != nil) {
//         _window = window;
//     }
//     return self;
// }

// - (BOOL)acceptsFirstResponder {
//     return YES;  /* to receive key-down events */
// }

// - (void)drawRect:(NSRect)dirtyRect {
//     image_t *surface = _window->surface;
//     NSBitmapImageRep *rep = [[[NSBitmapImageRep alloc]
//             initWithBitmapDataPlanes:&(surface->ldr_buffer)
//                           pixelsWide:surface->width
//                           pixelsHigh:surface->height
//                        bitsPerSample:8
//                      samplesPerPixel:3
//                             hasAlpha:NO
//                             isPlanar:NO
//                       colorSpaceName:NSCalibratedRGBColorSpace
//                          bytesPerRow:surface->width * 4
//                         bitsPerPixel:32] autorelease];
//     NSImage *nsimage = [[[NSImage alloc] init] autorelease];
//     [nsimage addRepresentation:rep];
//     [nsimage drawInRect:dirtyRect];
// }

// - (void)keyDown:(NSEvent *)event {
//     handle_key_event(_window, [event keyCode], 1);
// }

// - (void)keyUp:(NSEvent *)event {
//     handle_key_event(_window, [event keyCode], 0);
// }

// - (void)mouseDown:(NSEvent *)event {
//     UNUSED_VAR(event);
//     handle_button_event(_window, BUTTON_L, 1);
// }

// - (void)mouseUp:(NSEvent *)event {
//     UNUSED_VAR(event);
//     handle_button_event(_window, BUTTON_L, 0);
// }

// - (void)rightMouseDown:(NSEvent *)event {
//     UNUSED_VAR(event);
//     handle_button_event(_window, BUTTON_R, 1);
// }

// - (void)rightMouseUp:(NSEvent *)event {
//     UNUSED_VAR(event);
//     handle_button_event(_window, BUTTON_R, 0);
// }

// - (void)scrollWheel:(NSEvent *)event {
//     float offset = (float)[event scrollingDeltaY];
//     if ([event hasPreciseScrollingDeltas]) {
//         offset *= 0.1f;
//     }
//     handle_scroll_event(_window, offset);
// }

// @end

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

// - (BOOL)acceptsFirstResponder {
//     return YES;  /* to receive key-down events */
// }

// static 

// Trouble Shot: 
// - Inconsistent set of values to create NSBitmapImageRep (Fixed: bytesperrow must be set in consisten with bitsperpixel)
// - upside down fix
// - BGR -> RGB

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
    create_menubar();

    // Show window and run event loop.
    [handle orderFrontRegardless];
    [NSApp run];

    // Block here

    [g_auto_release_pool drain];

    return 0;
}