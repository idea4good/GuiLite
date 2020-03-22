echo "Build header-only library: GuiLite.h"

# build GuiLiteRaw.h
cd core_include
cat api.h cmd_target.h rect.h resource.h theme.h display.h word.h bitmap.h wnd.h audio.h > core.h
mv core.h ../

cd ../widgets_include
cat button.h dialog.h keyboard.h edit.h label.h list_box.h slide_group.h spinbox.h table.h wave_buffer.h wave_ctrl.h > widgets.h
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

# remove include core_include widgets_include from GuiLiteRaw.h
sed '/^#include.*core_include\|widgets_include.*/d' GuiLiteRaw.h > GuiLiteNoInclude.h

# remove include core_include widgets_include from GuiLiteRaw.cpp
sed '/^#include.*core_include\|widgets_include.*/d' GuiLiteRaw.cpp > GuiLiteNoInclude.cpp

# Delete empty lines or blank lines
sed '/^$/d' GuiLiteNoInclude.h > GuiLite.h
sed '/^$/d' GuiLiteNoInclude.cpp > GuiLite.cpp

# Append GuiLite.cpp to GuiLite.h
cat GuiLite.cpp >> GuiLite.h

# Verify
echo '#include "GuiLite.h"' > test.cpp
gcc -c -D GUILITE_ON test.cpp

# clean
rm GuiLiteRaw.h GuiLiteNoInclude.h GuiLiteRaw.cpp GuiLiteNoInclude.cpp GuiLite.cpp test.cpp
mv GuiLite.h ../

echo "Done!"
echo "You could find GuiLite.h in root folder"
./.sync.sh GuiLite-header
