// insert cover image

At first glance, it's just a joystick-controlled fish that occasionally chases after a pixelated worm. Don't get me wrong, I spent quite some time implementing movement mechanics to make the fish as convincing as possible, but one must dive deeper into the coy pond to see the bulk of the effort. Beneath the surface, I present to you the *ability* to animate a fish, or any creature for that matter.

To accomplish this, I created a library called LCDMap that provides an overlay for custom character generation through the Arduino LiquidCrystal library. At its core, LCDMap converts an image to a series of custom characters, taking into account the image's rotation and relative position on the screen. This middleman approach allows the user to shift, rotate, and animate *sprites* without having to work directly with bytes.

So what's so special about LCDMap? 

Well, to truly appreciate what it does, we must first look at how custom characters are generated. This is done using createChar(num, data) 

// insert image about custom character -> lcd character

An important thing to note is that Arduino character displays, such as [insert model], only have enough onboard memory for **eight** custom characters. In other words, that's how many you get before having to overwrite an existing one. I know what you're thinking - what if you draw a character, then reuse the same memory location to draw another one? Unfortunately, anything printed on the LCD is directly tied to its physical slot in memory, so any changes are instantly reflected in all instances of that custom character.

Knowing this, let's look at some of the behind-the-scenes features of LCDMap that minimize memory usage.

**Optimized LCD memory:** characters are only created if they contain at least one drawn pixel, even if an image is ovelapping. 

**Optimized Arduino memory:** images are stored in a continuous array of bytes, where every bit represents the state of a pixel. This way, a 4x4 pixel image is saved using only two bytes.

**Eight-directional rotation:** one way to rotate an image is to create eight different versions - one for each direction. LCDMap accomplishes this using only two versions of the image, horizontal and diagonal. The two images are read in normal/reverse row/column order to "turn" in all other directions.

For high-level information about what the LCDMap does and how to use it, please refer to the documentation in [this repository]. Personally, I find that documentation is easier to understand after some hands-on experience with the functionality of the software.

And what better way to demonstrate LCDMap than with an example? [BouncyBalls] and [AnimatedFish] are Arduino programs that utilize most of the functions defined in the library.

// insert image of bouncy balls

BouncyBalls is the simpler of the two, serving as a starting point for users who are unfamiliar with LCDMap without cluttering the program with complex movement and rotation mechanics. The title perfectly summarizes its functionality: eight pixel-sized sprites move around and repel from the edges of the LCD. Once a ball has reached a vertical or horizontal boundary, its rotation is flipped respectively to create the illusion of a bounce.

That's pretty much it. In addition to movement and rotation, BouncyBalls also demonstrates how multiple sprites are drawn on the same custom character if they all overlap, but this is feature attributed to LCDMap.

// insert image of animated fish

// describe lcd fish

// describe basic elements
// wiring
// including libraries
// draw method








##References##

// lcd fish

// bouncy balls

// LCDMap

// wiring arduino display

// creating characters with Arduino
