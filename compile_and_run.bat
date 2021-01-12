@echo off
color 02
echo ################################chess#####################################
g++ -DWIN32 -std=c++17 -o chess.exe yadro.cpp  main.cpp bot_chess.cpp Graph_chess.cpp End.cpp Menu.cpp  Graph_lib\Graph.cpp Graph_lib\GUI.cpp Graph_lib\Window.cpp -I./Graph_lib -I./fltk_win64/include -L./fltk_win64/lib -lfltk_images -lfltk -lfltk_png -lz -lfltk_jpeg -lole32 -luuid -lcomctl32 -mwindows
echo .
echo .
echo .
echo ################################Done!######################################
pause
echo running...
echo .  
echo . 
echo . 
chess.exe
pause
