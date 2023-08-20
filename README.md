# GBEmu
A game Boy emulator written in C++

- ✅ Passed all [CPU instructions tests](https://github.com/retrio/gb-test-roms/tree/master/cpu_instrs)
- ✅ Video (So-so screen renders properly. With certain specs, 60 FPS can be maintained even in debug mode.)
- ✅ Audio (It sounds fine.)

## Build

- Visual Studio 2022

- [Siv3D](https://github.com/Siv3D/OpenSiv3D) 0.6.10

## References
  
  * [DMG Emulator](https://github.com/voidproc/dmge)
  * [gbemu](https://github.com/jgilchrist/gbemu)
  * [gb-docs-ja](https://github.com/pokemium/gb-docs-ja)
  * [Pan Docs](http://bgb.bircd.org/pandocs.htm)

## Screenshots

<img src="https://github.com/sashi0034/GBEmu/assets/82739042/181d9a71-6b28-400a-9f02-283e020120c3" width="800">

<img src="https://github.com/sashi0034/GBEmu/assets/82739042/893cb732-bc2f-4b4b-8d08-63b4b827a135" width="800">

<img src="https://github.com/sashi0034/GBEmu/assets/82739042/2338a340-3469-48e6-9362-9d47105607bb" width="800">

## Todo

  * GCB mode
  * Many MBCs (So far, only MBC1 has been implemented yet)
  * OpenGL Support (Only HLSL shaders for DirectX have been written yet, GLSL shaders and other areas are not yet complete.)
