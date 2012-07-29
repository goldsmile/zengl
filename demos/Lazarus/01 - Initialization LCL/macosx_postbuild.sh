# copy binary into bundle
rm ../../../bin/demo01LCL.app/Contents/MacOS/demo01LCL
cp ../../../bin/demo01LCL ../../../bin/demo01LCL.app/Contents/MacOS/demo01LCL
# make Info.plist and copy icon
cp -f demo01LCL_macosx.plist ../../../bin/demo01LCL.app/Contents/Info.plist
cp ../../../bin/data/zengl.icns ../../../bin/demo01LCL.app/Contents/Resources/demo01LCL.icns