# Memory Game
A game created with C/C++ to be played on an Arduino Uno with a I2C LCD 16x2 screen.

The game comes in two modes, practice mode and story mode. In both modes the game consists of arrows being displayed on the screen and the user having to use the Arduino buttons to recall the sequence shown to them. If they get the sequence correct then a green screen is shown with a congratulations message, otherwise a red screen is shown with a consolidation message. 

If the user selects practice mode they are able to fully customize the game by editing: the length of the sequence, the number of symbols used in the sequence and the length of time they have to recall the symbols. This settings can be altered using the Arduino buttons. 

If the user selects story mode they are take through levels of increasing difficulty. 
