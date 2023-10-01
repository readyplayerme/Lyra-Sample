# Customization Guide

The same UI of the custom avatar creator is entirely made with blueprints.
This is done on purpose so that it would be easier to duplicate and customize it.

## Requirements
You are allowed to change the entire UI, the only thing that you are required to have in your custom implementation is the **Ready Player Me sign-in** button and **Ready Player Me account-creation** ui.
This is a legal requirement from **Ready Player Me**.<br>
![Screenshot 2023-06-16 105931](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/bc449b10-4455-48ef-aec9-6db7dd18448b)
![Screenshot 2023-06-16 105914](https://github.com/readyplayerme/rpm-unreal-avatar-creator/assets/3124894/44d495e8-9292-421c-8cb6-85a0015d4b88)

## Customization Steps
To make a custom UI for the avatar creator, you need to do the following steps:

1. Make sure that you have the [**ReadyPlayerMe**](https://github.com/readyplayerme/rpm-unreal-sdk) and [**RpmAvatarCreator**](https://github.com/readyplayerme/rpm-unreal-avatar-creator/) plugins set up for your project.
2. Make a folder in your project's content folder and Duplicate the files from the [`RpmAvatarCreator` content](https://github.com/readyplayerme/rpm-unreal-avatar-creator/tree/master/Content) folder into it.
3. Make sure that you can add the duplicated avatar creator widget to the viewport, and that there are no errors.
4. Read the [SampleStructure.md](SampleStructure.md) readme file to better understand the components of the sample UI.
5. When making cosmetic changes to the duplicated UI, make sure that the public variables of the UI blueprints are not removed. All the public variables have blueprint logic connected with them.
6. If you still need to remove the public variables, make sure that you understand the blueprint logic connected with them.

## Up-To-Date Sample
While duplicating the sample you will not be able to have up-to-date changes from the sample UI.
To have an up-to-date version of the custom avatar creator, the following steps are required:
1. First follow the [**RpmAvatarCreator**](https://github.com/readyplayerme/rpm-unreal-avatar-creator/releases) repository to be notified about the new releases.
2. Update the ReadyPlayerMe and RpmAvatarCreator plugins in your project to the latest released version.
3. Open the [CHANGELOG.md](../CHANGELOG.md) readme file to check the changes.
4. Open the changed UI blueprint files that are mentioned in the readme.
5. Try to apply the UI changes to your duplicated custom version of the avatar creator.

## Cosmetic Customization Example
Currently, the sample is designed for the landscape mode.
If your application is made for portrait mode, you need to make your own custom version of the avatar creator UI.
First, duplicate the sample in your project. One by one open all the widgets that don't look good on the portrait mode.
Change the layout of the widgets, without removing the public variables.
Keep an eye on the **RpmAvatarCreator** repository, to update your local version of avatar creator if a new version is released.

## Functional Customization Example
If you'd like to entirely get rid of the navigation panel, because your application already has a navigation panel, these are the steps that you need to do.
AvatarCreator Widget uses the navigation panel for button visibility and responding to the the clicked buttons.
You need to replace the navigation panel reference in avatar creator with your own navigation panel.
In your own navigation panel add buttons that you need(save, back, ...). Add a blueprint interface NavigationSwitcher to your navigation panel to have a connection with the AvatarCreator.
Finally, fix all the blueprint errors connected with the replaced navigation panel.
