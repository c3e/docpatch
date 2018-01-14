# DocPatch AppImage

This directory contains the base files needed for building the DocPatch AppImage.

## Building the AppImage

AppImageTool needs to be installed to build the AppImage. It needs to be on the path under the name `appimagetool`.

To build an AppImage, edit the `Makefile` and set `prefix` to `/usr`.

```sh
# edit Makefile
make
make appimage
```
The AppImage can be found in the root directory of the project.

## Known limitations

- Dependent software is not packaged inside the AppImage and needs to be installed separately.
- The image is built for the `x86_64` architecture only although it should be architecture agnostic.

## Notes

The `AppRun` file was copied from the template on https://github.com/AppImage/AppImageKit/raw/appimagetool/master/resources/AppRun.
