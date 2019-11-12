./.sync.sh 1h1cpp

echo "Flatten source code into: GuiLite.h/GuiLite.cpp"

# build GuiLiteRaw.h
cd core_include
cat api.h cmd_target.h rect.h resource.h theme.h surface.h display.h word.h bitmap.h wnd.h audio.h > core.h
mv core.h ../

cd ../widgets_include
cat button.h dialog.h keyboard.h edit.h gesture.h label.h list_box.h slide_group.h spinbox.h table.h wave_buffer.h wave_ctrl.h > widgets.h
mv widgets.h ../

cd ..
cat core.h widgets.h > GuiLiteRaw.h
rm core.h widgets.h

# build GuiLiteRaw.cpp
cd core
cat *.cpp > core.cpp
mv core.cpp ../

cd adapter
cat *.cpp > adapter.cpp
mv adapter.cpp ../../

cd ../../widgets
cat *.cpp > widgets.cpp
mv widgets.cpp ../

cd ..
cat core.cpp adapter.cpp widgets.cpp > GuiLiteRaw.cpp
rm core.cpp adapter.cpp widgets.cpp

# remove include core_include widgets_include from GuiLiteRaw.cpp
sed '/^#include.*core_include\|widgets_include.*/d' GuiLiteRaw.cpp > GuiLiteNoInclude.cpp

# include GuiLite.h to GuiLiteNoInclude.cpp
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
