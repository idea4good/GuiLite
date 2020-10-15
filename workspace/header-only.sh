echo "Build header-only library: GuiLite.h"

# build GuiLiteRaw.h
cd core_include
cat api.h cmd_target.h resource.h theme.h display.h word.h bitmap.h wnd.h > core.h
mv core.h ../

cd ../widgets_include
cat button.h dialog.h keyboard.h edit.h label.h list_box.h slide_group.h spinbox.h table.h wave_buffer.h wave_ctrl.h > widgets.h
mv widgets.h ../

cd ..
cat core.h widgets.h > GuiLiteRaw.h

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

# remove include core_include widgets_include from GuiLiteRaw.h
sed -i '/^#include.*core_include\|widgets_include.*/d' GuiLiteRaw.h
# remove all #pragma once
sed -i '/^#pragma once/d' GuiLiteRaw.h
# add #pragma once for 1st line
sed -i '1 s/^/#pragma once\n/' GuiLiteRaw.h

# remove include core_include widgets_include from GuiLiteRaw.cpp
sed -i '/^#include.*core_include\|widgets_include.*/d' GuiLiteRaw.cpp

# Delete empty lines or blank lines
sed '/^$/d' GuiLiteRaw.h > GuiLite.h
sed '/^$/d' GuiLiteRaw.cpp > GuiLite.cpp

# Append GuiLite.cpp to GuiLite.h
cat GuiLite.cpp >> GuiLite.h

# Verify
echo '#include "GuiLite.h"' > test.cpp
gcc -c -D GUILITE_ON test.cpp

mv GuiLite.h ../
echo "Done!"
echo "You could find GuiLite.h in root folder"
./.sync.sh GuiLite-header

# clean
rm *.h *.cpp *.o
