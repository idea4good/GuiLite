echo "Build header-only library: GuiLite.h"

# build GuiLiteRaw.h
cd core
cat api.h resource.h theme.h display.h word.h image.h wnd.h > core.h
mv core.h ../
cd ..

cd widgets
cat button.h dialog.h keyboard.h edit.h label.h list_box.h slide_group.h spinbox.h table.h wave_buffer.h wave_ctrl.h > widgets.h
mv widgets.h ../
cd ..

cat core.h widgets.h > GuiLiteRaw.h

# build GuiLiteRaw.cpp
cp core/core.cpp ./
cp widgets/widgets.cpp ./

cd core/adapter
cat *.cpp > adapter.cpp
mv adapter.cpp ../../
cd ../..

cat core.cpp adapter.cpp widgets.cpp > GuiLiteRaw.cpp
# remove include core widgets from GuiLiteRaw.h
sed -i '/^#include.*core\|widgets.*/d' GuiLiteRaw.h
# remove all #pragma once
sed -i '/^#pragma once/d' GuiLiteRaw.h
# add #pragma once for 1st line
sed -i '1 s/^/#pragma once\n/' GuiLiteRaw.h

# remove include core widgets from GuiLiteRaw.cpp
sed -i '/^#include.*core\|widgets.*/d' GuiLiteRaw.cpp

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

# clean
rm *.h *.cpp *.o
