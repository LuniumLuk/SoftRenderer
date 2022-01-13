#include "Cocoa/Cocoa.h"
#include <mach-o/dyld.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include "platform.hpp"

using namespace Lurdr;

#define PATH_SIZE 256

// window and IO interfaces for MacOS
// reference : http://glampert.com/2012/11-29/osx-window-without-xcode-and-ib/
// & https://github.com/zauonlok/renderer/blob/master/renderer/platforms/macos.m
struct Lurdr::APPWINDOW
{
    NSWindow    *handle;
    byte_t      *surface;
    bool        keys[KEY_NUM];
    bool        buttons[BUTTON_NUM];
    bool        should_close;
    void        (*keyboardCallback)(AppWindow *window, KEY_CODE key, bool pressed);
    void        (*mouseButtonCallback)(AppWindow *window, MOUSE_BUTTON button, bool pressed);
    void        (*mouseScrollCallback)(AppWindow *window, float offset);
    void        (*mouseDragCallback)(AppWindow *window, float x, float y);
};

NSAutoreleasePool *g_auto_release_pool;

static void initializeMenuBar()
{
    // reference : https://github.com/zauonlok/renderer/blob/master/renderer/platforms/macos.m
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

static void initializeWorkingDirectory()
{
    char path[PATH_SIZE];
    uint32_t size = PATH_SIZE;
    _NSGetExecutablePath(path, &size);
    *strrchr(path, '/') = '\0';
    chdir(path);
}

void Lurdr::initializeApplication()
{
    if (NSApp == nil) {
        // Autorelease Pool:
        // Objects declared in this scope will be automatically released at the end of it, when the pool is 'drained'.
        g_auto_release_pool = [[NSAutoreleasePool alloc] init];
        // Create a shared app instance.
        // This will initialize the global variable 'NSApp' with the application instance.
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        initializeWorkingDirectory();
        initializeMenuBar();

        [NSApp finishLaunching];
    }
}

void Lurdr::terminateApplication()
{
    assert(g_auto_release_pool != NULL);
    [g_auto_release_pool drain];
    g_auto_release_pool = [[NSAutoreleasePool alloc] init];
}

void Lurdr::updateView(AppWindow *window)
{
    [[window->handle contentView] setNeedsDisplay:YES];  // invoke drawRect
}

// virtual-key codes reference : https://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes
void handleKeyEvent(AppWindow *window, int virtual_key, bool pressed)
{
    KEY_CODE key;
    switch (virtual_key) {
        case 0x00: key = KEY_A;      break;
        case 0x02: key = KEY_D;      break;
        case 0x01: key = KEY_S;      break;
        case 0x0D: key = KEY_W;      break;
        case 0x31: key = KEY_SPACE;  break;
        case 0x35: key = KEY_ESCAPE; break;
        default:   key = KEY_NUM;    break;
    }
    if (key < KEY_NUM)
    {
        window->keys[key] = pressed;
        if (window->keyboardCallback)
        {
            window->keyboardCallback(window, key, pressed);
        }
    }
}

void handleMouseDrag(AppWindow *window, float x, float y)
{
    if (window->mouseDragCallback)
    {
        window->mouseDragCallback(window, x, y);
    }
}

@interface WindowDelegate : NSObject <NSWindowDelegate>
@end

@implementation WindowDelegate {
    AppWindow *_window;
}

- (instancetype)initWithWindow:(AppWindow *)window {
    self = [super init];
    if (self != nil) {
        _window = window;
    }
    return self;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    (void)sender;
    _window->should_close = true;
    return NO;
}

@end

@interface ContentView : NSView
@end

@implementation ContentView
{
    AppWindow* _window;
}

- (instancetype)initWithWindow:(AppWindow*)window
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
    NSBitmapImageRep *rep = [[[NSBitmapImageRep alloc]
            initWithBitmapDataPlanes:&(_window->surface)
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

AppWindow* Lurdr::createWindow(const char *title, int width, int height, unsigned char *surface_buffer)
{
    NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

    NSRect windowRect = NSMakeRect(0, 0, width, height);
    NSWindow * handle = [[NSWindow alloc] initWithContentRect:windowRect
                                          styleMask:windowStyle
                                          backing:NSBackingStoreBuffered
                                          defer:NO];
    assert(handle != nil);
    [handle autorelease];

    AppWindow *window = new AppWindow();
    window->handle = handle;
    window->surface = surface_buffer;

    WindowDelegate *delegate;
    delegate = [[WindowDelegate alloc] initWithWindow:window];
    assert(delegate != nil);
    [handle setDelegate:delegate];

    ContentView *view;
    view = [[[ContentView alloc] initWithWindow:window] autorelease];

    [handle setTitle:[NSString stringWithUTF8String:title]];
    [handle setColorSpace:[NSColorSpace genericRGBColorSpace]];
    [handle setContentView:view];
    [handle makeFirstResponder:view];
    [handle orderFrontRegardless];
    [handle makeKeyAndOrderFront:nil];

    return window;
}

void Lurdr::runApplication()
{
    // block
    [NSApp run];
    // how to make NSApp not block : https://stackoverflow.com/questions/48020222/how-to-make-nsapp-run-not-block
    // NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
    //                                     untilDate:[NSDate distantFuture]
    //                                        inMode:NSDefaultRunLoopMode
    //                                       dequeue:YES];
    // [NSApp sendEvent:event];
}

// reference : https://github.com/zauonlok/renderer/blob/master/renderer/platforms/macos.m
void Lurdr::pollEvent()
{
    while (true)
    {
        NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                            untilDate:[NSDate distantPast]
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES];
        if (event == nil)
        {
            break;
        }
        [NSApp sendEvent:event];
    }
    terminateApplication();
}

bool Lurdr::windowShouldClose(AppWindow *window)
{
    return window->should_close;
}

/**
 * input & callback registrations
 */
void Lurdr::setKeyboardCallback(AppWindow *window, void(*callback)(AppWindow*, KEY_CODE, bool))
{
    window->keyboardCallback = callback;
}

void Lurdr::setMouseButtonCallback(AppWindow *window, void(*callback)(AppWindow*, MOUSE_BUTTON, bool))
{
    window->mouseButtonCallback = callback;
}

void Lurdr::setMouseScrollCallback(AppWindow *window, void(*callback)(AppWindow*, float))
{
    window->mouseScrollCallback = callback;
}

void Lurdr::setMouseDragCallback(AppWindow *window, void(*callback)(AppWindow*, float, float))
{
    window->mouseDragCallback = callback;
}
