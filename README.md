# Cesium Street-lights for Unreal

A plugin extension for the [Cesium for Unreal](https://github.com/CesiumGS/cesium-unreal) plugin that enables users to create and manage street-lighting from OpenStreetMap data.

![Picture of Zurich at night](Resources/ZurichAtNight.png)

> [!CAUTION]  
> This repository is in **early development** and is not ready **at all** for any production or development use yet.
> Not working version is available on this GitHub repository for now.

## Features

- ⌛ Parsing of `.osm.pbf` files to extract street-lighting data.
- ⌛ Parsing of `.parquet` files to extract street-lighting data.
- ✅ Parsing of `.gol` files to extract street-lighting data.
- ⌛ HTTP API to fetch street-lighting data from OSM or Overpass.
- ⌛ Independent height adjustment per street-light.
- ✅ Customisable light style.
- ⌛ LOD selection.
- ⌛ Customizable lighting parameters per LOD.

## Data-sources:
- ✅ Geodesk `.gol` files ([ref](https://www.geodesk.com/))
- ⌛ GeoJSON files
- ⌛ OpenStreetMap `.osm.pbf` files
- ⌛ OpenStreetMap Overpass API

### Editor specific

- ✅ Add static streetlights in a specified radius from a geolocation.
- ⌛ Add static streetlights in a specified bounding-box area.

### Runtime specific

- ⌛ Add streetlights dynamically in the main controller camera view.

## Installation from a release

You can simply download the latest release and copy the `CesiumStreetlightsForUnreal` folder into the `Plugins` folder of your Unreal project/engine.

## Installation from Fab

> [!NOTE]
> Not available yet.

## License

See the [MIT LICENCE](LICENSE.md) file for details.
