BUILD INSTRUCTIONS:
- Build and compile the "NetworkingUtils" project
- Take the .lib it generates from $(SolutionDir)x64/$(Configuration) and put it in $(SolutionDir)Extern/libs/x64/$(Configuration)
- Take the DLL's located in $(SolutionDir)Extern/DLLs/$(Configuration) and place them in $(SolutionDir)x64/$(Configuration)
- Once that is done, you should be able to compile and run the servrer and client

CONTROLS:
WASD - Move
Space - Jump
Left Click - Hit other players while looking at them
'+' - Enable client side prediction
'-' - Disable client side prediction
'0' - Enable Dead reckoning
'9' - Disable dead reckoning
'8' - Enable interpolation
'7' - Disable interpolation

'1' - Set Frequency to 1
'2' - Set Frequency to 10
'3' - Set Frequency to 60
'4' - Set Frequency to 100

Demo Video: 