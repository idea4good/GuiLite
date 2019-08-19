echo "Merge GuiLite source code into: 1 hearder & 1 source file"
echo ""
echo "Choose 1: Build for Linux"
echo "Choose 2: Build for Windows"
echo "Choose 3: Build for None OS or any OS"
echo "Choose 4: exit"

read -p "Please input:[1-3]:" input

# build GuiLite.h
cd core_include
cat api.h cmd_target.h rect.h msg.h resource.h theme.h surface.h display.h word.h bitmap.h wnd.h audio.h > core.h
mv core.h ../

cd ../widgets_include
cat button.h dialog.h keyboard.h edit.h gesture.h label.h list_box.h slide_group.h spinbox.h table.h wave_buffer.h wave_ctrl.h > widgets.h
mv widgets.h ../

cd ..
cat core.h widgets.h > GuiLiteRaw.h
rm core.h widgets.h

# build GuiLite.cpp
cd core
cat *.cpp > core.cpp
mv core.cpp ../
cd adapter

while :
do
  case $input in
    1)
      echo "Choose 1"
      cat *linux*.cpp > adapter.cpp
      break
      ;;
    2)
      echo "Choose 2"
      cat *win*.cpp > adapter.cpp
      break
      ;;
    3)
      echo "Choose 3"
      cat *unknow*.cpp > adapter.cpp
      break
      ;;
    *)
      rm ../../GuiLiteRaw.h ../../core.cpp
      exit 0
      ;;
    esac
done

mv adapter.cpp ../../

cd ../../widgets
cat *.cpp > widgets.cpp
mv widgets.cpp ../

cd ..
cat core.cpp adapter.cpp widgets.cpp > GuiLiteRaw.cpp
rm core.cpp adapter.cpp widgets.cpp

# remove include core_include widgets_include from GuiLite.cpp
sed '/^#include.*core_include\|widgets_include.*/d' GuiLiteRaw.cpp > GuiLiteNoInclude.cpp

# include GuiLite.h to GuiLite.cpp
sed -i '1s/^/#include "GuiLite.h" /' GuiLiteNoInclude.cpp

# Delete empty lines or blank lines
sed '/^$/d' GuiLiteRaw.h > GuiLite.h
sed '/^$/d' GuiLiteNoInclude.cpp > GuiLite.cpp

# Verify
gcc -c GuiLite.cpp

# clean
rm GuiLiteRaw.h GuiLiteRaw.cpp GuiLiteNoInclude.cpp

echo "Done!"
echo "You could find GuiLite.h/GuiLite.cpp in this folder"
