#import "dialog.h"
#import <Cocoa/Cocoa.h>

void Alert() {
	int response = NSRunAlertPanel(
		@"OpenGL 2.0 not supported.",
		@"Please check that your video card supports OpenGL 2.0 or higher and that your system and drivers are up-to-date.",
		@"Close", @"Ignore (not recommended)", nil
	);
	// user chose (wisely) to close.
	if (response == NSAlertDefaultReturn)
		exit(1);
}
