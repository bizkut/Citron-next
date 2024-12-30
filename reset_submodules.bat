@echo off
title Citron's - Submodule Tool

REM Remove submodules listed in .gitmodules
echo Removing listed submodules...
for /f "tokens=3" %%i in ('git config --file .gitmodules --get-regexp path ^| findstr /r "externals/"') do (
    git submodule deinit -f -- %%i
    git rm -f --cached %%i
)

REM Clean up any remaining submodule directories from the list in .gitmodules
echo Cleaning up remaining submodule directories...
for /f "tokens=3" %%i in ('git config --file .gitmodules --get-regexp path ^| findstr /r "externals/"') do (
    if exist %%i rd /s /q %%i
)

REM Re-add and force-update all submodules to the latest commit
echo Re-adding and force-updating submodules...
git submodule add --force https://github.com/lsalzman/enet.git externals/enet
git submodule add --force https://github.com/mozilla/cubeb.git externals/cubeb
git submodule add --force https://github.com/uzuy-emu/dynarmic.git externals/dynarmic
git submodule add --force https://github.com/libusb/libusb.git externals/libusb/libusb
git submodule add --force https://github.com/uzuy-emu/discord-rpc.git externals/discord-rpc
git submodule add --force https://github.com/KhronosGroup/Vulkan-Headers.git externals/Vulkan-Headers
git submodule add --force https://github.com/uzuy-emu/sirit.git externals/sirit
git submodule add --force https://github.com/uzuy-emu/mbedtls.git externals/mbedtls
git submodule add --force https://github.com/herumi/xbyak.git externals/xbyak
git submodule add --force https://github.com/xiph/opus.git externals/opus
git submodule add --force https://github.com/libsdl-org/SDL.git externals/SDL
git submodule add --force https://github.com/yhirose/cpp-httplib.git externals/cpp-httplib
git submodule add --force https://github.com/FFmpeg/FFmpeg.git externals/ffmpeg/ffmpeg
git submodule add --force https://github.com/microsoft/vcpkg.git externals/vcpkg
git submodule add --force https://github.com/arun11299/cpp-jwt.git externals/cpp-jwt
git submodule add --force https://github.com/bylaws/libadrenotools.git externals/libadrenotools
git submodule add --force https://github.com/lat9nq/tzdb_to_nx.git externals/nx_tzdb/tzdb_to_nx
git submodule add --force https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git externals/VulkanMemoryAllocator
git submodule add --force https://github.com/uzuy-emu/breakpad.git externals/breakpad
git submodule add --force https://github.com/brofield/simpleini.git externals/simpleini
git submodule add --force https://github.com/uzuy-emu/oaknut externals/oaknut
git submodule add --force https://github.com/KhronosGroup/Vulkan-Utility-Libraries.git externals/Vulkan-Utility-Libraries

REM Initialize, update, and force all submodules to the latest commit on their default branch
echo Updating and force-checking out the latest commit for all submodules...
git submodule update --init --recursive --remote

echo All listed submodules have been successfully updated to their latest commits.
pause
