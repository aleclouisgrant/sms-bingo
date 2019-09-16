# Super Mario Sunshine (SMS) Bingo

C++/Qt5 application that hooks into Dolphin's emulated RAM to read values for SMS Bingo. Bingo spaces are not selectable until the proper values have been read from the game's RAM. Once the application sees the correct requirements have been met to allow the player to select the corresponding bingo space, the space turns red. The player can then select the space themselves. The application can also immediately select the corresponding space automatically once the requirements have been met. No changes to Dolphin have to be made for this to work - the program does all the work hooking to Dolphin itself.

This project is intended to have online multiplayer as well as a spectator client that allows for a detailed look at what each player has acquired over the course of the bingo game. This application is also intended to work with the Wii's USB port so people can play over console and cross platform (console/Dolphin). 

If you're interested in hearing more and working with me on this project (even to help find memory addresses of specific values), feel free to email me at:
aleclouisgrant@gmail.com