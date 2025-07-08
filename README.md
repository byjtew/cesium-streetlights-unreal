# Cesium Street-lights for Unreal

A plugin extension for the [Cesium for Unreal](https://github.com/CesiumGS/cesium-unreal) plugin that enables users to create and manage street-lighting from OpenStreetMap data.

![Picture of Zurich at night](Resources/ZurichAtNight.png)

> [!CAUTION]  
> This repository is in **early development** and is not ready **at all** for any production or development use yet.
> Not working version is available on this GitHub repository for now.

## Features

- ✅ Parsing of `.osm.pbf` files to extract street-lighting data.
- ⌛ Parsing of `.parquet` files to extract street-lighting data.
- ⌛ HTTP API to fetch street-lighting data from OSM or Overpass.
- ⌛ Independent height adjustment per street-light.
- ✅ Customisable global lighting parameters.
- ⌛ LOD selection.
- ⌛ Customizable lighting parameters per LOD.

### Editor specific

- ⌛ Add static streetlights in a specified bounding-box area.
- ⌛ Add static streetlights in a specified radius from a geolocation.

### Runtime specific

- ⌛ Add streetlights dynamically in the main controller camera view.

## Installation from a release

> [!NOTE]
> To be filled in later.

## Installation from source

### ThirdParty libraries

<details>
<summary>On Windows:</summary>
```shell
cd Source/ThirdParty
git clone https://github.com/Microsoft/vcpkg.git _vcpkg
cd _vcpkg
./bootstrap-vcpkg.bat
cd ..
./_vcpkg/vcpkg.exe install --x-install-root=. --triplet=x64-windows
```
</details>

<details>
<summary>On Linux:</summary>
```shell
cd Source/ThirdParty
git clone https://github.com/Microsoft/vcpkg.git _vcpkg
cd _vcpkg
./bootstrap-vcpkg.sh
cd ..
./_vcpkg/vcpkg install --x-install-root=. --triplet=x64-linux
```
</details>

## License

See the [MIT LICENCE](LICENSE.md) file for details.

