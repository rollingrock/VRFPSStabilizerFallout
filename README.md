# VRFPSStabilizerFallout
Port of Skyrim VRFPSStabilizer

# Build Dependencies
* [CommonLibF4](https://github.com/Ryan-rsm-McKenzie/CommonLibF4)
* [CommonLibF4 VR enabled](https://github.com/alandtse/CommonLibF4)
	* Add this as as an environment variable `CommonLibF4Path` or use the submodule in /external
* [fmt](https://github.com/fmtlib/fmt)
* [mmio](https://github.com/Ryan-rsm-McKenzie/mmio)
* [rsm-binary-io](https://github.com/Ryan-rsm-McKenzie/binary_io)
* [spdlog](https://github.com/gabime/spdlog)
* [Xbyak](https://github.com/herumi/xbyak)

# End User Dependencies
* [Address Library for F4SE Plugins](https://www.nexusmods.com/fallout4/mods/47327)
* [VR Address Library for F4SEVR](https://www.nexusmods.com/fallout4/mods/64879)
* [F4SE](https://f4se.silverlock.org/)
* [F4SEVR](https://f4se.silverlock.org/)
* [Microsoft Visual C++ Redistributable for Visual Studio 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)

# Building
```
git clone https://github.com/rollingrock/VRFPSStabilizerFallout.git
cd VRFPSStabilizerFallout
# pull commonlib /external to override the path settings
git submodule init
# to update submodules to checked in build
git submodule update
```

### Fallout 4
```
cmake --preset vs2022-windows-vcpkg
cmake --build build --config Release
```
### VR
```
cmake --preset vs2022-windows-vcpkg-vr
cmake --build buildvr --config Release
```

# Credits
* [Ryan-rsm-Mckenzie's CommonLibF4](https://github.com/Ryan-rsm-McKenzie/CommonlibF4)
* [Shizof's VR FPS Stabilizer](https://www.nexusmods.com/skyrimspecialedition/mods/31392)

# License
[MIT](LICENSE)
