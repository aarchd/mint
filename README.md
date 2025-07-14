# Mint

Mint is a [minui](https://github.com/droidian/android-minui)-based splash and info display tool for Linux and Android devices

## Features

 - Battery status graphic for offline charging
 - Splash
 - On-screen text graphics for displaying logs

## Deps

 - `minui` 
 - `cairo`
 - `freetype2`
 - `xxd`
 - `meson`

## Build

```bash
   meson setup build --prefix=/usr
   ninja -C build -j($nproc)
   sudo ninja -C build install

```

## Extra
mkinitcpio hooks for mint can be found [here](https://github.com/aarchd/PKGBUILDs/tree/main/mint)
