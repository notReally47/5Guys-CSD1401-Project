You can create a text file with all the inputs in it. (Example 'Seven11_Level_2_Input.txt')

Run wsl in current directory (level_mapper) and run 'gcc -o level_generate_CSV.exe level_generate_CSV.c' to get the latest version of the exe file.

If level_generate_CSV.c was edited, compile into an executable file in wsl with:
gcc -o level_generate_CSV.exe level_generate_CSV.c

Run level_generate_CSV.exe in cmd or wsl:
./level_generate_CSV.exe

Or feed in the Input File:
./level_generate_CSV.exe < level_inputs/Seven11_Level_2_Input.txt

The program will ask for different inputs. Please read and follow the input format correctly.

The program will then generate the level csv file, the level csv backup file & the notes file (for reference).

IMPORTANT!!
Please do not edit the csv in Excel/Sheets and save, for some reason this will change the format
of the CSV file and hang the game when loading the newly-saved CSV File!!

If messed up, rename the main CSV file & remove '_bak' from the backup CSV File.

If need to edit, please use notepad++ and use the csv file & notes file side by side as reference to the cell.
e.g:
Open both 'Seven11_Level_1.csv' & 'Seven11_Notes_Level_1.txt' in Notepad++.
On one of the file, click 'View' > 'Move/Clone Current Document' > 'Move to Other View'
Make sure bothe files are at the top.
Click 'View' > 'Synchronise Vertical Scrolling'
Click 'View' > 'Synchronise Horizontal Scrolling'
Edit and Save.

Please refer to screenshots in 'instruction_example_screenshots' for example.