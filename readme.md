## Waypoints Game (for Arduino)

A few years back, I read a story about a guy who made this box. On the outside of this box, there was a screen and a button. When the button was pressed, the screen showed a distance in Miles. There was no direction.

This same guy took his girlfriend out one day, with the box. He made a rule for the box - you could only have 10 button pushes. He choose a special spot for them both, and they took off. They found that the trick is to draw a circle on the map that matches the given distance around your current location. With enough circles, they made it to their waypoint. The box unlocked, and inside was a ring. Engagement, huzzah!

I wanted to make the same thing since I'm becoming gifted with dealing with GPS and coordinate based systems. I wanted the experience and more importantly I want to play this game with my friends soon. No engagement rings, or even unlocking functionality here - my game would involve a prize buried at a location!

This code represents the beginning of that game. I'm currently brainstorming some dynamic rules for it (like the Arduino showing the distance for 10s every random amount of minutes underneath 20 minutes).

## Getting Started
Like most Arduino projects, this is not going to be a complete solution for you and your hardware, unless you copycat my setup. I choose an older [Arduino UNO SMD edition](https://www.sparkfun.com/products/10356), an [EM-406A GPS Module](https://www.sparkfun.com/products/465), and a [Parallax LCD Screen](http://www.parallax.com/tabid/768/productid/50/default.aspx) (With Backlight).

You'll need to hook these all up and have them working independently prior to dropping in the code. When you have the components working, change some of the variables in the top of the INO accordingly. Then turn it on and TADA, Waypoints Game!

## Technical
This project thought me a good chunk about how to handle multiple serial devices with Arduino. For the GPS, I use the hardware UART because of it's reliability, and for the LCD I used SoftwareSerial. The GPS was returning a ton of garbage data for me at first, so I tried writing the implementation in lower C (meaning *less* Strings, not *no* Strings) and now it works absolutely wonderful!

If you choose the same GPS module as me (which I recommend, it's a workhorse of a module), I recommend running this bit of code before you jump off:

    Serial.write("$PSRF103,01,00,00,01*25\r\n");
    Serial.write("$PSRF103,02,00,00,01*26\r\n");
    Serial.write("$PSRF103,03,00,00,01*27\r\n");
    Serial.write("$PSRF103,04,00,00,01*20\r\n");
    Serial.write("$PSRF103,05,00,00,01*21\r\n");
    
That will disable all the GPS messages other than the GPGGA sentence, which contains all the data we need.

## It doesn't work!
This project was made for my pleasure and wasn't created with dynamic use cases in mind. Therefore, some things may be static for my setup (I think my altitude code may break things) and need your own craftiness to mend. Just fork and [tweet at me](http://twitter.com/kylehotchkiss) if you change some things to make it work better for you - I'd love to hear your milage!