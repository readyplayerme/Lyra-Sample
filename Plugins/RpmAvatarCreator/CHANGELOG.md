# Changelog

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [1.2.0] 2023-09-26

## Breaking
- Removed body type selection screen.

## Added
- Asset precompile, this will allow faster response time when the assets are selected.

## Fixed
- Removed the saved files when the Subdomain or AppId is changed
- Change the error with the warning when AppId or Subdomain is not set

## [1.1.0] 2023-09-14

## Breaking
- Subdomain parameter is removed from the avatar creator widget constructor. Subdomain needs to be set in the project settings.

## Added
- Added Ability to render any kind of environment and actor, without modifying the sample.

## Fixed
- Updated avatar loading popup UI
- Fixed deprecation warnings for UE5.2
- Cleared the code text field when retrying to log in

## [1.0.0] 2023-09-01

## Breaking
- Partner Domain parameter is renamed to Subdomain in avatar creator widget.

## Added
- Support for modular outfits

## Fixed
- Assets are downloaded with parallel requests

## [0.5.0] 2023-08-24

## Breaking
- AppId parameter is required by the avatar creator. It can be set in the **Project Setting > Game > Ready Player Me**

## Fixed
- Logout the user when the session expires
- Disabled the left moue drag. Use scroll and right mouse button drag instead
- Replaced the asset endpoint with the new endpoint
- Refactored image downloading logic to exclude unused images
- Updated the animation of the preview avatar

## [0.4.0] 2023-07-18

## Added
- Support for the full-body costumes
- AssetUnlockView that is shown when the locked asset is selected
- Asset unlocked button click event
- RpmAvatarEditorUI cpp class for handling the asset logic
- AssetButtonUI and ColorButtonUI as the parent class for the assets

## Fixed
- UI fixes for UE5
- Fixed layout for the category buttons

## Update
- Updated the steps to have the same background for all the steps
- Updated the eye color texture fetching logic to fetch a cropped image
- Updated asset selected icon

## [0.3.0] 2023-06-21

## Added

- Api for fetching avatar templates
- [sample structure](Documentation/SampleStructure.md) and [customization guide](Documentation/CustomizationGuide.md) readme files
- Request cancellation functionality, for canceling all the requests when the avatar creator is closed.
- Signup popup, that appears after saving an avatar

## Fixed

- Fixed the preview actor scale for big avatars
- Added Missing includes for UE5.2
- UI Fixes for the sample project
- Delete the avatar draft when the back button is clicked in the editor

## Update

- Replaced default avatars with avatar template implementation
- Added `WBP_AssetHandler` widget for handling asset logic
- Extracted `WBP_BadgeIcon` from the avatar editor

## [0.2.1] 2023-04-19

## Added

- Added CloseButtonClicked event to properly handle.

## Fixed

- Changed the spawned actor location to avoid lighting issues.
- Selfie page layout
- Hide profile dropdown when clicking on the scene

## Update

- Increased scrolling speed for the scroll boxes.

## [0.2.0] 2023-04-17

## Added

- API for fetching the default avatars.
- API for login with with a verification code.
- API for fetching the user avatars.
- API for color selection.
- UI for the new APIs in the sample.
- Ability to edit the existing avatars

## [0.1.0] 2023-02-24

## Added

- API for updating, creating and saving an avatar.
- API for authentication.
- API for fetching the partner assets.
- An example creator replicating the web creator natively.
